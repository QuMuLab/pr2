
#include "pr2.h"

#include "fond_search.h"
#include "partial_state_graph.h"
#include "policy.h"
#include "regression.h"
#include "simulator.h"
#include "solution.h"

#include "fd_integration/fsap_penalized_ff_heuristic.h"
#include "fd_integration/pr2_search_algorithm.h"

bool PR2Wrapper::run_pr2() {

    PR2.time.start();

    // Create the nondet mapping required
    PR2.generate_nondet_operator_mappings();

    //Manually construct engine
    plugins::Options opts = plugins::Options();
    opts.set("cost_type", OperatorCost::NORMAL);
    //Is bound used at all?
    opts.set("bound", 999999);
    opts.set("max_time", PR2.time.limit);
    opts.set("description", "PR2_Search");
    opts.set("verbosity", utils::Verbosity::NORMAL);
    
    PR2.pr2_engine = make_shared<pr2_search::PR2Search>(opts);

    // // Cast SearchAlgorithm shared_ptr to PR2Search shared_ptr
    // pr2_engine = dynamic_pointer_cast<pr2_search::PR2Search>(engine);

    /**********************************
     * Initialize the data structures *
     **********************************/
    PR2.general.num_vars = PR2.proxy->get_variables().size();

    PR2.general.goal_op = PR2.proxy->get_goal_operator();


    for (auto x : PR2.proxy->get_goals()) {
        PR2.localize.original_goal.push_back(pair<int, int>(x.get_variable().get_id(), x.get_value()));
    }

    if (PR2.deadend.enabled)
        generate_regressable_ops();

    // We create the policies even if we aren't using deadends, as
    //  they may be consulted by certain parts of the code.
    PR2.deadend.policy = new Policy<FSAP>();
    PR2.deadend.states = new Policy<PolicyItem>();
    PR2.deadend.online_policy = new Policy<PolicyItem>();

    // We also create a deadend heuristic computer
    PR2.deadend.reachability_heuristic = PR2.proxy->new_deadend_heuristic();

    /**********************
     * Handle Time Limits *
     **********************/

    cout << "\nTotal allotted time (s): " << PR2.time.limit << endl;

    // If we are going to do a final FSAP-free round, then we modify the
    //  time limits to give a 50/50 split between the main phase and final
    //  round phase
    double time_ratio = 0.5;
    if (PR2.general.final_fsap_free_round)
        PR2.time.limit *= time_ratio;

    cout << "Max time for core phase (remaining used in final-round repairs): " << PR2.time.limit << endl;

    // Adjust the g_time_limit so the epochs are handled properly
    int epochs_remaining = PR2.epoch.number;
    double single_time_limit = PR2.time.limit / (double)PR2.epoch.number;
    PR2.time.limit = single_time_limit;

    cout << "Max time for each of the " << epochs_remaining << " epochs: " << PR2.time.limit << endl << endl;




    /***************************************************
     * Initialize the remaining data structures needed *
     ***************************************************/

    cout << "\n\nCreating the simulator..." << endl;
    Simulator *sim = new Simulator(pr2_engine);

    cout << "\n\nGenerating an incumbent solution..." << endl;
    PR2.solution.incumbent = new Solution(sim);
    PR2.solution.best = PR2.solution.incumbent;

    /********************************
     * Do the main computation loop *
     ********************************/

    cout << "\n\nBeginning search for strong cyclic solution..." << endl;

    while (find_better_solution(sim)) {
        if (PR2.logging.verbose)
            cout << "Finished repair round." << endl;

        if (!PR2.time.time_left()) {
            epochs_remaining--;
            if (epochs_remaining > 0)
                PR2.time.limit += single_time_limit;
        }
    }

    cout << "Done repairing..." << endl;

    // Use the best policy found so far
    if (PR2.solution.incumbent && PR2.solution.best &&
        (PR2.solution.best != PR2.solution.incumbent) &&
        PR2.solution.best->better_than(PR2.solution.incumbent))
            PR2.solution.incumbent = PR2.solution.best;




    /********************************
     * Do the final FSAP free round *
     ********************************/

    if (PR2.general.final_fsap_free_round)
        PR2.time.limit /= time_ratio;

    if (PR2.general.final_fsap_free_round &&
        !(PR2.solution.incumbent->is_strong_cyclic())) {

        bool os1 = PR2.deadend.enabled;
        bool os2 = PR2.deadend.generalize;
        bool os3 = PR2.deadend.record_online;
        bool os4 = PR2.deadend.force_1safe_weak_plans;
        bool os5 = PR2.deadend.poison_search;
        bool os6 = PR2.weaksearch.limit_states;
        int  os7 = PR2.weaksearch.max_states;

        PR2.deadend.enabled = false;
        PR2.deadend.generalize = false;
        PR2.deadend.record_online = false;
        PR2.deadend.force_1safe_weak_plans = false;
        PR2.deadend.poison_search = false;
        PR2.weaksearch.limit_states = true;
        PR2.weaksearch.max_states = 1000;

        cout << "\n\nDoing one final best-effort round ignoring FSAPs for unhandled states." << endl;
        find_better_solution(sim);

        PR2.deadend.enabled = os1;
        PR2.deadend.generalize = os2;
        PR2.deadend.record_online = os3;
        PR2.deadend.force_1safe_weak_plans = os4;
        PR2.deadend.poison_search = os5;
        PR2.weaksearch.limit_states = os6;
        PR2.weaksearch.max_states = os7;
    }


    /********************************
     * Optimize things if necessary *
     ********************************/
    if (PR2.general.optimize_final_solution) {
        PR2.solution.incumbent->rebuild();
        PR2.deadend.policy->rebuild();
    }




    /************************************
     * Print out the general statistics *
     ************************************/
    cout << "\n\n-------------------------------------------------------------------\n" << endl;
    cout << "\n\t\t-----------------------------------" << endl;
    cout << "\t\t      { General Statistics }" << endl;
    cout << "\t\t-----------------------------------\n" << endl;
    cout << "                         Time taken: " << PR2.time.time_taken() << " sec" << endl;
    cout << "                           # Rounds: " << PR2.logging.fond_search_count << endl;
    cout << "                    # Weak Searches: " << PR2.weaksearch.num_searches << endl;
    cout << "                      Solution Size: " << PR2.solution.incumbent->get_size() << endl;
    cout << "                          FSAP Size: " << PR2.deadend.policy->size() << endl;
    if (PR2.deadend.combine)
        cout << "                  Combination Count: " << PR2.deadend.combination_count << endl;
    if (PR2.deadend.poison_search)
        cout << "                       Poison Count: " << PR2.deadend.poison_count << endl;
    cout << "\n-------------------------------------------------------------------\n" << endl;




    /**********************
     * Run the simulation *
     **********************/

    // Disable the deadend settings for the online simulation(s)
    PR2.deadend.enabled = false;
    PR2.deadend.generalize = false;
    PR2.deadend.record_online = false;

    cout << "\nRunning the simulation..." << endl;
    sim->run_trials();




    /*******************************
     * Dump the required log files *
     *******************************/
    if (PR2.output.format == PR2.output.MATCHTREE) {

        cout << "Dumping the policy and fsaps..." << endl;
        ofstream outfile;

        outfile.open("policy.out", ios::out);
        PR2.solution.incumbent->policy->generate_cpp_input(outfile);
        outfile.close();

        outfile.open("policy.fsap", ios::out);
        PR2.deadend.policy->generate_cpp_input(outfile);
        outfile.close();

    } else if (PR2.output.format == PR2.output.LIST) {

        cout << "Dumping the policy and fsaps..." << endl;
        PR2.solution.incumbent->policy->write_policy("policy.out");
        PR2.deadend.policy->write_policy("policy.fsap", true);

    } else if (PR2.output.format == PR2.output.CONTROLLER) {

        cout << "Dumping the final psgraph as json..." << endl;

        ofstream outfile;
        outfile.open("policy.out", ios::out);
        PR2.solution.incumbent->network->record_snapshot(outfile, "", false);

    }


    cout << endl;

    return PR2.solution.best->is_strong_cyclic();
}

void PR2Wrapper::generate_orig_applicable_ops(const PR2State &curr, vector<OperatorID> &ops) {
    pr2_engine->get_orig_successor_generator()->generate_applicable_ops(curr, ops);
}

void PR2Wrapper::generate_fsap_aware_applicable_ops(const PR2State &curr, vector<OperatorID> &ops) {
    // pr2_engine->get_deadend_aware_successor_generator()->generate_applicable_ops(curr, ops);
    // struct DeadendAwareSuccessorGenerator {
    //     void generate_applicable_ops(const PR2State &curr, vector<OperatorID> &ops) const;
    // };
    DeadendAwareSuccessorGenerator().generate_applicable_ops(curr, ops);
}

PR2Wrapper PR2; // Holds all of the settings and data for PR2

void PR2Wrapper::generate_nondet_operator_mappings() {
    // Map from operator id to nondet index
    vector<int>* nondet_index_map = new vector<int>(PR2.proxy->get_operators().size());

    // assert that the mappings are empty
    assert(PR2.general.nondet_mapping.empty());
    assert(PR2.general.nondet_outcome_mapping.empty());

    // temporary mapping from non-det name to index
    map<string, int> nondet_name_to_index;

    int current_nondet_index = 0;
    
    for (auto op : PR2.proxy->get_operators()) {
        //If not in the mapping yet
        PR2.general.conditional_mask.push_back(new vector<int>());
        PR2.deadend.nondetop2fsaps.push_back(new vector< FSAP* >());

        if (nondet_name_to_index.find(op.get_nondet_name()) == nondet_name_to_index.end()) {
            nondet_name_to_index[op.get_nondet_name()] = current_nondet_index;
            PR2.general.nondet_mapping.push_back(vector<int>());
            current_nondet_index++;



            PR2.general.conditional_mask.push_back(new vector<int>());
            PR2.deadend.nondetop2fsaps.push_back(new vector< FSAP* >());
        }
        PR2.general.nondet_mapping[nondet_name_to_index[op.get_nondet_name()]].push_back(op.get_id());
        PR2.general.nondet_outcome_mapping[op.get_id()] =
            PR2.general.nondet_mapping[nondet_name_to_index[op.get_nondet_name()]].size() - 1;
        op.nondet_index = nondet_name_to_index[op.get_nondet_name()];
        op.nondet_outcome = PR2.general.nondet_outcome_mapping[op.get_id()];
        (*nondet_index_map)[op.get_id()] = op.nondet_index;

        for (auto eff : op.get_all_effects()) {
            for (auto cond : eff.get_conditions()) {
                vector<int> *var_list = PR2.general.conditional_mask[op.nondet_index];
                if (find(var_list->begin(), var_list->end(), cond.get_variable().get_id()) == var_list->end())
                    PR2.general.conditional_mask[op.nondet_index]->push_back(cond.get_variable().get_id());
            }
        }
    }

    PR2.proxy->set_nondet_index_map(*nondet_index_map);
}

void PR2OperatorProxy::update_nondet_info() {
    nondet_index = PR2.proxy->get_nondet_index(_index);
    nondet_outcome = PR2.general.nondet_outcome_mapping[_index];
}

bool PR2Wrapper::pr2_goal_check(TaskProxy task, State state) {
    if (PR2.solution.incumbent){
        SolutionStep * best_step = PR2.solution.incumbent->get_step(state);

        if ((best_step && PR2.weaksearch.stop_on_policy) || (best_step && best_step->is_goal)) {
        #ifndef NDEBUG
                if (!best_step->is_goal) {
                    PR2State *cur = new PR2State(state);
                    assert(!is_forbidden(*cur, best_step->op));
                    delete cur;
                }
        #endif
            PR2.general.matched_step = best_step;
            return true;
        } else {
            return false;
        }
    }

    for (auto varval : PR2.general.goal_op->get_preconditions()) {

        if (state[varval.get_variable().get_id()].get_value() != varval.get_value()) {
            return false;
        }
    }

    PR2State *gs = new PR2State();
    for (auto varval : PR2.general.goal_op->get_preconditions())
        (*gs)[varval.get_variable().get_id()] = varval.get_value();

    SolutionStep *grs = new SolutionStep(gs, PR2.solution.incumbent->network, 0,
                                         // Null?
                                         PR2OperatorProxy(*PR2.general.goal_op),
                                         -1, true, true, true);

    PR2.general.matched_step = grs;

    return true;
}
