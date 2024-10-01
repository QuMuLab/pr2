
#include "fond_search.h"

#include "expand.h"
#include "simulator.h"
#include "solution.h"
#include "partial_state_graph.h"





/********************
 * Main FOND Search *
 ********************/

bool find_better_solution(Simulator *sim) {

    PR2.logging.fond_search_count++;

    PR2SearchStatus * status;

    cout << "\n\nFOND Search: Round " << PR2.logging.fond_search_count << endl;

    // Restore the search if we stopped early due to an epoch timeout
    if (PR2.epoch.last_search_status) {

        cout << "Restoring the search from a previous epoch..." << endl;
        status = PR2.epoch.last_search_status;
        status->warm_start = true;

    } else {

        cout << "Starting a fresh search..." << endl;
        status = new PR2SearchStatus(sim);

        // For now, we only record the search nodes if we need them in snapshots
        if (PR2.logging.dump_snapshots)
            status->created_search_nodes = new list< PR2SearchNode * >();

        // Back up the originial initial state
        status->old_initial_state = PR2.proxy->generate_new_init();
        // Build the goal state
        status->goal_orig = new PR2State();
        for (auto goal_pair : PR2.proxy->get_goals())
            (*(status->goal_orig))[goal_pair.get_variable().get_id()] = goal_pair.get_value();

        status->init();
        status->current_state = PR2.proxy->generate_new_init();
        status->current_goal = new PR2State(*(status->goal_orig));

        PR2SearchNode *init_node = new PR2SearchNode(status->current_state, status->current_goal, NULL, NULL, -1);
        if (status->created_search_nodes)
            status->created_search_nodes->push_back(init_node);
        status->open_list->push(init_node);

        status->created_states->push_back(status->current_state);
        status->created_states->push_back(status->current_goal);
    }

    if (!PR2.logging.fond_search)
        cout << "\n {" << flush;

    // Keep going while there's still time and until we've closed off
    //  the entire open list or found a strong cyclic incumbent.
    while (status->keep_searching() && !(PR2.solution.incumbent->is_strong_cyclic())) {
        // Commonly used snapshot logging code
        status->validate_if_needbe();
        status->snapshot_if_needbe();

        // Kick things off by selecting the next PR2SearchNode
        status->pop_next_node();

        /**************************************************************
         * There are six possible ways to handle a new search state
         *
         *  1) The popped search node is either poisoned or should
         *     be flagged as such. A poisoned node stems from a chain
         *     that we have identified to be sub-optimal (in the vanilla
         *     FOND case, this means some outcome leads to an unavoidable
         *     deadend).
         *
         *  2) We pop a new PR2SearchNode, but the complete state has
         *     already been handled by some other part of the search.
         *     In this case, we want to hook things up properly in both
         *     the search-node graph and the solstep network.
         *
         *  3) The parent step has a solution step already for the
         *     successor, and we just need to make sure things will
         *     connect ok. I.e., follow the solution graph if at all
         *     possible.
         *
         *  4) There isn't a successor decided on yet, but we can
         *     find a valid connection in the solution graph some
         *     place else. I.e., augment the solution graph with a
         *     new edge.
         *
         *  5) We don't know how to achieve the goal yet in a state
         *     that looks like this, so we compute a weak plan and
         *     then update things appropriately. I.e., augment the
         *     solution graph with new nodes / edges.
         *
         *  6) The goal cannot be achieved in the determinization,
         *     so we treat this as a deadend (attempting to find a
         *     generalization if possible), and then continue.
         *
         **************************************************************/

        bool handled_state = false;

        // Case 1 //
        // Don't bother doing anything if this is dead search space
        handled_state = case1_poisoned_node(status);

        // Case 2 //
        // If we've seen the state, then we need to re-write the nodes
        //  and solsteps so that we have a proper merger.
        if (!handled_state)
            handled_state = case2_match_complete_state(status);
        // If the node isn't poised or a duplicate, then we record it as a new state in the seen list, etc.
        if (!handled_state)
            status->record_new_state();

        // Case 3 //
        // See if this part of the solution graph is already done
        if (!handled_state)
            handled_state = case3_predefined_path(status);

        // Case 4 //
        // See if we can hook things up in the solution graph
        if (!handled_state)
            handled_state = case4_hookup_solsteps(status);

        // Case 5 //
        // See if we can find a new path to the solution graph
        if (!handled_state)
            handled_state = case5_new_path(status);

        // Case 6 //
        // When all else fails, this must be a deadend state
        bool failed_initial_state = false;
        if (!handled_state)
            failed_initial_state = case6_deadend(status);

        if (failed_initial_state)
            return false;
    }


    /********************************************************************
     * In the final phase of a round, we update the incumbent as needed,
     * record the state of affairs if we're stopping because of an epoch,
     * and handle the debugging required.
     *
     ********************************************************************/

    bool time_limit_hit = !PR2.time.time_left();

    // Reset the original goal and initial state
    sim->set_state(status->old_initial_state);
    sim->set_goal(status->goal_orig);

    // Do a full marking of the psgraph for proper analysis
    PR2.solution.incumbent->network->full_marking();

    // Commonly used snapshot logging code
    status->validate_if_needbe();
    status->snapshot_if_needbe();
    status->log_end_of_round();

    // See if we need to update the best policy found so far
    status->update_incumbent_if_needbe();

    // Update the deadends if we need to
    status->update_deadends_if_needbe();

    bool run_again = status->need_to_rerun();

    // If we don't need to re-run, and we didn't finish early, then the psgraph
    //  must be comlete -- thus the initial state must be strong cyclic.
    assert(!PR2.deadend.enabled || run_again || PR2.solution.incumbent->is_strong_cyclic());

    // Store the rollout in case we want to pick the search up in the
    //  next epoch or final fsap-free round
    if (time_limit_hit && !status->need_to_rerun())
        status->save_for_epoch();
    else
        delete status;

    return run_again;
}



/********************************************************************
 * Helper functions for the FOND search (i.e., the case-based code) *
 ********************************************************************/
// Case 1 //
// See if this node is poisoned, or should be flagged as such //
bool case1_poisoned_node(PR2SearchStatus * SS) {
    if (!PR2.deadend.poison_search)
        return false;

    SS->last_round_type = "(case-1) Poisoned node";

    bool poisoned = false;

    // See if we're already poisoned
    if (SS->current_node->poisoned)
        poisoned = true;
    // Check if this state is a recognized deadend
    else if (PR2.deadend.states->check_entailed_match(*(SS->current_state)) ||
             is_deadend(*(SS->current_state))) {
        poisoned = true;
        SS->poisoned = true;
        SS->current_node->poison();
        SS->mark_current_state_failed();
    }

    if (poisoned && PR2.logging.fond_search) {
        cout << "\nFONDSEARCH(" << PR2.logging.id() << "): Current node found to be poisoned:" << endl;
        SS->current_node->dump();
    }

    return poisoned;
}


// Case 2 //
// See if the complete state just popped matches something we already handled //
bool case2_match_complete_state(PR2SearchStatus * SS) {

    if (!SS->repeat_state())
        return false;

    SS->last_round_type = "(case-2) Matched complete state\\n -- No modification";

    if (PR2.logging.fond_search) {
        cout << "\nFONDSEARCH(" << PR2.logging.id() << "): Matched on the complete state:" << endl;
        SS->current_node->dump();
        SS->current_state->dump_pddl();
    }

    // We should never be in here for the initial state search node,
    //  which is the only one that cannot have a predecessor.
    assert(SS->previous_step);
    assert(SS->previous_node);

    // Point the previous node to the original search node for
    //  this state, and add it as a pointer back.
    PR2SearchNode * original_node = (*(SS->state2searchnode))[*(SS->current_state)];
    assert(original_node);

    // If this is truely a duplicate, then we don't need to do anything
    if (original_node != SS->current_node) {

        SS->last_round_type = "(case-2) Matched complete state\\n -- Modifying just search nodes";

        bool found = false;
        for (unsigned i = 0; i < SS->previous_node->next_nodes.size() && !found; i++) {
            if (SS->current_node == SS->previous_node->next_nodes[i]) {
                found = true;
                SS->previous_node->next_nodes[i] = original_node;
            }
        }
        assert(found);
        original_node->previous_nodes.push_back(SS->previous_node);
        original_node->previous_node_outcomes.push_back(SS->prev_to_curr_outcome);

        // Hook up the solsteps if we need to, and do the FPR / marking
        //  that may now be possible. If solstep doesn't exist, then
        //  it means that we've determined this state to be a deadend
        //  anyways. This is because case 3-6 must have already occurred
        //  for the original_node, and 3-5 would have added the appropriate
        //  solstep while 6 would treat things as a deadend.
        SolutionStep * solstep = original_node->matched_step;
        if (solstep) {

            if (SS->previous_step->has_successor(SS->prev_to_curr_outcome))
                SS->last_round_type = "(case-2) Matched complete state\\n -- Modifying search nodes and rewriting solution graph connections";
            else
                SS->last_round_type = "(case-2) Matched complete state\\n -- Modifying search nodes and new solution graph connection";

            // Finally, make the connection, go back, and strengthen things up
            strengthen_and_mark(SS, SS->previous_step, solstep, solstep,
                                SS->previous_node, original_node,
                                SS->prev_to_curr_outcome);
        }

        // Mark this node as subsumed (it's not going any further)
        SS->current_node->subsumed = true;
    }

    return true;
}


// Case 3 //
// See if this part of the solution graph is already done
bool case3_predefined_path(PR2SearchStatus * SS) {

    SolutionStep * solstep = SS->previous_step->get_successor(SS->prev_to_curr_outcome);

    if (solstep) {

        SS->last_round_type = "(case-3) Predefined Path";

        if (PR2.logging.fond_search)
            cout << "\nFONDSEARCH(" << PR2.logging.id() << "): Handled by Case-3 (pre-defined path)" << endl;

        // We assume that the newly reached state matches the
        //  target solstep, because otherwise there wouldn't be
        //  a target solstep that exists -- when an extension
        //  through case 4 or 5 is made, it will split away all
        //  of the inconsistent full-states that share the same
        //  abstract path. Thus, this check becomes an invariant.

        solstep->state;

        assert(SS->current_state->entails(*(solstep->state)));
        assert(SS->previous_step);

        SS->current_node->expand(SS, solstep);

        return true;
    }
    return false;
}


// Case 4 //
// See if we can hook things up in the solution graph
bool case4_hookup_solsteps(PR2SearchStatus * SS) {

    // See if we can already handle this state by a new hookup in the solution graph
    SolutionStep * solstep = PR2.solution.incumbent->get_step(*(SS->current_state));

    if (solstep) {

        SS->last_round_type = "(case-4) Hooking Up";

        if (PR2.logging.fond_search)
            cout << "\nFONDSEARCH(" << PR2.logging.id() << "): Handled by Case-4 (connecting up solsteps)" << endl;

        // Expand the state given the new solstep connection
        SS->current_node->expand(SS, solstep);

        strengthen_and_mark(SS, SS->previous_step, solstep, solstep,
                            SS->previous_node, SS->current_node,
                            SS->prev_to_curr_outcome);

        return true;
    }
    return false;
}


// Case 5 //
// See if we can find a new path to the solution graph
bool case5_new_path(PR2SearchStatus * SS) {

    SS->sim->set_state(SS->current_state);
    SS->sim->set_goal(SS->current_goal);
    SolutionStep * solstep = SS->sim->replan();

    if (solstep) {

        SS->last_round_type = "(case-5) New Path";

        if (PR2.logging.fond_search)
            cout << "\nFONDSEARCH(" << PR2.logging.id() << "): Handled by Case-5 (computing new path)" << endl;

        // Update the statistics, as we just patched up the
        //  policy a little bit.
        SS->num_fixed_states++;
        SS->made_change = true;

        // Go through every one of the states along the new
        //  plan and add the new relevant open leaves to the
        //  search queue. plan_state will be the expected
        //  full state that follows the plan, and we use the
        //  successor generator function to get at the other
        //  (new) leaf states
        PR2State * plan_state = SS->current_state;
        SolutionStep * plan_solstep = solstep;
        PR2SearchNode * expected_node = SS->current_node;
        for (auto op : SS->sim->engine->get_plan()) {

            assert(op.get_index() == plan_solstep->op.get_id());
            assert(plan_state->entails(*(plan_solstep->state)));

            if (op.get_index() != plan_solstep->op.get_id())
                cout << "ERROR: op and plan_solstep->op don't match!" << endl;

            if (PR2.logging.fond_search_expanding) {
                cout << "\nFONDSEARCH-EXPANSION(" << PR2.logging.id() << "): Inserting seen state:" << endl;
                plan_state->dump_pddl();
            }

            // If this is a new state that we're expanding and assuming part
            //  of the fond search, then we should add the nodes to the respective
            //  data structures as it would if it was just popped off the queue.
            if (SS->current_state != plan_state) {
                assert(0 == SS->seen->count(*plan_state));
                assert(SS->state2searchnode->find(*plan_state) == SS->state2searchnode->end());
                SS->seen->insert(*plan_state);
                (*(SS->state2searchnode))[*plan_state] = expected_node;
            }

            expected_node = expected_node->expand(SS, plan_solstep);

            assert(*expected_node != *(SS->current_node));
            plan_state = expected_node->full_state;
            plan_solstep = plan_solstep->get_expected_successor();

        }

        strengthen_and_mark(SS, SS->previous_step, solstep, plan_solstep,
                            SS->previous_node, SS->current_node,
                            SS->prev_to_curr_outcome);

        return true;
    }
    return false;
}


// Case 6 //
// When all else fails, this must be a deadend state
bool case6_deadend(PR2SearchStatus * SS) {

    SS->last_round_type = "(case-6) Node Unhandled";

    if (PR2.logging.fond_search)
        cout << "\nFONDSEARCH(" << PR2.logging.id() << "): Handled by Case-6 (deadend)" << endl;

    // This only matches when no strong cyclic solution exists
    if (*(SS->current_state) == *(SS->old_initial_state)) {

        SS->log_end_of_round();

        cout << endl;
        cout << "Found the initial state to be a failed one. No strong cyclic plan exists." << endl;
        cout << "Using the best policy found, with a score of " << PR2.solution.best->get_score() << endl;
        cout << endl;

        SS->sim->set_state(SS->old_initial_state);
        SS->sim->set_goal(SS->goal_orig);

        // Commonly used snapshot logging code
        SS->validate_if_needbe();
        SS->snapshot_if_needbe();

        // Use the best policy we've found so far
        if (PR2.solution.incumbent && (PR2.solution.best != PR2.solution.incumbent))
            delete PR2.solution.incumbent;
        PR2.solution.incumbent = PR2.solution.best;

        // Return true so the search stops
        return true;

    } else {
        if (PR2.deadend.poison_search)
            SS->current_node->poison();
        SS->mark_current_state_failed();
        return false;
    }
}

void strengthen_and_mark(PR2SearchStatus * status,
                         SolutionStep * previous_step,
                         SolutionStep * solstep,
                         SolutionStep * join_solstep,
                         PR2SearchNode * previous_node,
                         PR2SearchNode * current_node,
                         int successor_id_for_dst) {

    // Strengthen the solsteps all the way back
    list<SolutionStep *> new_steps;

    PR2.solution.incumbent->network->fixed_point_regression(
        previous_step, // src
        solstep, // old_dst
        solstep, // new_dst
        previous_node, // src_node
        current_node, // dst_node
        successor_id_for_dst, // outcome id
        *(status->solstep2searchnode),
        new_steps,
        true
    );

    // If we've created new solsteps, then we need to update the bookeeping
    if (!new_steps.empty())
        PR2.solution.incumbent->insert_steps(new_steps);

    // Clean things up if we're willing to spend the time doing it
    if (PR2.psgraph.clear_dead_solsteps)
        PR2.solution.incumbent->clear_dead_solsteps(status->solstep2searchnode);

    // Finally, update the marking if we can
    if (join_solstep)
        for (auto pred : join_solstep->get_predecessors())
            PR2.solution.incumbent->network->fixed_point_marking(pred);

    // Do the more advanced (i.e., complete) marking if set
    if (PR2.psgraph.full_scd_marking)
        PR2.solution.incumbent->network->full_marking();

}




/***********************
 * Search Node Methods *
 ***********************/

void PR2SearchNode::poison() {

    if (PR2.logging.poisoning) {
        cout << "\nPOISONING(" << PR2.logging.id() << "): Starting a poisoning for the following node:" << endl;
        dump();
    }

    // Start the recursive call (which crawls forwards) on the predecessors.
    //  We start things there, as the predecessor action would be forbidden
    //  in future iterations.
    for (auto pred : previous_nodes)
        pred->poison_recurse();

    // As a special case, if there are multiple predecessors, we need to
    //  call the recursion on this node (as multiple predecessors is a
    //  stopping condition in the recursion).
    if (previous_nodes.size() > 1) {
        poisoned = true;
        PR2.deadend.poison_count++;
        for (auto succ : next_nodes)
            succ->poison_recurse();
    }

    // As another special case, we need to recurse if this is the init node
    if (0 == previous_nodes.size())
        poison_recurse();

    assert(poisoned);
}

void PR2SearchNode::poison_recurse() {

    // Main stopping condition
    if (poisoned)
        return;

    // If we can arrive here multiple ways, then stop poisoning things
    if (previous_nodes.size() > 1)
        return;

    // Otherwise, poison this node and continue
    poisoned = true;
    PR2.deadend.poison_count++;
    for (auto succ : next_nodes)
        succ->poison_recurse();
}

void PR2SearchNode::dump() const {
    cout << "\nPR2SearchNode(" << id << "):" << endl;
    cout << "- Open: " << (open ? "true" : "false") << endl;
    cout << "- Init: " << (init ? "true" : "false") << endl;
    cout << "- Subsumed: " << (subsumed ? "true" : "false") << endl;
    cout << "- Poisoned: " << (poisoned ? "true" : "false") << endl;
    cout << "- Full State: " << full_state << endl;
    cout << "- Expected State: " << expected_state << endl;
    cout << "- Primary Previous Node: " << (previous_nodes.empty() ? -1 : previous_nodes[0]->id) << endl;
    cout << "- Next Nodes:";
    for (auto n : next_nodes)
        cout << " " << n->id;
    cout << endl;
    cout << "- Previous Nodes:";
    for (auto n : previous_nodes)
        cout << " " << n->id;
    cout << endl;
    cout << "- Previous Node Outcomes:";
    for (auto n : previous_node_outcomes)
        cout << " " << n;
    cout << endl;
    cout << "- Matched Step: " << (matched_step ? matched_step->step_id : -1) << endl;
    cout << "- Parent Step: " << (parent_step ? parent_step->step_id : -1) << endl;
}

void PR2SearchNode::record_snapshot(ofstream &outfile, string indent) {
    outfile << indent << id << ": {" << endl;
    if (next_nodes.size() > 0)
        if (next_nodes[0]->parent_step)
            outfile << indent << "    name: \"" << "(" << id << ") " << next_nodes[0]->parent_step->op.get_nondet_name() << "\"," << endl;
        else
            outfile << indent << "    name: \"" << "(" << id << ") ???" << "\"," << endl;
    else
        outfile << indent << "    name: \"(" << id << ")\"," << endl;
    outfile << indent << "    open: " << open << "," << endl;
    outfile << indent << "    init: " << init << "," << endl;
    outfile << indent << "    poisoned: " << poisoned << "," << endl;
    outfile << indent << "    subsumed: " << subsumed << "," << endl;
    outfile << indent << "}," << endl;
}

void PR2SearchNode::validate() {

    if (PR2.logging.network_assertions) {
        cout << "\nVALIDATIONS(" << PR2.logging.id() << "): Validating for the following node:" << endl;
        dump();
    }

    // Nothing needs to match up if this node is subsumed.
    if (subsumed)
        return;

#ifndef NDEBUG
    // Every previous node should have this as a next node
    for (auto pn : previous_nodes)
        assert(find(pn->next_nodes.begin(),
                    pn->next_nodes.end(),
                    this)
                != pn->next_nodes.end());

    // Every next node should have this as a previous node
    for (auto sn : next_nodes)
        assert(find(sn->previous_nodes.begin(),
                    sn->previous_nodes.end(),
                    this)
                != sn->previous_nodes.end());
#endif

    // Make sure the vector lengths match up
    assert(previous_nodes.size() == previous_node_outcomes.size());

    // If we have a parent step and matched step, then they should jive
    if (parent_step && matched_step) {
        // matched_step is a successor
        assert(find(parent_step->get_successors().begin(),
                    parent_step->get_successors().end(),
                    matched_step)
                != parent_step->get_successors().end());

        // parent_step is a predecessor
        assert(matched_step->has_predecessor(parent_step));
    }
}

PR2SearchNode * PR2SearchNode::expand(PR2SearchStatus * SS, SolutionStep * solstep) {

    if (PR2.logging.fond_search_expanding) {
        cout << "\nFONDSEARCH-EXPANSION(" << PR2.logging.id() << "): Expanding the current search node:" << endl;
        dump();
        full_state->dump_pddl();
        cout << "...with SolStep..." << endl;
        solstep->dump();
    }

    if (SS->solstep2searchnode->find(solstep) == SS->solstep2searchnode->end())
        (*(SS->solstep2searchnode))[solstep] = new set< PR2SearchNode * >();
    (*(SS->solstep2searchnode))[solstep]->insert(this);
    matched_step = solstep;

    // If the solstep is a goal node or already marke strong cyclic, then don't expand
    if (solstep->is_goal || solstep->is_sc)
        return this;

    vector< NondetSuccessor * > successors;
    PR2SearchNode * expected_node = NULL;
    PR2State * full_expected_state = generate_nondet_successors(full_state, &(solstep->op), successors);
    PR2State * expected_state = full_expected_state;
    SolutionStep *expected_step = PR2.solution.incumbent->get_step(*expected_state);

    if (PR2.logging.fond_search_expanding) {
        cout << "\nFONDSEARCH-EXPANSION(" << PR2.logging.id() << "): Expected successor state:" << endl;
        full_expected_state->dump_pddl();
    }

    if (PR2.localize.enabled && PR2.localize.generalize && expected_step) {
        expected_state = new PR2State(*(expected_step->state));
        SS->created_states->push_back(expected_state);
    }

    for (auto succ : successors) {
        SS->created_states->push_back(succ->state);
        PR2SearchNode * new_node = new PR2SearchNode(succ->state,
                                                     expected_state,
                                                     this,
                                                     solstep,
                                                     succ->id);

        if (PR2.logging.fond_search_expanding) {
            cout << "\nFONDSEARCH-EXPANSION(" << PR2.logging.id() << "): Adding new PR2SearchNode:" << endl;
            new_node->dump();
            succ->state->dump_pddl();
        }

        SS->open_list->push(new_node);
        if (SS->created_search_nodes)
            SS->created_search_nodes->push_back(new_node);
        if (solstep->op.nondet_outcome == succ->id) {
            assert(*full_expected_state == *(succ->state));
            expected_node = new_node;
        }
    }

    assert(expected_node);

    return expected_node;
}

bool pr2_node_comparison::operator() (const PR2SearchNode * n1, const PR2SearchNode * n2) {
    if (PR2.fondsearch.node_preference == PR2.fondsearch.OPEN_LIST_STACK) {
        return (n1->id < n2->id);
    } else if (PR2.fondsearch.node_preference == PR2.fondsearch.OPEN_LIST_QUEUE) {
        return (n1->id > n2->id);
    } else if (PR2.fondsearch.node_preference == PR2.fondsearch.OPEN_LIST_NEAR_INIT) {
        return (n1->parent_step->distance > n2->parent_step->distance);
    } else if (PR2.fondsearch.node_preference == PR2.fondsearch.OPEN_LIST_AWAY_INIT) {
        return (n1->parent_step->distance < n2->parent_step->distance);
    } else if (PR2.fondsearch.node_preference == PR2.fondsearch.OPEN_LIST_RANDOM) {
        return (PR2.rng.random() < 0.5);
    } else {
        cout << "\n\n\n\t\tError: Unrecognized open list type of " <<
                PR2.fondsearch.node_preference << endl;
        return false;
    }
}


 /*************************
  * Search Status Methods *
  *************************/

PR2SearchStatus::~PR2SearchStatus()  {
    // Clean up the data structures we've created
    for (auto s : *created_states)
        if (s)
            delete s;

    for (auto d : *failed_states)
        if (d)
            delete d;

    for (auto kv : *solstep2searchnode)
        if (kv.second)
            delete kv.second;

    delete seen;
    delete open_list;
    delete created_states;
    delete failed_states;
    delete solstep2searchnode;
    delete state2searchnode;
    if (created_search_nodes)
        delete created_search_nodes;

    poisoned = false;

    if (warm_start)
        PR2.epoch.last_search_status = NULL;
}

void PR2SearchStatus::init()  {
    seen = new set<PR2State>();
    open_list = new priority_queue< PR2SearchNode *, vector< PR2SearchNode * >, pr2_node_comparison >();
    failed_states = new vector<DeadendTuple *>();
    created_states = new vector<PR2State *>();
    solstep2searchnode = new map< SolutionStep* , set<PR2SearchNode *> *>();
    state2searchnode = new map< PR2State, PR2SearchNode * >();
}

bool PR2SearchStatus::keep_searching () {
    return !open_list->empty() && (PR2.time.time_left());
}

bool PR2SearchStatus::repeat_state() {
    return 0 != seen->count(*current_state);
}

bool PR2SearchStatus::need_to_update_incumbent() {
    return made_change || poisoned || (failed_states->size() > 0) || PR2.solution.incumbent->is_strong_cyclic();
}

bool PR2SearchStatus::need_to_update_deadends() {
    return PR2.time.time_left() && PR2.deadend.enabled && (failed_states->size() > 0);
}

bool PR2SearchStatus::need_to_rerun() {
    return (failed_states->size() > 0) || poisoned;
}

void PR2SearchStatus::pop_next_node () {

    num_checked_states++;

    // Avoid the const cast issue...
    current_node = open_list->top();
    current_node->open = false;
    open_list->pop();

    assert(!(current_node->subsumed));

    previous_step = current_node->parent_step;
    current_state = current_node->full_state;
    current_goal = current_node->expected_state;

    // Only should fail the check in the initial state node
    previous_op = NULL;
    previous_node = NULL;
    prev_to_curr_outcome = -1;

    if (previous_step) {
        assert(!(current_node->previous_nodes.empty()));
        previous_node = current_node->previous_nodes[0];
        prev_to_curr_outcome = current_node->previous_node_outcomes[0];
        int prev_op_ind = PR2.general.nondet_mapping[previous_step->op.nondet_index][prev_to_curr_outcome];
        PR2OperatorProxy prev_op_proxy = PR2.proxy->get_operators()[prev_op_ind];
        previous_op = &prev_op_proxy;
    }
}

void PR2SearchStatus::record_new_state () {
    // If this is the first time looking at this state, then we
    //  shouldn't have matched any additional previous nodes to
    //  the current node yet.
    assert(current_node);
    assert(current_node->previous_nodes.size() <= 1);
    assert(state2searchnode->find(*current_state) == state2searchnode->end());

    seen->insert(*current_state);
    (*state2searchnode)[*current_state] = current_node;

    if (PR2.logging.fond_search) {
        cout << "\nFONDSEARCH(" << PR2.logging.id() << "): Tackling the current node / state:" << endl;
        current_node->dump();
        current_state->dump_pddl();
    } else
        cout << "." << flush;
}

void PR2SearchStatus::save_for_epoch() {
    // Add the most recent PR2SearchNode in case we start up another epoch
    if (current_node)
        open_list->push(current_node);

    cout << "Saving the FOND search state settings." << endl;
    PR2.epoch.last_search_status = this;
}



void PR2SearchStatus::update_incumbent_if_needbe() {
    if (need_to_update_incumbent()) {
        if (PR2.solution.incumbent->better_than(PR2.solution.best)) {
            cout << "Found a better policy of score " << PR2.solution.incumbent->get_score() << endl;
            if (PR2.solution.best && (PR2.solution.best != PR2.solution.incumbent))
                delete PR2.solution.best;
            PR2.solution.best = PR2.solution.incumbent;
        }
    }
}

void PR2SearchStatus::update_deadends_if_needbe() {
    if (need_to_update_deadends()) {
        update_deadends(*failed_states);
        // We delete the policy so we can start from scratch next time with
        //  the deadends recorded.
        if (PR2.solution.incumbent && (PR2.solution.best != PR2.solution.incumbent))
            delete PR2.solution.incumbent;
        PR2.solution.incumbent = new Solution(sim);
        made_change = true;
    }
}

void PR2SearchStatus::mark_current_state_failed() {
    if (PR2.deadend.enabled) {
        if (PR2.deadend.generalize)
            generalize_deadend(*(current_state));

        assert (NULL != current_state);
        assert (NULL != previous_node);
        assert (NULL != previous_op);

        if (PR2.logging.fond_search) {
            cout << "\nFONDSEARCH(" << PR2.logging.id() << "): Adding the following DE tuple:" << endl;
            cout << "Old state..." << endl;
            previous_node->full_state->dump_pddl();
            cout << "..applying " << previous_op->get_nondet_name() << " leading to..." << endl;
            current_state->dump_pddl();
        }

        failed_states->push_back(new DeadendTuple(current_state, // The current state that is a deadend
                                                  previous_node->full_state, // The previous state that lead here
                                                  previous_op)); // The precise operator that lead here
    }
}

void PR2SearchStatus::log_end_of_round() {
    if (!(PR2.logging.fond_search))
        cout << "}" << endl;
    cout << "\nCould not close " << failed_states->size() << " of " << num_fixed_states + failed_states->size() << " open leaf states." << endl;
    cout << "Investigated " << num_checked_states << " states for the strong cyclic plan." << endl;
}

void PR2SearchStatus::snapshot_if_needbe() {
    if (PR2.logging.dump_snapshots) {
        cout << "\nFONDSEARCH(" << PR2.logging.id() << "): Recording FOND Search Snapshot #" << PR2.logging.snapshot_num << "\n\n\n\n\n\n" << endl;
        ofstream outfile;
        outfile.open("fond-snapshot." + to_string(PR2.logging.snapshot_num++) + ".out", ios::out);
        string label = last_round_type;
        if (current_node)
            label += " [node " + to_string(current_node->id) + "]";
        PR2.solution.incumbent->record_snapshot(outfile, *solstep2searchnode, created_search_nodes, label);
        outfile.close();
    }
}

void PR2SearchStatus::validate_if_needbe() {
    if (PR2.logging.validate_network_and_nodes) {
        cout << "\nFONDSEARCH(" << PR2.logging.id() << "): Validating the solution steps..." << endl;
        for (auto ss : *solstep2searchnode)
            ss.first->validate(*(ss.second));
        cout << "FONDSEARCH(" << PR2.logging.id() << "): Validating the search nodes..." << endl;
        if (created_search_nodes)
            for (auto sn : *created_search_nodes)
                sn->validate();
    }
}
