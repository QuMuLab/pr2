#include "fsap_penalized_ff_heuristic.h"

#include "../../plugins/options.h"
#include "../../plugins/plugin.h"

#include "../../task_utils/task_properties.h"

#include "../pr2.h"
#include "../deadend.h"

#include <cassert>
#include <vector>

using namespace std;

namespace fsap_penalized_ff_heuristic {
// construction and destruction
FSAPPenalizedFFHeuristic::FSAPPenalizedFFHeuristic(
    shared_ptr<AbstractTask> task,
    bool cache_estimates,
    const string desc,
    utils::Verbosity verb) : RelaxationHeuristic(tasks::AxiomHandlingType::APPROXIMATE_NEGATIVE, task, cache_estimates, desc, verb),
      did_write_overflow_warning(false),
      relaxed_plan(task_proxy.get_operators().size(), false) {
    if (PR2.logging.verbose)
        cout << "Initializing FSAP aware FF heuristic..." << endl;
}

FSAPPenalizedFFHeuristic::~FSAPPenalizedFFHeuristic() {
}

void FSAPPenalizedFFHeuristic::enqueue_if_necessary(PropID prop_id, int cost, OpID op_id){
    assert(cost >= 0);
    Proposition *prop = get_proposition(prop_id);
    if (prop->cost == -1 || prop->cost > cost) {
        prop->cost = cost;
        prop->reached_by = op_id;
        queue.push(cost, prop_id);
    }
    if (PR2.logging.heuristic) {
        UnaryOperator *op = get_operator(op_id);
        if (op) {
            cout << "Enquing operator " << PR2.proxy->get_operators()[op->operator_no].get_name() << " at cost " << cost << endl;
            cout << "  PRE:";
            for (auto pre : get_preconditions(op->operator_no))
                cout << "  " << pre;
            cout << "\n  EFF:  " << prop << endl;
        } else
            cout << "Enquing true prop " << prop << " at cost " << cost << endl;
    }
    assert(prop->cost != -1 && prop->cost <= cost);
}

void FSAPPenalizedFFHeuristic::write_overflow_warning() {
    if (!did_write_overflow_warning) {
        cout << "WARNING: overflow on h^add! Costs clamped to "
             << MAX_COST_VALUE << endl;
        cerr << "WARNING: overflow on h^add! Costs clamped to "
             << MAX_COST_VALUE << endl;
        did_write_overflow_warning = true;
    }
}

void FSAPPenalizedFFHeuristic::reset(std::vector<FactPair> &goal_facts) {
    // Turn the old goals off
    for (auto prop : goal_propositions)
        get_proposition(prop)->is_goal = false;
    goal_propositions.clear();

    // Change to use goal_facts
    for (auto varval : goal_facts) {
        Proposition * prop = get_proposition(varval.var, varval.value);
        prop->is_goal = true;
        goal_propositions.push_back(get_prop_id(varval.var, varval.value));
    }
}

void FSAPPenalizedFFHeuristic::reset() {
    // Turn the old goals off
    for (auto prop : goal_propositions)
        get_proposition(prop)->is_goal = false;
    goal_propositions.clear();

    // Turn the new goals on
    for (auto varval : PR2.localize.original_goal) {
        Proposition * prop = get_proposition(varval.first, varval.second);
        prop->is_goal = true;
        goal_propositions.push_back(get_prop_id(varval.first, varval.second));
    }
}

// heuristic computation
void FSAPPenalizedFFHeuristic::setup_exploration_queue() {
    queue.clear();
    seen_fsaps.clear();

    for (Proposition &prop : propositions) {
        prop.cost = -1;
        prop.marked = false;
    }

    // Deal with operators and axioms without preconditions.
    for (UnaryOperator &op : unary_operators) {
        op.unsatisfied_preconditions = op.num_preconditions;
        op.cost = op.base_cost; // will be increased by precondition costs

        if (op.unsatisfied_preconditions == 0)
            enqueue_if_necessary(op.effect, op.base_cost, get_op_id(op));
    }
}

void FSAPPenalizedFFHeuristic::setup_exploration_queue_state(const State &state) {
    for (FactProxy fact : state) {
        PropID init_prop = get_prop_id(fact);
        enqueue_if_necessary(init_prop, 0, NO_OP);
    }
}


void FSAPPenalizedFFHeuristic::setup_exploration_queue_state(const PR2State &state) {
    // We need to be a bit more careful about going through the var/val
    //  pairs, as we may have undefined variables (which should take on
    //  every value simultaneously).
    for (VariableProxy var : PR2.proxy->get_variables()) {
        if (state.is_undefined(var)) {
            for (int val = 0; val < var.get_domain_size(); ++val) {
                PropID init_prop = get_prop_id(var.get_id(), val);
                enqueue_if_necessary(init_prop, 0, NO_OP);
            }
        } else {
            PropID init_prop = get_prop_id(var.get_id(), state[var.get_id()]);
            enqueue_if_necessary(init_prop, 0, NO_OP);
        }
    }
}

int FSAPPenalizedFFHeuristic::compute_fsap_penalty(int op_num) {
    // Axioms have a -1 operator number
    if (-1 == op_num)
        return 0;

    // Give the option to bypass this (potentially costly) computation
    if (!PR2.weaksearch.penalize_potential_fsaps)
        return 0;

    // We'll keep the running total of all the FSAPs found, scaled by the penalty amount
    int total = 0;
    for (auto fsap : *(PR2.deadend.nondetop2fsaps[PR2.proxy->get_nondet_index(op_num)])) {

        // If we've already seen it, then we don't need to check the partial state again
        if (0 != seen_fsaps.count(fsap))
            total += PR2.weaksearch.fsap_penalty;
        else {

            // It will only hold if all of the props are already added in the relaxed graph
            bool holds = true;
            for (auto varval : *(fsap->varvals())) {
                if (-1 == get_proposition(varval.first, varval.second)->cost) {
                    holds = false;
                    break;
                }
            }

            // When it holds, we add the new fsap as being enabled, and increase the penalty
            if (holds) {
                seen_fsaps.insert(fsap);
                total += PR2.weaksearch.fsap_penalty;
                if (PR2.logging.heuristic) {
                    cout << "\nFSAP-Heur(" << PR2.logging.id() << "): Penalizing for FSAP (" << fsap << "):" << endl;
                    fsap->dump();
                }
            }
        }
    }
    return total;
}

void FSAPPenalizedFFHeuristic::relaxed_exploration() {
    int unsolved_goals = goal_propositions.size();
    while (!queue.empty()) {
        pair<int, PropID> top_pair = queue.pop();
        int distance = top_pair.first;
        PropID prop_id = top_pair.second;
        Proposition *prop = get_proposition(prop_id);
        int prop_cost = prop->cost;
        assert(prop_cost >= 0);
        assert(prop_cost <= distance);
        if (prop_cost < distance)
            continue;
        if (prop->is_goal && --unsolved_goals == 0)
            return;
        for (OpID op_id : precondition_of_pool.get_slice(
                 prop->precondition_of, prop->num_precondition_occurences)) {
            UnaryOperator *unary_op = get_operator(op_id);
            increase_cost(unary_op->cost, prop_cost);
            --unary_op->unsatisfied_preconditions;
            assert(unary_op->unsatisfied_preconditions >= 0);
            if (unary_op->unsatisfied_preconditions == 0)
                enqueue_if_necessary(unary_op->effect,
                                     unary_op->cost + compute_fsap_penalty(unary_op->operator_no),
                                     op_id);
        }
    }
}

void FSAPPenalizedFFHeuristic::mark_preferred_operators(
    const State &state, PropID goal_id) {
    Proposition *goal = get_proposition(goal_id);
    if (!goal->marked) { // Only consider each subgoal once.
        goal->marked = true;
        OpID op_id = goal->reached_by;
        if (op_id != NO_OP) { // We have not yet chained back to a start node.
            UnaryOperator *unary_op = get_operator(op_id);

            bool is_preferred = true;
            for (PropID precond : get_preconditions(op_id)) {
                mark_preferred_operators(state, precond);
                if (get_proposition(precond)->reached_by != NO_OP) {
                    is_preferred = false;
                }
            }
            int operator_no = unary_op->operator_no;
            if (is_preferred && operator_no != -1) {
                // This is not an axiom.
                OperatorProxy op = task_proxy.get_operators()[operator_no];
                assert(task_properties::is_applicable(op, state));
                // Note that we also will only want to do this if the
                //  action isn't forbidden here. Marking a forbidden
                //  action as preferred can lead to bad search expansion.
                if (0 == forbidden_ops.count(PR2.proxy->get_nondet_index(op))) {
                    set_preferred(op);
                }
                set_preferred(op);
            }
        }
    }
}

void FSAPPenalizedFFHeuristic::mark_preferred_operators_and_relaxed_plan(
    const State &state, PropID goal_id) {
    Proposition *goal = get_proposition(goal_id);
    if (!goal->marked) { // Only consider each subgoal once.
        goal->marked = true;
        OpID op_id = goal->reached_by;
        if (op_id != NO_OP) { // We have not yet chained back to a start node.


            UnaryOperator *unary_op = get_operator(op_id);
            bool is_preferred = true;
            for (PropID precond : get_preconditions(op_id)) {
                mark_preferred_operators_and_relaxed_plan(
                    state, precond);
                if (get_proposition(precond)->reached_by != NO_OP) {
                    is_preferred = false;
                }
            }
            int operator_no = unary_op->operator_no;
            if (operator_no != -1) {
                // This is not an axiom.
                relaxed_plan[operator_no] = true;
                if (is_preferred) {
                    OperatorProxy op = task_proxy.get_operators()[operator_no];
                    assert(task_properties::is_applicable(op, state));
                    // Note that we also will only want to do this if the
                    //  action isn't forbidden here. Marking a forbidden
                    //  action as preferred can lead to bad search expansion.
                    if (0 == forbidden_ops.count(PR2.proxy->get_nondet_index(op))) {
                        set_preferred(op);
                    }
                }
            }
        }
    }
}

int FSAPPenalizedFFHeuristic::compute_add_and_ff(const State &state) {

    if (PR2.logging.heuristic) {
        cout << "\nFSAP-Heur(" << PR2.logging.id() << "): Computing heuristic for the following state:" << endl;
        PR2.proxy->dump_pddl_state(state);
        cout << endl;
    }

    setup_exploration_queue();
    setup_exploration_queue_state(state);
    relaxed_exploration();

    int total_cost = 0;
    for (PropID goal_id : goal_propositions) {
        const Proposition *goal = get_proposition(goal_id);
        int goal_cost = goal->cost;
        if (goal_cost == -1)
            return DEAD_END;
        increase_cost(total_cost, goal_cost);
    }
    
    if (PR2.logging.heuristic)
        cout << "\nFSAP-Heur(" << PR2.logging.id() << "): Heuristic value = " << total_cost << endl;

    return total_cost;
}
int FSAPPenalizedFFHeuristic::compute_add_and_ff(const PR2State &state) {

    setup_exploration_queue();
    setup_exploration_queue_state(state);
    relaxed_exploration();

    int total_cost = 0;
    for (PropID goal_id : goal_propositions) {
        const Proposition *goal = get_proposition(goal_id);
        int goal_cost = goal->cost;
        if (goal_cost == -1)
            return DEAD_END;
        increase_cost(total_cost, goal_cost);
    }
    return total_cost;
}

int FSAPPenalizedFFHeuristic::compute_heuristic(const State &state) {

    if (PR2.deadend.record_online &&
        PR2.deadend.online_policy->check_entailed_match(PR2State(state)))
        return DEAD_END;

    int h = compute_add_and_ff(state);
    if (h != DEAD_END) {

        // Make sure we don't mark an operator as preferred if it's forbidden
        forbidden_ops.clear();
        vector<FSAP *> reg_items;
        PR2State * ps = new PR2State(state);
        PR2.deadend.policy->generate_entailed_items(*ps, reg_items);
        delete ps;
        for (auto item : reg_items)
            forbidden_ops.insert(item->get_index());

        // Collecting the relaxed plan also sets the preferred operators.
        for (size_t i = 0; i < goal_propositions.size(); ++i)
            mark_preferred_operators_and_relaxed_plan(state, goal_propositions[i]);

        int h_ff = 0;
        for (size_t op_no = 0; op_no < relaxed_plan.size(); ++op_no) {
            if (relaxed_plan[op_no]) {
                relaxed_plan[op_no] = false; // Clean up for next computation.
                h_ff += task_proxy.get_operators()[op_no].get_cost();
            }
        }
        return h_ff;

        // for (size_t i = 0; i < goal_propositions.size(); ++i)
        //     mark_preferred_operators(state, goal_propositions[i]);
    } else {
        if (PR2.logging.deadends)
            cout << "\nHeuristic found deadend!" << endl;

        if (PR2.deadend.record_online) {
            PR2.deadend.found_online.push_back(new DeadendTuple(new PR2State(state), NULL, NULL));
            PR2.deadend.online_policy->add_item(new Deadend(new PR2State(state)));
        }
    }
    return h;
}

int FSAPPenalizedFFHeuristic::compute_heuristic(const PR2State &state) {
    return compute_add_and_ff(state);
}

void FSAPPenalizedFFHeuristic::compute_heuristic_for_cegar(const State &state) {
    compute_heuristic(state);
}

// TODO: See if this can be removed.
// class FSAPPenalizedFFHeuristicFeature : public plugins::TypedFeature<Evaluator, FSAPPenalizedFFHeuristic> {
// public:
//     FSAPPenalizedFFHeuristicFeature() : TypedFeature("fsapff") {
//         document_title("FSAP Penalized FF heuristic");

//         Heuristic::add_options_to_feature(*this);

//         document_language_support("action costs", "supported");
//         document_language_support("conditional effects", "supported");
//         document_language_support("axioms", "supported soon");
//         document_property("admissible", "no");
//         document_property("consistent", "no");
//         document_property("safe", "yes for tasks without axioms");
//         document_property("preferred operators", "yes");
//     }
// };

// static plugins::FeaturePlugin<FSAPPenalizedFFHeuristicFeature> _plugin;




}
