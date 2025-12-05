
#include "deadend.h"
#include "regression.h"


FSAP::FSAP(PR2State *s, PR2OperatorProxy *o) : PolicyItem(s), op(o) {}


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
        return op->nondet_index < other.op->nondet_index;
}

string FSAP::get_name() {
    return op->get_nondet_name();
}

int FSAP::get_index() {
    return op->get_id();
}

int FSAP::get_nondet_index() {
    return PR2.general.nondet_name_to_index[op->get_nondet_name()];
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

    list<FSAP *> fsaps;
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
        // for (auto item : reg_items) {

        //     RegressableOperator *ro = (RegressableOperator*)item;
        //     PR2OperatorProxy *ro_op_ptr = new PR2OperatorProxy(ro->op);
        //     fsaps.push_back(new FSAP(failed_state->regress(ro->op, dummy_state),
        //                              ro_op_ptr));

        // }

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
            PR2OperatorProxy *prev_op_ptr = new PR2OperatorProxy(*prev_op);
            fsaps.push_back(new FSAP(
                failed_state->regress(*prev_op, failed_state_prev),
                prev_op_ptr));
        }
    }

    delete dummy_state;

    if (PR2.logging.deadends) {
        cout << "DEADENDS(" << PR2.logging.id() << "): Adding the following new FSAPS:" << endl;
        for (auto fsap : fsaps)
            fsap->dump();
    }

    // Add a pointer from the operator to the newly created fsaps
    for (auto fsap : fsaps) {
        PR2.deadend.nondetop2fsaps[fsap->get_nondet_index()]->push_back(fsap);
    }

    PR2.deadend.policy->update_policy(fsaps);
    PR2.deadend.states->update_policy(deadends);
}


