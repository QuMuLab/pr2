
#include "simulator.h"

// Used to get us 2-decimal place precision
#include <iomanip>

#include "solution.h"
#include "deadend.h"

Simulator::Simulator(shared_ptr<pr2_search::PR2Search> eng) : engine(eng) {
    current_state = PR2.proxy->generate_new_init();
}

void Simulator::setup_simulation(PR2State * init) {
    if (init)
        current_state = new PR2State(*init);
    else
        current_state = PR2.proxy->generate_new_init();
}

const PR2OperatorProxy Simulator::pick_action(SolutionStep *step, int index) {
    if (-1 == index)
        index = PR2.rng.random(PR2.general.nondet_mapping[step->op.nondet_index].size());
    int op_ind = PR2.general.nondet_mapping[step->op.nondet_index][index];
    return PR2.proxy->get_operators()[op_ind];
}

void Simulator::reset_goal() {
    PR2.proxy->set_goal(PR2.localize.original_goal);
}

// Adjust the goal if we are planning locally
void Simulator::set_local_goal() {
    if (PR2.localize.enabled) {
        PR2.proxy->set_goal(*current_goal);
    }
}

void Simulator::search() {
    // First set the new initial state
    PR2.proxy->set_initial_state(*current_state);

    if (PR2.logging.verbose) {
        cout << "\nPlanning for initial state:" << endl;
        current_state->dump_pddl();
        cout << "\n...and goal state:" << endl;
        current_goal->dump_pddl();
        cout << endl;
    }

    // Finally, solve the problem
    engine->search();
    PR2.weaksearch.num_searches++;
}

bool Simulator::simulate_policy(Solution *sol, PR2State * init) {

    setup_simulation(init);

    PR2State * tmp;

    SolutionStep * step = sol->get_step(*current_state);
    last_run_count = 0;

    while (step && (last_run_count < PR2.simulator.trial_depth)) {

        last_run_count++;

        if (step->is_goal) {
            delete current_state;
            return true;
        }

        tmp = current_state->progress(pick_action(step));
        delete current_state;
        current_state = tmp;
        step = sol->get_step(*current_state);
    }

    last_run_hit_depth = (last_run_count >= PR2.simulator.trial_depth);

    delete current_state;
    return false;
}

bool Simulator::simulate_graph(Solution *sol, PR2State * init) {

    setup_simulation(init);

    SolutionStep * step = sol->get_step(*current_state);
    delete current_state;
    last_run_count = 0;

    while (step && (last_run_count < PR2.simulator.trial_depth)) {
        last_run_count++;
        if (step->is_goal)
            return true;
        step = step->get_successor(PR2.rng.random(step->get_successors().size()));
    }

    last_run_hit_depth = (last_run_count >= PR2.simulator.trial_depth);

    return false;
}

bool Simulator::simulate_solution(Solution *sol, PR2State * init) {
    setup_simulation(init);

    PR2State * tmp;

    SolutionStep * step = sol->get_step(*current_state);
    last_run_count = 0;

    while (step && (last_run_count < PR2.simulator.trial_depth)) {

        last_run_count++;

        if (step->is_goal) {
            delete current_state;
            return true;
        }

        int choice = PR2.rng.random(step->get_successors().size());
        tmp = current_state->progress(pick_action(step, choice));
        delete current_state;
        current_state = tmp;

        step = step->get_successor(choice);
        if (!step)
            step = sol->get_step(*current_state);
        assert((!step) || (current_state->entails(*(step->state))));
    }

    last_run_hit_depth = (last_run_count >= PR2.simulator.trial_depth);

    return false;
}

void Simulator::run_trials() {

    int succ=0, fail=0, depth=0;
    double succ_avg_depth=0.0, fail_avg_depth=0.0;

    for (int i = 0; i < PR2.simulator.num_trials; i++) {
        if (simulate_solution(PR2.solution.incumbent)) {
            succ++;
            succ_avg_depth += double(last_run_count) / double(PR2.simulator.num_trials);
        } else {
            if (last_run_hit_depth)
                depth++;
            else {
                fail++;
                fail_avg_depth += double(last_run_count) / double(PR2.simulator.num_trials);
            }
        }
    }

    cout << setprecision(2) << fixed;
    cout << endl;
    cout << "\t\t--------------------------------------" << endl;
    cout << "\t\t      { Simulation Statistics }" << endl;
    cout << "\t\t--------------------------------------\n" << endl;
    cout << "                          Trials: " << PR2.simulator.num_trials << endl;
    cout << "                           Depth: " << PR2.simulator.trial_depth << endl;
    cout << "                         Success: " << succ << "\t (" << (100.0 * double(succ) / double(succ+depth+fail)) << " %)" << endl;
    cout << "                        Failures: " << (fail+depth) << "\t (" << (100.0 * double(depth+fail) / double(succ+depth+fail)) << " %)" << endl;
    cout << endl;
    cout << " Failures due to unhandled state: " << fail << "\t (" << (100.0 * double(fail) / double(depth+fail)) << " %)" << endl;
    cout << "     Failures due to depth limit: " << depth << "\t (" << (100.0 * double(depth) / double(depth+fail)) << " %)" << endl;
    cout << endl;
    cout << "      Mean successful run length: " << succ_avg_depth << endl;
    cout << "  Mean (state-)failed run length: " << fail_avg_depth << endl;
    cout << endl;
    cout << "-------------------------------------------------------------------\n" << endl;
}

bool Simulator::check_1safe() {

    if (!PR2.deadend.enabled)
        return true;

    // We need to reset in order to get reliable deadend detection
    reset_goal();

    vector<DeadendTuple *> new_deadends;
    PR2State* old_s = new PR2State(*current_state);
    PR2State* new_s;

    // If we do the full 1safe check, then we go through the entire plan. If not,
    //  then we should at least check the first action. In particular, we don't
    //  want the first action in the plan to end up being forbidden.
    unsigned safe_checks = 1;
    if (PR2.deadend.force_1safe_weak_plans)
        safe_checks = engine->get_plan().size();

    for (unsigned i = 0; i < safe_checks; i++) {
        const PR2OperatorProxy op = PR2.proxy->get_operators()[engine->get_plan()[i]];
        vector<NondetSuccessor *> successors;
        new_s = generate_nondet_successors(old_s, &op, successors);

        for (auto succ : successors) {
            if (is_deadend(*(succ->state))) {
                PR2State * new_dead_state = new PR2State(*(succ->state));
                int op_ind = PR2.general.nondet_mapping[op.nondet_index][succ->id];
                const PR2OperatorProxy bad_op = PR2.proxy->get_operators()[op_ind];
                if (PR2.deadend.generalize)
                    generalize_deadend(*new_dead_state);
                new_deadends.push_back(new DeadendTuple(new_dead_state, new PR2State(*old_s), &bad_op));
            }
        }

        for (auto succ : successors)
            if (succ->state != new_s)
                delete succ;

        delete old_s;
        old_s = new_s;
    }

    if (new_deadends.size() > 0) {
        if (PR2.logging.deadends)
            cout << "Found " << new_deadends.size() << " new deadends during 1-safe checking!" << endl;
        update_deadends(new_deadends);
        return false;
    }

    // As a sanity check, make sure that we aren't forbidding the first action
    //  in the plan.
    assert(!is_forbidden(*current_state, PR2.proxy->get_operators()[engine->get_plan()[0]]));

    return true;
}

SolutionStep* Simulator::record_plan() {

    if (PR2.logging.simulator)
        cout << "SIMULATOR(" << PR2.logging.id() << "): Recording the found plan." << endl;

    // Reset the global goal
    reset_goal();

    // Incorporate the new plan, and return the first SolutionStep constructed
    return PR2.solution.incumbent->incorporate_plan(engine->get_plan(),
                                                    current_state,
                                                    PR2.general.matched_step);
}

SolutionStep* Simulator::replan() {

    // If the policy is complete, searching further won't help us
    if (PR2.solution.incumbent->is_strong_cyclic()) {
        cout << "Error: Trying to replan with a strong cyclic incumbent." << endl;
        exit(0);
    }

    if (!current_state) {
        cout << "Error: No current state for the replan." << endl;
        exit(0);
    }

    // If we are detecting deadends, and know this is one, don't even try
    if (PR2.deadend.enabled)
        if (PR2.deadend.states->check_entailed_match(*current_state))
            return nullptr;

    // If we can detect that this is a deadend for the original goal, forget about it
    if (is_deadend(*current_state))
        return nullptr;

    // Will hold later only if no plan works, and we want to plan locally
    bool try_again = PR2.localize.enabled;
    if (try_again && PR2.localize.limited) {
        PR2.weaksearch.limit_states = true;
        PR2.weaksearch.max_states = PR2.localize.max_states;
    }

    if (PR2.logging.simulator)
        cout << "SIMULATOR(" << PR2.logging.id() << "): Trying to plan initially" << endl;

    set_local_goal();
    search();

    while (engine->found_solution() && !check_1safe()) {
        // We need to reset the local goal since the check_1safe resets it
        //  to the original for proper deadend detection
        set_local_goal();
        search();
    }

    PR2.weaksearch.limit_states = false;

    if (engine->found_solution())
        return record_plan();

    reset_goal(); // Note that if record_plan() was called, then so was reset_goal()

    if (try_again) {

        if (PR2.logging.simulator)
            cout << "SIMULATOR(" << PR2.logging.id() << "): Trying to plan again" << endl;

        search();

        while (engine->found_solution() && !check_1safe())
            search();

        if (engine->found_solution())
            return record_plan();
    }

    return nullptr;
}
