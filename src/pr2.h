#ifndef PR2_H
#define PR2_H

#include <map>
#include <queue>
#include <set>
#include <iostream>
#include <sstream>
#include <vector>

#include "fd_integration/pr2_proxies.h"
#include "fd_integration/pr2_search_algorithm.h"
#include "fd_integration/fsap_penalized_ff_heuristic.h"

using namespace std;

struct DeadendTuple;
struct FSAP;
struct PR2SearchNode;
struct PR2SearchStatus;

class PR2State;
class Policy;
class Solution;
class SolutionStep;

class pr2_node_comparison;

class PR2TaskProxy;
class PR2OperatorProxy;
class SearchAlgorithm;

namespace pr2_search {
    class PR2Search;
}

// General struct for PR2 settings and data structures
struct PR2Wrapper {


    // Main call to the PR2 planner's outer loop
    bool run_pr2();

    // Used for our simulator
    utils::RandomNumberGenerator rng;
    // = utils::RandomNumberGenerator(724227515);

    // Meta-level task that houses all the FD details
    PR2TaskProxy *proxy = nullptr;

    PR2Wrapper() : rng(724227515) {}

    // The single search algorithm used by PR2
    shared_ptr<pr2_search::PR2Search> pr2_engine;
    unique_ptr<pr2_search::PR2Search> generate_initial_search_algorithm();

    void generate_orig_applicable_ops(const PR2State &curr, vector<OperatorID> &ops);
    void generate_fsap_aware_applicable_ops(const PR2State &curr, vector<OperatorID> &ops);

    void generate_nondet_operator_mappings();


    /*********************************************
     *
     * General notes about the PR2 config:
     *  - Settings will typically be set once and then left
     *  - Data structures typcially change during the course of PR2
     *
     ******************************************/


    /*************
     * Solutions *
     *************/
    struct SOL {

        // Settings
        int evaluation_trials = 1000; // Number of trials to do when evaluating a policy's score

        // Data structures used globally for the solutions
        Solution *incumbent; // The current solution we are building
        Solution *best; // The best solution we've found so far
        int num_steps_created = 0; // Used to give each step a unique id

    } solution;


    /********
     * Time *
     ********/
    struct TIME {

        // Settings
        double limit = 3600.0; // An upper limit on when we should stop searching

        // Data structures / methods
        utils::Timer main; // The main timer for PR2
        void start() { main.reset(); main.resume(); }
        void stop() { main.stop(); }
        bool time_left() { return main() < limit; }
        double time_taken() { return main(); }

    } time;


    /************
     * Deadends *
     ************/
    struct DE {

        // Settings
        bool enabled = true; // Use deadend detection and FSAPs
        bool generalize = true; // Attempt to generalize any found deadends
        bool record_online = true; // Record the deadends found online
        bool combine = false; // Combine FSAP conditions for a new deadend when there are no applicable actions
        bool regress_trigger_only = false; // If true, the only FSAP for a new deadend should be from the action that lead there
        bool force_1safe_weak_plans = true; // If true, a weak plan is only used if no 1-off reachable state is a deadend
        bool poison_search = true; // If true, deadends will disable certain aspects of the full search tree

        // Data structures
        fsap_penalized_ff_heuristic::FSAPPenalizedFFHeuristic *reachability_heuristic; // A custom heuristic for detecting deadends
        Policy *policy; // Holds all of the FSAPs
        Policy *states; // Holds all of the generalized deadends
        Policy *online_policy; // Temporary store for deadends found online
        vector< DeadendTuple* > found_online; // Stores the deadends that we detect online (along with the necessary context)
        vector< vector< FSAP* > * > nondetop2fsaps; // Maps a nondet operator id to the set of FSAPs that forbid it from occurring
        int combination_count = 0; // Keeps track of how many times we combined FSAPs to produce a new deadend
        int poison_count = 0; // Keeps track of how many search nodes we've poisoned

    } deadend;


    /**********
     * Epochs *
     **********/
    struct EPOCH {

        // Settings
        int number = 1; // The number of epochs we want to perform

        // Data structure to save and restart an epoch if no deadends were found
        PR2SearchStatus * last_search_status;

    } epoch;


    /*****************
     * Weak Planning *
     *****************/
    struct WEAK {

        // Settings
        bool stop_on_policy = true; // Stop planning when the policy matches
        bool penalize_potential_fsaps = true; // If true, the additive heuristic is augmented to disuade using potential FSAPs in the delete relaxation
        int fsap_penalty = 10; // Every fsap that holds will increase the operator cost by this amount when it is enqueued

        // Data structures / configurations that change for the weak search
        bool limit_states = false; // Forces the search to stop based on the max_states setting
        int max_states = 100; // The number of states that we should limit the search to
        int num_searches = 0; // Number of times we call for a new plan

    } weaksearch;


    /**********
     * Output *
     **********/
    struct OUT {

        // Settings
        int CONTROLLER = 3; // Basically the psgraph
        int LIST = 2; // Just a big if-then-else list
        int MATCHTREE = 1; // Proper match tree format
        int format = CONTROLLER; // The type of output we want to use by default

        // Data structures

    } output;


    /***************
     * FOND Search *
     ***************/
    struct FONDSEARCH {

        // Settings
        int OPEN_LIST_STACK = 1;
        int OPEN_LIST_QUEUE = 2;
        int OPEN_LIST_NEAR_INIT = 3;
        int OPEN_LIST_AWAY_INIT = 4;
        int OPEN_LIST_RANDOM = 5;
        int node_preference = OPEN_LIST_STACK; // The method that should be used for the priority queue

        // Data structures
        int PR2NodeCount = 0; // Just a count on the number of search nodes created

    } fondsearch;


    /***************************
     * Localized (re-)Planning *
     ***************************/
    struct LOCAL {

        // Settings
        bool enabled = true; // Use the planning locally technique
        bool generalize = true; // Plan for the generalized state instead of a complete one
        bool limited = true; // Restrict the localized planning to a small number of states
        int max_states = 100; // The number of states to restrict ourselves to if limited is true

        // Data structures
        vector<pair<int, int>> original_goal; // The original goal that we can use for resetting the search

    } localize;


    /***********
     * PSGraph *
     ***********/
    struct PSGRAPH {

        // Settings
        bool full_scd_marking = true; // Do sound & complete SCD marking of the solution graph (may be expensive and unnecessary)
        bool clear_dead_solsteps = false; // After the fixed-point-regression, try to clear up any unused parts of the psgraph

    } psgraph;


    /*************
     * Simulator *
     *************/
    struct SIM {

        // Settings
        int trial_depth = 1000; // Used to limit the number of simulation steps
        int num_trials = 100; // Number of trials that should be used for the simulation

    } simulator;


    /***********
     * Logging *
     ***********/
    struct LOG {

        // Trick to sort through massive debug logs. The runs should be
        //  deterministic, so unless the search procedure changes, you
        //  can locate the same spot again by searching for the same
        //  <num> location.
        int _ID = 0;
        string id() {return "<" + to_string(++_ID) + ">";} // Used to find a specific place in output logs
        bool check_id(int id) {return id == _ID;} // Used to target output at a specific time/place

        // Targetted debug logs -- flag which solsteps and searchnode id's
        //  should be "watched", and anything relevant happening to them
        //  will be logged.
        //
        set<int> solstep_watch_list = // Set of SolutionStep objects we want to generate logging for
                    {};
        set<int> searchnode_watch_list = // Set of SearchNode objects we want to generate logging for
                    {};
        //
        bool log_solstep(int id) {return solstep_watch_list.find(id) != solstep_watch_list.end();}
        bool log_searchnode(int id) {return searchnode_watch_list.find(id) != searchnode_watch_list.end();}

        // Settings
        bool verbose = false; // If true, print a bunch of debug info (can be set via command-line)
        bool dump_snapshots = false; // If true, depending on the other logging, json snapshots will be written to file
        bool validate_network_and_nodes = false; // If true, the solution graph and search nodes will be validated for consistency
        bool disable_state_dump = false; // If true, printing partial states will be disabled (helpful for massive outputs of hard-to-read states)

        // General data structures
        int fond_search_count = 0; // Keeps track of how many FOND search's have taken place.
        int snapshot_num = 0; // Used to identify the snapshot we are taking (will be increased every snapshot written to file)

        // Component-specific debugging
        bool heuristic = false; // If true, details on the heuristic computation are output
        bool weak_search = false; // If true, details about the lazy search are output
        bool fond_search = false; // If true, details about the high-level FOND search are output
        bool fond_search_expanding = false; // If true, details on the expansion during fond search are output
        bool simulator = false; // If true, simulator debug output is logged
        bool psgraph = false; // If true, the PSGraph methods output logging info
        bool psgraph_condensed = false; // If true, condensed SolutionStep path details will be displayed for the fixed-point-regression
        bool deadends = false; // If true, deadend and FSAP methods output logging info
        bool network_assertions = false; // If true, loads of info on the network validation steps will be dumped help diagnose assertion failures
        bool poisoning = false; // If true, details on the node poisoning is printed

    } logging;


    /***********
     * General *
     ***********/
    struct GENERAL {

        // General settings
        bool final_fsap_free_round = true; // Do a final best-effort round
        bool optimize_final_solution = true; // Rebuild the final solution to throw away irrelevant parts

        // General stats
        unsigned int num_vars = 0; // The number of variables in the problem

        // General data structures
        vector< vector<int> > nondet_mapping; // Maps a non-deterministic action id to a list of ground operator ids
        map<int, int> nondet_outcome_mapping; // Maps an action id to the outcome of the non-deterministic action

        vector<vector<int> *> conditional_mask; // Maps a non-deterministic action id to the variables that must be defined when doing context-sensitive regression
        Policy *regressable_ops; // The policy to check what operators are regressable
        Policy *regressable_cond_ops; // The policy to check what operators with conditional effects are regressable
        SolutionStep * matched_step; // Contains the condition that matched when our policy recognized the state

        PR2OperatorProxy * goal_op; // The operator that we use to achieve the goal

    } general;

    /************************************************************
     * Used to seed all of the options in the underlying solver *
     ************************************************************/
    bool check_options(const vector<string> &args) {

        

        //The first element of args is the path
        for (unsigned int i = 1; i < args.size(); i++) {
            /**************************************************************/

            if (args[i].compare("--solution-evaluation-trials") == 0)
                solution.evaluation_trials = stoi(args[++i]);

            /**************************************************************/

            else if (args[i].compare("--time-limit") == 0)
                time.limit = (double)stof(args[++i]);

            /**************************************************************/

            
            else if (args[i].compare("--search") == 0)
                assert(args[++i] == "pr2search()");

            else if (args[i].compare("--internal-plan-file") == 0) {
                ++i;continue;
            }

            /**************************************************************/ 

            else if (args[i].compare("--deadend-enabled") == 0)
                deadend.enabled = (1 == stoi(args[++i]));

            else if (args[i].compare("--deadend-generalize") == 0)
                deadend.generalize = (1 == stoi(args[++i]));

            else if (args[i].compare("--deadend-record-online") == 0)
                deadend.record_online = (1 == stoi(args[++i]));

            else if (args[i].compare("--deadend-combine") == 0)
                deadend.combine = (1 == stoi(args[++i]));

            else if (args[i].compare("--deadend-regress-trigger-only") == 0)
                deadend.regress_trigger_only = (1 == stoi(args[++i]));

            else if (args[i].compare("--deadend-force-1safe-weak-plans") == 0)
                deadend.force_1safe_weak_plans = (1 == stoi(args[++i]));

            else if (args[i].compare("--deadend-poison-search") == 0)
                deadend.poison_search = (1 == stoi(args[++i]));

            /**************************************************************/

            else if (args[i].compare("--epoch") == 0)
                epoch.number = stoi(args[++i]);

            /**************************************************************/

            else if (args[i].compare("--weaksearch-stop-on-policy") == 0)
                weaksearch.stop_on_policy = (1 == stoi(args[++i]));

            else if (args[i].compare("--weaksearch-penalize-potential-fsaps") == 0)
                weaksearch.penalize_potential_fsaps = (1 == stoi(args[++i]));

            else if (args[i].compare("--weaksearch-fsap-penalty") == 0)
                weaksearch.fsap_penalty = stoi(args[++i]);

            /**************************************************************/

            else if (args[i].compare("--output-format") == 0)
                output.format = stoi(args[++i]);

            /**************************************************************/

            else if (args[i].compare("--fondsearch-node-preference") == 0)
                fondsearch.node_preference = stoi(args[++i]);

            /**************************************************************/

            else if (args[i].compare("--localize-enabled") == 0)
                localize.enabled = (1 == stoi(args[++i]));

            else if (args[i].compare("--localize-generalize") == 0)
                localize.generalize = (1 == stoi(args[++i]));

            else if (args[i].compare("--localize-limited") == 0)
                localize.limited = (1 == stoi(args[++i]));

            else if (args[i].compare("--localize-max-states") == 0)
                localize.max_states = stoi(args[++i]);

            /**************************************************************/

            else if (args[i].compare("--psgraph-full-scd-marking") == 0)
                psgraph.full_scd_marking = (1 == stoi(args[++i]));

            else if (args[i].compare("--psgraph-clear-dead-solsteps") == 0)
                psgraph.clear_dead_solsteps = (1 == stoi(args[++i]));

            /**************************************************************/

            else if (args[i].compare("--simulator-trial-depth") == 0)
                simulator.trial_depth = stoi(args[++i]);

            else if (args[i].compare("--simulator-num-trials") == 0)
                simulator.num_trials = stoi(args[++i]);

            /**************************************************************/

            else if (args[i].compare("--logging-solstep-watch-list") == 0) {
                stringstream ss(args[++i]);
                string item;
                while (getline(ss, item, ',')) {
                    logging.solstep_watch_list.insert(atoi(item.c_str()));
                }
            }

            else if (args[i].compare("--logging-searchnode-watch-list") == 0) {
                stringstream ss(args[++i]);
                string item;
                while (getline(ss, item, ',')) {
                    logging.searchnode_watch_list.insert(atoi(item.c_str()));
                }
            }

            else if (args[i].compare("--logging-verbose") == 0)
                logging.verbose = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-dump-snapshots") == 0)
                logging.dump_snapshots = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-validate-network-and-nodes") == 0)
                logging.validate_network_and_nodes = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-disable-state-dump") == 0)
                logging.disable_state_dump = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-heuristic") == 0)
                logging.heuristic = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-weak-search") == 0)
                logging.weak_search = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-fond-search") == 0)
                logging.fond_search = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-fond-search-expanding") == 0)
                logging.fond_search_expanding = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-simulator") == 0)
                logging.simulator = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-psgraph") == 0)
                logging.psgraph = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-psgraph-condensed") == 0)
                logging.psgraph_condensed = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-deadends") == 0)
                logging.deadends = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-network-assertions") == 0)
                logging.network_assertions = (1 == stoi(args[++i]));

            else if (args[i].compare("--logging-poisoning") == 0)
                logging.poisoning = (1 == stoi(args[++i]));

            /**************************************************************/

            else if (args[i].compare("--final-fsap-free-round") == 0)
                general.final_fsap_free_round = (1 == stoi(args[++i]));

            else if (args[i].compare("--optimize-final-solution") == 0)
                general.optimize_final_solution = (1 == stoi(args[++i]));

            /**************************************************************/

            else
                throw std::invalid_argument( "Unknown argument: {" + args[i] + "}" );
        }

        return true;
    }

    string pr2_option_string() {
        return string("\n\n")
        + "\t\t\t -{ PR2 Usage }-\n\n"
        + "\n"
        + "\t --solution-evaluation-trials NUM_TRIALS (default=" + to_string(solution.evaluation_trials) + ")\n"
        + "\t\t Number of monte-carlo trials used to evaluate the quality of a policy.\n\n"
        + "\n\n"
        + "\t --time-limit TIME_LIMIT (default=" + to_string(int(time.limit)) + ")\n"
        + "\t\t Only search for the given time. This will be cut in half if --final-fsap-free-round is used.\n\n"
        + "\n\n"
        + "\t --deadend-enabled 1/0 (default=" + to_string(deadend.enabled) + ")\n"
        + "\t\t Use primitive deadend detection to ensure a strongly cyclic solution.\n\n"
        + "\t --deadend-generalize 1/0 (default=" + to_string(deadend.generalize) + ")\n"
        + "\t\t Generalize the deadends found based on relaxed reachability.\n\n"
        + "\t --deadend-record-online 1/0 (default=" + to_string(deadend.record_online) + ")\n"
        + "\t\t Generate and store deadend states that are found online.\n\n"
        + "\t --deadend-combine 1/0 (default=" + to_string(deadend.combine) + ")\n"
        + "\t\t Combine the FSAP conditions if every applicable action is forbidden to be a new deadend.\n\n"
        + "\t --deadend-regress-trigger-only 1/0 (default=" + to_string(deadend.regress_trigger_only) + ")\n"
        + "\t\t Regress deadends in the FSAP construction only through actions that trigger the deadend.\n\n"
        + "\t --deadend-force-1safe-weak-plans 1/0 (default=" + to_string(deadend.force_1safe_weak_plans) + ")\n"
        + "\t\t Keep computing weak plans until we have one that doesn't reach a deadend in one step.\n\n"
        + "\t --deadend-poison-search 1/0 (default=" + to_string(deadend.poison_search) + ")\n"
        + "\t\t Prune parts of the search space if they would no longer be reached in the same way because of a found deadend / FSAP.\n\n"
        + "\n\n"
        + "\t --epoch EPOCH_COUNT (default=" + to_string(epoch.number) + ")\n"
        + "\t\t Minimum number of times to execute the outer search loop for a policy. Useful if deadends are present and a single pass takes too long.\n\n"
        + "\n\n"
        + "\t --weaksearch-stop-on-policy 1/0 (default=" + to_string(weaksearch.stop_on_policy) + ")\n"
        + "\t\t Stop the weak sub-planning search when the policy matches the current state.\n\n"
        + "\t --weaksearch-penalize-potential-fsaps 1/0 (default=" + to_string(weaksearch.penalize_potential_fsaps) + ")\n"
        + "\t\t Penalize the FSAP actions in the heuristic computation by a constant amount..\n\n"
        + "\t --weaksearch-fsap-penalty PENALTY (default=" + to_string(weaksearch.fsap_penalty) + ")\n"
        + "\t\t The constant to use for penalizing FSAP actions in the heuristic computation.\n\n"
        + "\n\n"
        + "\t --output-format 1/2/3 (default=" + to_string(output.format) + ")\n"
        + "\t\t Dump the policy to the file policy.out.\n"
        + "\t\t  1. Creates a switch graph (currently unsafe to use)\n"
        + "\t\t  2. Creates a human readable form (preferred for use with the pr2_api.py file).\n"
        + "\t\t  3. Creates a JSON dump of the final solution graph (directed, and possibly cyclic).\n\n"
        + "\n\n"
        + "\t --fondsearch-node-preference [1-5] (default=" + to_string(fondsearch.node_preference) + ")\n"
        + "\t\t Controls the priority queue for which nodes to look at next according to:\n"
        + "\t\t  1. Stack -- most recently generated search node first\n"
        + "\t\t  2. Queue -- oldest search node first\n"
        + "\t\t  3. Near init -- nodes near the initial state first\n"
        + "\t\t  4. Away init -- nodes further from the initial state first\n"
        + "\t\t  5. Random -- random node is selected next\n\n"
        + "\n\n"
        + "\t --localize-enabled 0/1 (default=" + to_string(localize.enabled) + ")\n"
        + "\t\t Plan locally to recover before planning for the goal.\n\n"
        + "\t --localize-generalize 0/1 (default=" + to_string(localize.generalize) + ")\n"
        + "\t\t Use the partial state that matches the expect state when planning locally.\n\n"
        + "\t --localize-limited 0/1 (default=" + to_string(localize.limited) + ")\n"
        + "\t\t Limit the planlocal searching to a fixed number of search steps.\n\n"
        + "\t --localize-max-states MAX (default=" + to_string(localize.max_states) + ")\n"
        + "\t\t The number of states to limit the local planning search to (in expansions).\n\n"
        + "\n\n"
        + "\t --psgraph-full-scd-marking 0/1 (default=" + to_string(psgraph.full_scd_marking) + ")\n"
        + "\t\t Does a full graph analysis on the solution graph for marking nodes as strong cyclic.\n\n"
        + "\t --psgraph-clear-dead-solsteps 0/1 (default=" + to_string(psgraph.clear_dead_solsteps) + ")\n"
        + "\t\t Removes parts of the solution graph that have no search nodes associated with them.\n\n"
        + "\n\n"
        + "\t --simulator-trial-depth DEPTH (default=" + to_string(simulator.trial_depth) + ")\n"
        + "\t\t Stop simulations and consider it a failure after DEPTH actions.\n\n"
        + "\t --simulator-num-trials TRIALS (default=" + to_string(simulator.num_trials) + ")\n"
        + "\t\t Number of trials to run for the simulator.\n\n"
        + "\n\n"
        + "\t --logging-solstep-watch-list {comma-separated-id-list} (default={})\n"
        + "\t\t ID's for the solution steps that should be monitored for changes and logging recorded.\n\n"
        + "\t --logging-searchnode-watch-list {comma-separated-id-list} (default={})\n"
        + "\t\t ID's for the search nodes that should be monitored for changes and logging recorded.\n\n"
        + "\t --logging-verbose 0/1 (default=" + to_string(logging.verbose) + ")\n"
        + "\t\t Output plans and other information during the planning process.\n\n"
        + "\t --logging-dump-snapshots 0/1 (default=" + to_string(logging.dump_snapshots) + ")\n"
        + "\t\t Dump the solution graph and search space at every iteration of the FOND search (for visualization).\n\n"
        + "\t --logging-validate-network-and-nodes 0/1 (default=" + to_string(logging.validate_network_and_nodes) + ")\n"
        + "\t\t Validate the network connections in both the solution graph and search space at regular intervals.\n\n"
        + "\t --logging-disable-state-dump 0/1 (default=" + to_string(logging.disable_state_dump) + ")\n"
        + "\t\t Silence the part of loggin that dumps full and partial states.\n\n"
        + "\t --logging-heuristic 0/1 (default=" + to_string(logging.heuristic) + ")\n"
        + "\t\t Output logging info for the weak planning heuristic computation.\n\n"
        + "\t --logging-weak-search 0/1 (default=" + to_string(logging.weak_search) + ")\n"
        + "\t\t Output logging info for the weak search procedure.\n\n"
        + "\t --logging-fond-search 0/1 (default=" + to_string(logging.fond_search) + ")\n"
        + "\t\t Output logging info for the meta FOND search.\n\n"
        + "\t --logging-fond-search-expanding 0/1 (default=" + to_string(logging.fond_search_expanding) + ")\n"
        + "\t\t Output logging info for the search node expansions.\n\n"
        + "\t --logging-simulator 0/1 (default=" + to_string(logging.simulator) + ")\n"
        + "\t\t Output logging info for the simulator calls.\n\n"
        + "\t --logging-psgraph 0/1 (default=" + to_string(logging.psgraph) + ")\n"
        + "\t\t Output logging info for the solution graph (primarily the fixed point regression procedure).\n\n"
        + "\t --logging-psgraph-condensed 0/1 (default=" + to_string(logging.psgraph_condensed) + ")\n"
        + "\t\t Output logging info for the fixed point regression in condensed form (useful for larger problems).\n\n"
        + "\t --logging-deadends 0/1 (default=" + to_string(logging.deadends) + ")\n"
        + "\t\t Output logging info for deadend computation and maintenance.\n\n"
        + "\t --logging-network-assertions 0/1 (default=" + to_string(logging.network_assertions) + ")\n"
        + "\t\t Output logging info for the network validation code.\n\n"
        + "\t --logging-poisoning 0/1 (default=" + to_string(logging.poisoning) + ")\n"
        + "\t\t Output logging info for the poisoning procedure.\n\n"
        + "\n\n"
        + "\t --final-fsap-free-round 0/1 (default=" + to_string(general.final_fsap_free_round) + ")\n"
        + "\t\t Do one final meta search round with the best solution found (closing every leaf possible)..\n\n"
        + "\t --optimize-final-solution 0/1 (default=" + to_string(general.optimize_final_solution) + ")\n"
        + "\t\t Do a final simulation and throw out any solution step (or FSAP) not used..\n\n"
        + "\n"
        + "\n\n\t\tSee http://www.haz.ca/research/pr2 for details.\n\n\n";
    }
};

extern PR2Wrapper PR2; // Holds all of the settings and data for PR2

#endif
