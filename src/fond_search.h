#ifndef FOND_SEARCH_H
#define FOND_SEARCH_H

#include <algorithm>
#include <queue>
#include <set>
#include <map>
#include <list>
#include <vector>

#include "deadend.h"
#include "pr2.h"
#include "fd_integration/pr2_proxies.h"
#include "fd_integration/pr2_search_algorithm.h"

class Simulator;
class PR2Search;

bool find_better_solution(Simulator *sim);

bool case1_poisoned_node(PR2SearchStatus *status);
bool case2_match_complete_state(PR2SearchStatus * status);
bool case3_predefined_path(PR2SearchStatus * status);
bool case4_hookup_solsteps(PR2SearchStatus * status);
bool case5_new_path(PR2SearchStatus * status);
bool case6_deadend(PR2SearchStatus * status);

void strengthen_and_mark(PR2SearchStatus * status,
                         SolutionStep * previous_step,
                         SolutionStep * solstep,
                         SolutionStep * join_solstep,
                         PR2SearchNode * previous_node,
                         PR2SearchNode * current_node,
                         int successor_id_for_dst);


/***********************************************************************
 * The search status contains the data structures that we maintain for
 * the general (i.e., high-level) FOND search. This includes open lists,
 * collections of created states, mappings between solsteps and search
 * nodes, etc. It also contains info on the current search node so that
 * the various phases can access details succinctly: this includes things
 * like the current and previous nodes, solsteps, expected goals, etc.
 **********************************************************************/
struct PR2SearchStatus {

    Simulator *sim;

    // Note: The seen and state2searchnode data structures are doing double
    //       duty. Technically, we could get away with just the latter, but
    //       we would face a log N complexity on the lookup instead of just
    //       constant time, as is the case with the set seen.

    // Data structures that make up the FOND search progress and status
    set< PR2State > * seen; // Keeps track of the full states we've seen
    priority_queue< PR2SearchNode *, vector< PR2SearchNode * >, pr2_node_comparison > * open_list; // Open list we traverse until strong cyclicity is proven
    vector< PR2State * > * created_states; // Used to clean up the created state objects
    vector< DeadendTuple * > * failed_states; // The failed states (used for creating deadends)
    map< SolutionStep* , set< PR2SearchNode * > * > * solstep2searchnode; // Mapping from a solstep to the nodes that are handled by that solstep
    map< PR2State, PR2SearchNode * > * state2searchnode; // Mapping from the complete state to the appropriate (closed) search node
    list< PR2SearchNode * > * created_search_nodes = NULL; // Just a list of the search nodes for printing and reference

    // Backups of the original goal and initial state
    PR2State * old_initial_state;
    PR2State * goal_orig;

    // Boolean flags to keep track of the status of the search
    bool made_change = false; // True if we add anything to the g_policy (i.e., replan)
    bool poisoned = false; // True if a deadend has caused some of the psgraph to be invalidated
    bool warm_start = false; // Keep track if we've continued a search, or started from scratch

    // Statistics on the search progress
    int num_checked_states = 0; // Number of states we check
    int num_fixed_states = 0; // Number of states we were able to repair (by replanning)
    string last_round_type = "Initial Round"; // Records what happened during the last round for snapshots

    // Data structures for the current state in the search
    PR2SearchNode * current_node = NULL;
    PR2SearchNode * previous_node; // The previous search node in the search
    SolutionStep * previous_step; // The solution step that led to the current state in the loop
    PR2State * current_state; // The current state in the loop
    PR2State * current_goal; // The current goal in the loop
    PR2OperatorProxy * previous_op; // The operator that took us from previous_node->full_state to the current state
    int prev_to_curr_outcome; // The outcome id that leads previous_node to current_node

    // Shouldn't be copying this directly.
    PR2SearchStatus() = delete;
    PR2SearchStatus(Simulator *s) : sim(s) {}
    ~PR2SearchStatus();

    // Initializes all of the data structures.
    void init();

    // Boolean checks that are used during search
    bool keep_searching ();
    bool repeat_state();
    bool need_to_update_incumbent();
    bool need_to_update_deadends();
    bool need_to_rerun();

    // General methods for key parts of the search
    void pop_next_node ();
    void record_new_state ();
    void save_for_epoch();
    void update_incumbent_if_needbe();
    void update_deadends_if_needbe();
    void mark_current_state_failed();

    // Logging methods
    void log_end_of_round();
    void snapshot_if_needbe();
    void validate_if_needbe();

};

struct PR2SearchNode {

    PR2State * full_state; // The full state in the search
    PR2State * expected_state; // The partial state we expected to hit in the solution graph

    vector <int> previous_node_outcomes; // The nondet outcomes that lead to this node (corresponds to the previous_nodes vector)
    vector <PR2SearchNode *> previous_nodes; // Nodes attached to incoming edges
    vector <PR2SearchNode *> next_nodes; // Nodes attached to outgoing edges

    SolutionStep * parent_step; // The SolutionStep that lead to this node
    SolutionStep * matched_step; // The SolutionStep that we expect to match

    int id;

    bool open; // True when we haven't expanded this during the fond search yet
    bool init; // True if it is the first node in the fond search (and doesn't start with a predecessor)
    bool subsumed; // True if it's complete state is matched exactly by another search node (and thus is merged)
    bool poisoned; // True if we are marking this part of the search space dead (either a deadend or has a poisoned ancestor)

    PR2SearchNode(const PR2SearchNode & that) = delete;

    PR2SearchNode() : PR2SearchNode(NULL, NULL, NULL, NULL, -1) {}

    PR2SearchNode(PR2State * fs, PR2State * es, PR2SearchNode * pn, SolutionStep * pr, int s_id) :
       full_state(fs), expected_state(es), parent_step(pr), matched_step(NULL), id(PR2.fondsearch.PR2NodeCount++), open(true), init(false), subsumed(false), poisoned(false)
    {
        if (pn) {
            assert(s_id >= 0);
            pn->next_nodes.push_back(this);
            previous_nodes.push_back(pn);
            previous_node_outcomes.push_back(s_id);
        } else
            init = true;
    }

    PR2SearchNode * expand(PR2SearchStatus * status, SolutionStep * solstep);

    bool operator==(const PR2SearchNode &other) const { return id == other.id; }
    bool operator!=(const PR2SearchNode &other) const { return id != other.id; }
    bool operator<(const PR2SearchNode &other) const { return id < other.id; }
    size_t hash() const { return id; }

    void poison();
    void poison_recurse();

    void dump() const;
    void validate();
    void record_snapshot(ofstream &outfile, string indent="");
};

class pr2_node_comparison
{
    public:
        bool operator () (const PR2SearchNode * n1, const PR2SearchNode * n2);
};


#endif
