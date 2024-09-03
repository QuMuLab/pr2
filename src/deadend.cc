
#include "deadend.h"
#include "regression.h"


FSAP::FSAP(PR2State *s, PR2OperatorProxy o) : PolicyItem(s), op(&o) {}


void FSAP::dump() const {
    cout << "FSAP:" << endl;
    cout << "Operator: " << op->get_nondet_name() << endl;
    cout << " -{ State }-" << endl;
    state->dump_pddl();
    cout << "" << endl;
}

void Deadend::dump() const {
    cout << "Deadend:" << endl;
    cout << " -{ State }-" << endl;
    state->dump_pddl();
    cout << "" << endl;
}

bool FSAP::operator< (const FSAP& other) const {
    if (is_active != other.is_active)
        return is_active;
    else
        return op->nondet_index > other.op->nondet_index;
}

string FSAP::get_name() {
    return op->get_nondet_name();
}

int FSAP::get_index() {
    return op->get_id();
}


bool is_deadend(PR2State &state) {
    PR2.deadend.reachability_heuristic->reset();
    return (-1 == PR2.deadend.reachability_heuristic->compute_add_and_ff(state));
}

bool is_forbidden(PR2State &state, const PR2OperatorProxy op) {
    vector<OperatorID> ops;
    PR2.generate_fsap_aware_applicable_ops(state, ops);
    return op.is_possibly_applicable(state) && (find(ops.begin(), ops.end(), OperatorID(op.get_id())) == ops.end());
}


bool generalize_deadend(PR2State &state) {

    // If the whole state isn't recognized as a deadend, then don't bother
    //  looking for a subset of the state
    if (!is_deadend(state))
        return false;

    // We go through each variable and unset it, checking if the relaxed
    //  reachability is violated.
    for (unsigned i = 0; i < PR2.general.num_vars; i++) {

        int val = state[i];
        state[i] = -1;

        // If relaxing variable i causes us to reach the goal, keep it
        if (!is_deadend(state))
            state[i] = val;
    }

    if (PR2.logging.deadends) {
        cout << "Found relaxed deadend:" << endl;
        state.dump_pddl();
    }

    return true;
}

void update_deadends(vector< DeadendTuple* > &failed_states) {

    list<PolicyItem *> fsaps;
    list<PolicyItem *> deadends;

    PR2State *dummy_state = new PR2State();

    for (auto fs : failed_states) {

        // Generalize the deadend if need be
        PR2State * failed_state = fs->de_state;
        PR2State * failed_state_prev = fs->prev_state;
        const PR2OperatorProxy * prev_op = fs->prev_op;

        // Add the failed state to our list of deadends (no op_index means
        //  that we are just using this FSAP as a deadend).
        deadends.push_back(new Deadend(new PR2State(*failed_state)));

        // HAZ: Only do the forbidden state-action computation when
        //  the non-deterministic action doesn't have any associated
        //  conditional effects. This is ensured by the construction
        //  of the g_regressable_ops data structure.

        // Get the regressable operators for the given state.
        vector<PolicyItem *> reg_items;
        PR2.general.regressable_ops->generate_consistent_items(*failed_state,
                                                               reg_items,
                                                               PR2.deadend.regress_trigger_only);

        // For each operator, create a new deadend avoidance pair
        for (auto item : reg_items) {

            RegressableOperator *ro = (RegressableOperator*)item;

            fsaps.push_back(new FSAP(failed_state->regress(ro->op, dummy_state),
                                     ro->op));

        }

        ////////////////////////////////////////////

        // // Check to see if we have any consistent "all-fire" operators
        // reg_items.clear();
        // PR2.general.regressable_cond_ops->generate_consistent_items(*failed_state,
        //                                                             reg_items,
        //                                                             PR2.deadend.regress_trigger_only);

        // // For each operator, create a new deadend avoidance pair
        // for (auto item : reg_items) {

        //     RegressableOperator *ro = (RegressableOperator*)item;

        //     fsaps.push_back(new FSAP(failed_state->regress(ro->op, ro->op.all_fire_context),
        //                              ro->op));

        // }

        ////////////////////////////////////////////

        // If we have a specified previous state and action, use that to
        //  build a forbidden state-action pair
        if (NULL != failed_state_prev) {
            fsaps.push_back(new FSAP(
                    failed_state->regress(*prev_op, failed_state_prev),
                    *prev_op));
        }
    }

    delete dummy_state;

    if (PR2.logging.deadends) {
        cout << "DEADENDS(" << PR2.logging.id() << "): Adding the following new FSAPS:" << endl;
        for (auto fsap : fsaps)
            fsap->dump();
    }

    // Add a pointer from the operator to the newly created fsaps
    for (auto fsap : fsaps)
        PR2.deadend.nondetop2fsaps[((FSAP*)fsap)->get_index()]->push_back((FSAP*)fsap);

    PR2.deadend.policy->update_policy(fsaps);
    PR2.deadend.states->update_policy(deadends);
}



// void DeadendAwareSuccessorGenerator::generate_applicable_ops(const PR2State &_curr, vector<OperatorID> &ops) const {
//     if (PR2.deadend.enabled && PR2.deadend.policy) {

//         PR2State curr = PR2State(_curr);

//         vector<PolicyItem *> reg_items;
//         vector<OperatorID> orig_ops;
//         map<int, PolicyItem *> fsap_map;

//         PR2.generate_orig_applicable_ops(_curr, orig_ops);
//         PR2.deadend.policy->generate_entailed_items(curr, reg_items);

//         set<int> forbidden;
//         for (auto item : reg_items) {

//             int index = ((FSAP*)item)->get_index();

//             forbidden.insert(index);

//             if ((fsap_map.find(index) == fsap_map.end()) ||
//                 (item->state->size() < fsap_map[index]->state->size()))
//                     fsap_map[index] = item;
//         }

//         vector<int> ruled_out;
//         for (auto opid : orig_ops) {
//             if (0 == forbidden.count(PR2.proxy->get_nondet_index(opid)))
//                 ops.push_back(opid);
//             else if (PR2.deadend.combine)
//                 ruled_out.push_back(PR2.proxy->get_nondet_index(opid));
//         }

//         // Add this state as a deadend if we have ruled out everything
//         if (!PR2.weaksearch.limit_states && PR2.deadend.record_online &&
//              PR2.deadend.combine && (orig_ops.size() > 0) && ops.empty()) {

//             // Combind all of the FSAPs
//             PR2State *newDE = new PR2State();
//             for (unsigned i = 0; i < ruled_out.size(); i++) {
//                 newDE->combine_with(*(((FSAP*)(fsap_map[ruled_out[i]]))->state));
//             }

//             // Also rule out all of the unapplicable actions
//             for (const auto & op : PR2.proxy->get_operators()) {
//                 if (0 == forbidden.count(op.nondet_index)) {
//                     if (op.is_possibly_applicable(*newDE)) {
//                         assert (!(op.is_possibly_applicable(curr)));
//                         int conflict_var = op.compute_conflict_var(curr);
//                         assert (conflict_var != -1);
//                         assert ((*newDE)[conflict_var] == -1);
//                         (*newDE)[conflict_var] = curr[conflict_var];
//                     }
//                 }
//             }

//             PR2.deadend.combination_count++;

//             vector<DeadendTuple *> failed_states;
//             failed_states.push_back(new DeadendTuple(newDE, NULL, NULL));
//             update_deadends(failed_states);
//         }

//     } else {

//         PR2.generate_orig_applicable_ops(_curr, ops);

//     }

//     return;
// }

