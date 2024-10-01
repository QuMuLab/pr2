
#include "regression.h"

#include "pr2.h"

bool RegressableOperator::check_relevance(const PR2State &ps) {
    for (auto eff : op.get_effects())
        if (!ps.is_undefined(eff.get_fact().get_variable().get_id()))
            return true;
    return false;
}

void RegressableOperator::dump() const {
    cout << "Regressable operator:" << endl;
    cout << " -{ Operator }-" << endl;
    op.dump();
    cout << " -{ State }-" << endl;
    state->dump_pddl();
    cout << "" << endl;
}

string RegressableOperator::get_name() {
    return op.get_name();
}

void generate_regressable_ops() {

    list<PolicyItem *> reg_steps;
    list<PolicyItem *> cond_reg_steps;

    PR2State *s;
    for (const auto & op : PR2.proxy->get_operators()) {
        if (0 == PR2.general.conditional_mask[op.nondet_index]->size()) {
            s = new PR2State();

            // Only applicable if the effects currently hold.
            for (auto eff : op.get_effects()) {
                (*s)[eff.get_fact().get_variable().get_id()] = eff.get_fact().get_value();
            }

            reg_steps.push_back(new RegressableOperator(op, s));
        }
        // Next, consider operators that have conditional effects that
        //  are consistent -- i.e., they can all fire simultaneously.
        else {
            s = new PR2State();
            bool consistent = true;

            // Ensure that the preconditions and conditional effect
            //  conditions are all consistent to fire.
            for (auto pre : op.get_preconditions()) {
                int pre_var = pre.get_variable().get_id();
                bool var_is_defined = !(s->is_undefined(pre_var));
                bool var_is_consistent = (pre.get_value() == (*s)[pre_var]);
                if (var_is_defined && !var_is_consistent) {
                    consistent = false;
                    break;
                } else {
                    (*s)[pre_var] = pre.get_value();
                }
            }

            // Only makes sense to continue if it is consistent so far
            if (consistent) {
                for (auto eff : op.get_effects()) {
                    for (auto cond : eff.get_conditions()) {


                        // If the variable is defined, then it must be consistent
                        //  with the conditional effect condition.
                        int cond_var = cond.get_variable().get_id();
                        bool var_is_defined = !(s->is_undefined(cond_var));
                        bool var_is_consistent = (cond.get_value() == (*s)[cond_var]);

                        if (var_is_defined && !var_is_consistent) {
                            consistent = false;
                            break;
                        } else {
                            (*s)[cond_var] = cond.get_value();
                        }
                    }
                    if (!consistent)
                        break;
                }
            }

            // Reset the state for checking the post conditions
            delete s;
            s = new PR2State();

            // Only makes sense to continue if it is consistent so far
            if (consistent) {
                // Only applicable if the post conditions currently hold.
                for (auto eff : op.get_effects()) {
                    int eff_var = eff.get_fact().get_variable().get_id();
                    bool var_is_defined = !(s->is_undefined(eff_var));
                    bool var_is_consistent = (eff.get_fact().get_value() == (*s)[eff_var]);
                    if (var_is_defined && !var_is_consistent) {
                        consistent = false;
                        break;
                    } else {
                        (*s)[eff_var] = eff.get_fact().get_value();
                    }
                }
            }

            if (consistent)
                cond_reg_steps.push_back(new RegressableOperator(op, s));
            else
                delete s;
        }
    }

    PR2.general.regressable_ops = new Policy<PolicyItem>();
    PR2.general.regressable_ops->update_policy(reg_steps);
    PR2.general.regressable_cond_ops = new Policy<PolicyItem>();
    PR2.general.regressable_cond_ops->update_policy(cond_reg_steps);

}
