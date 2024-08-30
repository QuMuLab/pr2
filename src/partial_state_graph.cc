
#include "partial_state_graph.h"

PSGraph::PSGraph() : init(nullptr), goal(nullptr) {}

PSGraph::~PSGraph() {}

void PSGraph::fixed_point_regression(SolutionStep * src,
                                     SolutionStep * old_dst,
                                     SolutionStep * new_dst,
                                     PR2SearchNode * src_node,
                                     PR2SearchNode * dst_node,
                                     int successor_id_for_dst,
                                     map< SolutionStep*, set< PR2SearchNode * > * > &solstep2searchnode,
                                     list<PolicyItem *> &new_steps,
                                     bool make_connection) {

    #ifndef NDEBUG
    if (src) {
        if (PR2.logging.log_solstep(src->step_id)) {
            cout << "FPR For SolutionStep:" << endl;
            src->dump();
        }
    }
    #endif

    // Don't bother if we're coming from or heading to a poisoned search node
    if ((dst_node && dst_node->poisoned) || (src_node && src_node->poisoned))
        return;

    // Default base case is when we've hit the start of the graph, in
    //  which case we update the graph's init to the newly generated one
    if (!(src)) {
        if (PR2.logging.psgraph)
            cout << "\nPSGRAPH(" << PR2.logging.id() << "): Base case -- at the front of the graph" << endl;
        init = new_dst;
        return;
    }

    if (PR2.logging.psgraph) {
        cout << "\nPSGRAPH(" << PR2.logging.id() << "): Called with src / old_dst / new_dst solsteps (make_connection = " << make_connection << "):" << endl;
        if (src)
            src->dump();
        else
            cout << "src DNE!" << endl;
        if (old_dst)
            old_dst->dump();
        else
            cout << "old_dst DNE!" << endl;
        if (new_dst)
            new_dst->dump();
        else
            cout << "new_dst DNE!" << endl;

        cout << "PSGRAPH(" << PR2.logging.id() << "): src / dst search nodes:" << endl;
        if (src_node)
            src_node->dump();
        else
            cout << "src_node DNE!" << endl;
        if (dst_node)
            dst_node->dump();
        else
            cout << "dst_node DNE!" << endl;
    }

    #ifndef NDEBUG
    if (PR2.logging.psgraph_condensed) {
        int srcid = -1, dstid = -1, ndstid = -1;
        if (src) srcid = src->step_id;
        if (old_dst) dstid = old_dst->step_id;
        if (new_dst) ndstid = new_dst->step_id;
        cout << srcid << " -" << successor_id_for_dst << "-> [ " << dstid << " / " << ndstid << " ]" << flush;
    }

    assert(solstep2searchnode[new_dst]->find(dst_node) != solstep2searchnode[new_dst]->end());
    assert(solstep2searchnode[src]->find(src_node) != solstep2searchnode[src]->end());
    assert(dst_node);
    #endif

    // Find the determinized operator leading from src to dst
    assert(successor_id_for_dst >= 0);
    assert(successor_id_for_dst < (int)(PR2.general.nondet_mapping[src->op.nondet_index].size()));
    int op_ind = PR2.general.nondet_mapping[src->op.nondet_index][successor_id_for_dst];
    PR2OperatorProxy _op = PR2.proxy->get_operators()[op_ind];
    PR2OperatorProxy * op = &_op;

    assert(op);

    // Compute the new partial state
    PR2State * updated_state = new_dst->state->regress(*op, src_node->full_state);
    updated_state->combine_with(*(src->state));
    assert(src_node->full_state->entails(*updated_state));

    // As a slight optimization, if we only have a single state associated
    //  with the src_node, and it is already strong enough to capture the
    //  regression, then we can stop the pull-back here and re-direct the
    //  successor nodes appropriately.
    if ((1 == solstep2searchnode[src]->size()) && src->state->entails(*updated_state)) {

        if (PR2.logging.psgraph)
            cout << "\nPSGRAPH(" << PR2.logging.id() << "): Base case -- found a solstep stronger than the regression with just a single node." << endl;

        // Re-wire the solsteps
        if (src->has_successor(successor_id_for_dst)) {
            assert(make_connection || (old_dst == src->get_successor(successor_id_for_dst)));
            src->unconnect_from_successor(successor_id_for_dst);
        } else
            assert(make_connection);

        src->connect_to_successor(successor_id_for_dst, new_dst);

        return;
    }

    // We split off just the one node for the path being augmented. A more
    //  advanced technique for the future is to take other applicable nodes
    //  as well that can be used on the strengthened path. However, this
    //  would require ensuring that they are fine for the future, and have
    //  the fixed point regression go all the way back on their paths too.

    // Create the new strengthened solstep
    SolutionStep * new_src = src->copy();
    delete new_src->state;
    new_src->state = updated_state;

    new_steps.push_back(new_src);

    solstep2searchnode[new_src] = new set< PR2SearchNode * >();
    solstep2searchnode[src]->erase(src_node);
    solstep2searchnode[new_src]->insert(src_node);
    src_node->matched_step = new_src;

    // Update the init state if it's shifted to the new source. Note that
    //  we need this here because the base case above (i.e., when no src
    //  parameter is passed in) will not hold in the cases that we've looped
    //  the network back to the init node (i.e., there will be a cycle that
    //  contains the init node, and the entails base case is what triggers).
    if ((src == init) && (0 == solstep2searchnode[src]->size())) {
        if (PR2.logging.psgraph)
            cout << "\nPSGRAPH(" << PR2.logging.id() << "): Updating to a new init node." << endl;
        init = new_src;
    }

    // Fix the connections in the abstract graph
    for (unsigned i = 0; i < src->get_successors().size(); ++i)
        if (src->has_successor(i))
            new_src->connect_to_successor(i, src->get_successor(i));

    assert(make_connection || (old_dst == new_src->get_successor(successor_id_for_dst)));
    if (new_src->has_successor(successor_id_for_dst))
        new_src->unconnect_from_successor(successor_id_for_dst);
    else
        assert(make_connection);
    new_src->connect_to_successor(successor_id_for_dst, new_dst);

    for (auto n : src_node->next_nodes)
        n->parent_step = new_src;

    // Sanity checks
    #ifndef NDEBUG
    int outcome = -1;
    for (auto succss : new_src->get_successors()) {
        outcome += 1;
        if (succss) {
            int op_ind = PR2.general.nondet_mapping[new_src->op.nondet_index][outcome];
            PR2OperatorProxy used_op = PR2.proxy->get_operators()[op_ind];
            assert(new_src->state->entails(*(succss->state->regress(used_op, src_node->full_state))));
        }
    }

    if (PR2.logging.psgraph_condensed)
        cout << " (" << src_node->previous_nodes.size() << ")" << endl;
    #endif

    // Recurse backwards
    for (unsigned i = 0; i < src_node->previous_nodes.size(); ++i) {
        // Don't bother trying the assertions if the step back is poisoned
        if (!src_node->previous_nodes[i]->poisoned) {
            SolutionStep * prev_solstep = NULL;
            assert(src_node->previous_nodes[i]);
            prev_solstep = src_node->previous_nodes[i]->matched_step;
            assert(prev_solstep->get_successor(src_node->previous_node_outcomes[i]) == src);
            assert(src->has_predecessor(prev_solstep));
            fixed_point_regression(prev_solstep, src, new_src,
                                   src_node->previous_nodes[i], src_node,
                                   src_node->previous_node_outcomes[i],
                                   solstep2searchnode, new_steps);
        }
    }
}

void PSGraph::fixed_point_marking(SolutionStep * node) {

    // Only bother doing something if the node exists
    if (!node)
        return;

    // If the solstep is already marked, then we've dealt with it
    if (node && node->is_sc)
        return;

    // We'll only mark this step as strong cyclic if all it's successors are
    for (auto succ : node->get_successors())
        if (!succ || !(succ->is_sc))
            return;

    // Otherwise, this node is all set, and we should mark / recurse
    node->is_sc = true;
    for (auto pred : node->get_predecessors())
        fixed_point_marking(pred);
}

void PSGraph::full_marking() {

    // Identify all of the solsteps that aren't marked strong cyclic
    set< SolutionStep * > unmarked;
    for (auto s : steps)
        if (!(s->is_sc))
            unmarked.insert(s);

    // For any that possibly veer off course, flag them as not strong cyclic
    set< SolutionStep * > not_sc;
    list< SolutionStep * > todo;
    for (auto s : unmarked) {
        for (auto ns : s->get_successors()) {
            if (nullptr == ns) {
                not_sc.insert(s);
                todo.push_back(s);
                break;
            }
        }
    }

    // Recurse backwards marking more potential solution steps as not strong cyclic
    //  Note: This relies on every solstep having some path to the goal
    while (!todo.empty()) {
        SolutionStep * s = todo.front();
        todo.pop_front();
        for (auto ps : s->get_predecessors()) {
            if (not_sc.find(ps) == not_sc.end()) {
                not_sc.insert(ps);
                todo.push_back(ps);
            }
        }
    }

    // Finally, mark all the remaining potential ones as strong cyclic
    for (auto s : unmarked)
        if (not_sc.find(s) == not_sc.end())
            s->is_sc = true;

}

void PSGraph::crawl_steps(SolutionStep * n, bool reversed, set< SolutionStep * > &seen) {
    if (seen.find(n) == seen.end()) {
        seen.insert(n);
        if (reversed) {
            for (auto prev : n->get_predecessors()) {
                if (prev)
                    crawl_steps(prev, reversed, seen);
            }
        } else {
            for (auto next : n->get_successors()) {
                if (next)
                    crawl_steps(next, reversed, seen);
            }
        }
    }
}

void PSGraph::record_snapshot(ofstream &outfile, string indent, bool keyname) {

    if (keyname)
        outfile << "\"psgraph\": ";

    outfile << "{" << endl;

    if (init) {
        outfile << endl << indent << "  \"init\": \"" << init->step_id << "\",";
    } else
        outfile << endl << indent << "  \"init\": false,";

    if (goal) {
        outfile << endl << indent << "  \"goal\": \"" << goal->step_id << "\"";
    } else
        outfile << endl << indent << "  \"goal\": false";

    if (!(init)) {
        outfile << endl << indent << "}";
        return;
    }

    // Print out the nodes
    outfile << "," << endl << indent << "  \"nodes\" : {" << endl;
    bool first = true;
    for (auto n : steps) {
        if (n) {
            if (first)
                first = false;
            else
                outfile << "," << endl;
            n->record_snapshot(outfile, indent+"    ");
        }
    }
    outfile << endl << indent << "  }," << endl;

    // Print out the edges
    outfile << indent << "  \"edges\" : [" << endl;
    first = true;
    for (auto n1 : steps) {
        if (n1) {
            for (auto n2 : n1->get_successors()) {
                if (first)
                    first = false;
                else
                    outfile << "," << endl;
                if (n2)
                    outfile << indent << "      [\"" << n1->step_id << "\", \">\", \"" << n2->step_id << "\"]";
                else
                    outfile << indent << "      [\"" << n1->step_id << "\", \">\", \"-1\"]";
            }
        }
    }
    outfile << endl << indent << "  ]," << endl;

    // Print out the partial states
    outfile << indent << "  \"states\" : {" << endl;
    first = true;
    for (auto n : steps) {
        if (n) {
            if (first)
                first = false;
            else
                outfile << "," << endl;
            n->state->record_snapshot(outfile, indent+"    ");
        }
    }
    outfile << endl << indent << "   }" << endl;

    // Wrap up
    outfile << indent << "}";
}
