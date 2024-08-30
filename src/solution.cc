
#include "solution.h"

#include "partial_state_graph.h"
#include "simulator.h"

SolutionStep::SolutionStep(PR2State *s, PSGraph *psg, int d, const PR2OperatorProxy o, int exid, bool is_r, bool is_g, bool is_s) :
                PolicyItem(s),
                containing_graph(psg),
                op(o),
                distance(d),
                is_relevant(is_r),
                is_goal(is_g),
                is_sc(is_s),
                expected_id(exid),
                step_id(PR2.solution.num_steps_created++)
{
    // Resize the successors to the right number of outcomes. Change
    //  this if you have a complex nondet successor function in the
    //  expand.* files.
    if (!is_g)
        succ.resize(PR2.general.nondet_mapping[op.nondet_index].size(), nullptr);
    // Inform the PSGraph that we've created another SolutionStep
    containing_graph->add_step(this);

    #ifndef NDEBUG
    if (PR2.logging.log_solstep(step_id))
        cout << "\nSOLSTEP(" << PR2.logging.id() << "): Created new step " << step_id << endl;
    #endif
}

SolutionStep* SolutionStep::copy() {

    #ifndef NDEBUG
    if (PR2.logging.log_solstep(step_id))
        cout << "\nSOLSTEP(" << PR2.logging.id() << "): Copying " << step_id << endl;
    #endif

    return new SolutionStep(new PR2State(*state),
                            containing_graph,
                            distance,
                            op,
                            expected_id,
                            is_relevant,
                            is_goal,
                            is_sc);
}

string SolutionStep::get_name() {
    if (is_goal)
        return "goal / SC / d=0";
    else
        return op.get_nondet_name() + " / " + (is_sc ? "SC" : "NSC") + " / d=" + to_string(distance);
}

void SolutionStep::dump() const {
    cout << "\nSolution Step (" << step_id << ")" << endl;
    cout << "- Distance: " << distance << endl;
    cout << "- Relevant: " << is_relevant << endl;
    cout << "- SC: " << is_sc << endl;
    cout << "- Next Steps:";
    for (auto s : succ)
        if (s)
            cout << " " << s->step_id;
        else
            cout << " -";
    cout << endl;
    cout << "- Previous Steps:";
    for (auto s : pred)
        cout << " " << s->step_id;
    cout << endl;
    if (!is_goal) {
        cout << "\n -{ Operator }-" << endl;
        op.dump();
    } else {
        cout << "\n -{ Goal }-" << endl;
    }
    cout << "\n -{ State }-" << endl;
    state->dump_pddl();
    cout << "" << endl;
}

bool SolutionStep::operator< (const SolutionStep& other) const {
    if (is_active != other.is_active)
        return is_active;
    else if (is_sc != other.is_sc)
        return is_sc;
    else if (distance != other.distance)
        return distance < other.distance;
    else
        return step_id > other.step_id;
}

void SolutionStep::strengthen(PR2State *context) {

    // Essentially, this method will fill in undefined variable settings
    //  of the solstep partial state using the context from which it was
    //  created, and do so greedily until every possible FSAP that would
    //  conflict with this solstep is in mutex. This could be done more
    //  carefully with a hitting set computation, but that might incur a
    //  computational cost that isn't worth it in the end.

    // If this step is a goal, then it will never be forbidden
    if (is_goal)
        return;

    // Fetch all of the forbidden items from this context state
    vector<FSAP *> reg_items;
    PR2.deadend.policy->generate_consistent_items<FSAP>(*state, reg_items, false);

    // Each item could potentially be a forbidden state-action pair
    for (auto fsap : reg_items) {

        // If this holds, then we may trigger the forbidden pair
        if (fsap->get_index() == op.nondet_index) {

            for (unsigned j = 0; j < PR2.general.num_vars; j++) {

                int val = (*(fsap->state))[j];

                // We may have broken it in a previous iteration
                if (!(fsap->state->is_undefined(j)) && // FSAP variable is set...
                    !(state->is_undefined(j)) &&       // and the solstep variable is set...
                    (val != (*state)[j]))  // and differs from solstep.
                        break;

                // Just need to break one of the decisions with the current context
                if (!(fsap->state->is_undefined(j)) && (val != (*context)[j])) {

                    assert(state->is_undefined(j));

                    (*state)[j] = (*context)[j];
                    break;

                }
            }
        }
    }
}

void SolutionStep::validate(set< PR2SearchNode * > &matching_nodes) {

#ifndef NDEBUG
    // Every previous solstep should have this as a successor
    for (auto prevss : pred)
        assert(find(prevss->succ.begin(), prevss->succ.end(), this) != prevss->succ.end());

    // Every successor solstep should have this as a predecessor
    for (auto succss : succ)
        if (succss)
            assert(succss->has_predecessor(this));
#endif

    // Check the entailment invariant for the solution graph
    //  Note: This is a memory leak, but only when we're debugging and
    //        running assertions.
    for (auto searchnode : matching_nodes) {
        int outcome = -1;
        for (auto succss : succ) {
            outcome += 1;
            if (succss) {

                int op_ind = PR2.general.nondet_mapping[op.nondet_index][outcome];
                PR2OperatorProxy used_op = PR2.proxy->get_operators()[op_ind];
                bool failed = !(state->entails(*(succss->state->regress(used_op, searchnode->full_state))));

                if (failed || PR2.logging.network_assertions) {
                    cout << "\nVALIDATIONS(" << PR2.logging.id() << "): Regressing solstep <dst> to <src> with <op> and associated <state> / <search node>:\n" << endl;
                    cout << "----{ dst }----\n" << endl;
                    succss->dump();
                    cout << "----{ src }----\n" << endl;
                    dump();
                    cout << "----{ op }----\n" << endl;
                    used_op.dump();
                    cout << "\n----{ state }----\n" << endl;
                    searchnode->full_state->dump_pddl();
                    cout << "\n----{ search node }----\n" << endl;
                    searchnode->dump();
                    cout << "\n-----------" << endl;
                    cout << "Entails: " << !failed << endl;
                    cout << "Consistent: " << state->consistent_with(*(succss->state->regress(used_op, searchnode->full_state))) << endl;
                }

                assert(!failed);
            }
        }
    }
}

void SolutionStep::record_snapshot(ofstream &outfile, string indent) {
    if (op.get_id() == -1)
        return;
    outfile << indent << "\"" << step_id << "\": {" << endl;
    if (is_goal)
        outfile << indent << "  \"expected_successor\": false," << endl;
    else if (!(get_expected_successor()))
        outfile << indent << "  \"expected_successor\": false," << endl;
    else
        outfile << indent << "  \"expected_successor\": \"" << get_expected_successor()->step_id << "\"," << endl;
    
    outfile << indent << "  \"action\": \"" << op.get_nondet_name() << "\"," << endl;
    outfile << indent << "  \"state\": \"" << state << "\"," << endl;
    outfile << indent << "  \"distance\": " << distance << "," << endl;
    outfile << indent << "  \"is_relevant\": " << is_relevant << "," << endl;
    outfile << indent << "  \"is_goal\": " << is_goal << "," << endl;
    outfile << indent << "  \"is_sc\": " << is_sc << "," << endl;
    outfile << indent << "  \"successors\": [" << endl;
    int i = 0;
    for (auto s : succ) {
        if (i != 0)
            outfile << "," << endl;
        outfile << indent << "    {" << endl;
        int op_ind = PR2.general.nondet_mapping[op.nondet_index][i++];
        outfile << indent << "        \"outcome_label\": \"" << PR2.proxy->get_operators()[op_ind].get_name() << "\"," << endl;
        outfile << indent << "        \"successor_id\": ";
        if (s)
            outfile << "\"" << s->step_id << "\"" << endl;
        else
            outfile << "\"-1\"" << endl;
        outfile << indent << "    }";
    }
    outfile << endl << indent << "   ]" << endl;
    outfile << indent << "}";
}


Solution::Solution(Simulator *sim) {
    simulator = sim;
    score = 0.0;
    network = new PSGraph();
    policy = new Policy();

    // Create an initial default goal solution step
    PR2State * gs = new PR2State();
    for (auto goal_tuple : PR2.localize.original_goal)
        (*gs)[goal_tuple.first] = goal_tuple.second;

    SolutionStep * gss = new SolutionStep(gs, network, 0, *(PR2.general.goal_op), -1, true, true, true);
    policy->add_item(gss);
    network->goal = gss;
}

Solution::~Solution() {
    delete network;
    delete policy;
}

PR2OperatorProxy Solution::get_action(const PR2State &state, bool avoid_forbidden) {
    return get_step(state, avoid_forbidden)->op;
}

SolutionStep * Solution::get_step(const PR2State &state, bool avoid_forbidden) {

    vector<SolutionStep *> items;
    policy->generate_entailed_items<SolutionStep>(state, items);

    if (items.empty())
        return nullptr;

    SolutionStep * best;
    if (avoid_forbidden) {

        vector<OperatorID> ops;
        PR2.generate_fsap_aware_applicable_ops(state, ops);
        set<int> opset;
        for (auto oid : ops)
            opset.insert(oid.get_index());
        

        vector<SolutionStep *> non_forbidden_items;
        for (auto item : items) {
            if (item->is_goal || (opset.find(item->op.get_id()) != opset.end())) {
                non_forbidden_items.push_back(item);
            }
        }

        if (non_forbidden_items.empty())
            return nullptr;
        best = *(min_element(non_forbidden_items.begin(), non_forbidden_items.end(), SolutionStepCompare()));
    } else
        best = *(min_element(items.begin(), items.end(), SolutionStepCompare()));

    if (best && best->is_active)
        return best;

    return nullptr;
}

void Solution::evaluate_random() {
    int succeeded = 0;
    for (int i = 0; i < PR2.solution.evaluation_trials; i++)
        if (simulator->simulate_solution(this))
            succeeded++;
    score = double(succeeded) / double(PR2.solution.evaluation_trials);
}

void Solution::evaluate() {
    if (1.0 <= score)
        return;
    evaluate_random();
}

double Solution::get_score() {
    if (0.0 == score)
        evaluate();
    return min(score, 1.0);
}

int Solution::get_size() {
    return policy->size();
}

bool Solution::better_than(Solution * other) {
    if (get_score() != other->get_score())
        return get_score() > other->get_score();
    else if (is_strong_cyclic() != other->is_strong_cyclic())
        return is_strong_cyclic();
    else
        return get_size() > other->get_size();
}

void Solution::rebuild() {
    // First rebuild the graph, which may discard some steps
    clear_dead_solsteps(nullptr);

    // Next, rebuild the policy with the relevant (i.e., active) items
    policy->rebuild();

    score = 0.0;
}

SolutionStep* Solution::incorporate_plan(const DeterministicPlan &plan,
                                         PR2State *start_state,
                                         SolutionStep *goal_step) {

    score = 0.0;

    // Get every complete state going forward for context / strengthening
    vector<PR2State *> states;
    list<PolicyItem *> new_steps;
    states.push_back(new PR2State(*start_state));

    for (auto op : plan)
        states.push_back(states.back()->progress(PR2.proxy->get_operators()[op]));

    // Do the repeated regression and set up the links for the network
    SolutionStep *succ = goal_step;
    SolutionStep *pred = NULL;

    for (int i = plan.size() - 1; i >= 0; i--) {

        // Create the new solution step for this part of the plan
        PR2State *regrps = succ->state->regress(PR2.proxy->get_operators()[plan[i]], states[i]);
        int nondet_outcome = PR2.proxy->get_operators()[plan[i]].nondet_outcome;
        pred = new SolutionStep(regrps, PR2.solution.incumbent->network, succ->distance + 1, PR2.proxy->get_operators()[plan[i]], nondet_outcome);
        new_steps.push_back(pred);

        pred->connect_to_successor(nondet_outcome, succ);

        // Strengthen the step so it doesn't fire an FSAP at some
        // some point. Strengthening is sufficient but not neccessary
        pred->strengthen(states[i]);

        // Set things up for the next loop
        succ = pred;

    }

    // Clean up the (complete) states that were used for regression
    for (auto state : states)
        delete state;

    // Add the new SolutionSteps to the policy
    policy->update_policy(new_steps);

    // If this is the first time we've added things, then pred should be
    //  the init step for the network
    if (!(network->init))
        network->init = pred;

    // Finally, return the SolutionStep at the start of the new plan
    assert(pred);
    return pred;
}

void Solution::insert_step(SolutionStep * step) {

    score = 0.0;

    //
    // Nothing to do for the graph
    //
    // Insert the individual step into the policy
    policy->add_item(step);
}

void Solution::insert_steps(list<PolicyItem *> &steps) {
    if (steps.empty())
        return;

    score = 0.0;

    //
    // Nothing to do for the graph
    //
    // Insert the steps into the policy all at once
    policy->update_policy(steps);
}

void Solution::clear_dead_solsteps(map< SolutionStep* , set< PR2SearchNode * > * > * solstep2searchnode) {

    // Get all of the steps reachable from the initial one
    set< SolutionStep * > seen;
    network->crawl_steps(network->init, false, seen);

    // Remove all of the dead space
    set< SolutionStep * > dead;
    set_difference(network->steps.begin(), network->steps.end(),
                   seen.begin(), seen.end(),
                   inserter(dead, dead.end()));

    for (auto s : dead) {

        // Make sure things look the way they should
        assert(seen.find(s) == seen.end());
        if (solstep2searchnode) {
            assert(solstep2searchnode->find(s) != solstep2searchnode->end());
            assert(0 == (*solstep2searchnode)[s]->size());
        }

#ifndef NDEBUG
        for (auto ss : s->get_predecessors())
            assert(dead.find(ss) != dead.end());
#endif

        // Remove the solstep from all the places
        for (int sid = 0; sid < s->num_successors(); sid++)
            if (s->has_successor(sid))
                s->unconnect_from_successor(sid);

        if (solstep2searchnode) {
            delete (*solstep2searchnode)[s];
            solstep2searchnode->erase(s);
        }
        network->remove_step(s);
        s->is_active = false;

    }

}

bool Solution::is_strong_cyclic() {
    return ((network->init) && (network->init->is_sc));
}

void Solution::record_snapshot(ofstream &outfile, map< SolutionStep* , set< PR2SearchNode * > * > &solstep2searchnode, list< PR2SearchNode * > * created_search_nodes, string type, string indent) {

    outfile << indent << "\"solution\": {" << endl;

    outfile << indent << "  \"type\": \"" << type << "\"," << endl;
    outfile << indent << "  \"score\": " << get_score() << "," << endl;
    outfile << indent << "  \"size\": " << get_size() << "," << endl;
    outfile << indent << "  \"round\": " << PR2.logging.fond_search_count << "," << endl;

    network->record_snapshot(outfile, indent+"  ");
    outfile << "," << endl;
    policy->record_snapshot(outfile, indent+"  ");

    outfile << indent << "  \"ps2fs\": {" << endl;
    for (auto ss : solstep2searchnode) {
        outfile << indent << "    " << ss.first->step_id << ": {";
        for (auto sn : *(ss.second))
            outfile << sn->id << ": " << sn->id << ", ";
        outfile << "}," << endl;
    }
    outfile << indent << "  }," << endl;

    vector< pair <PR2SearchNode *, PR2SearchNode *> > search_node_direct_links;
    vector< pair <PR2SearchNode *, PR2SearchNode *> > search_node_repeat_links;

    outfile << indent << "  \"pr2searchnodes\": {" << endl;
    for (auto sn : *created_search_nodes) {
        sn->record_snapshot(outfile, indent+"  ");

        // The first is the direct link that was used when sn was created
        if (!(sn->previous_nodes.empty()) && !(sn->init))
            search_node_direct_links.push_back(make_pair(sn->previous_nodes[0], sn));
    }
    outfile << indent << "  }," << endl;

    outfile << indent << "  \"pr2searchnodelinks\": [" << endl;
    for (auto link : search_node_direct_links)
        outfile << indent << "    [" << link.first->id << "," << link.second->id << "]," << endl;
    outfile << indent << "  ]," << endl;

    outfile << indent << "}," << endl;
}
