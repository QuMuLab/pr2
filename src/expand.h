#ifndef EXPAND_H
#define EXPAND_H

#include <vector>

#include "pr2.h"
#include "fd_integration/pr2_proxies.h"
#include "fd_integration/partial_state.h"

class PR2OperatorProxy;

class PR2State;

/*******************************************************
 * For more advanced reachability, such as multi-agent *
 *  stuff, derived predicates, etc, etc, you can add   *
 *  extra details to the NondetSuccessor struct and    *
 *  re-define what generate_successors does.           *
 *                                                     *
 * Note: If you want to modify things in such a way,   *
 *       then you will need to change the SolutionStep *
 *       interface for get_successor(...) to match the *
 *       same format as the id field below. This is    *
 *       because the reachable tree needs to coincide  *
 *       with the generalized solution graph.          *
 *******************************************************/

struct NondetSuccessor {
    PR2State * state; // The reached state
    bool expected; // True if this is the expected outcome
    int id; // Unique ID for the successor among its siblings
    NondetSuccessor(PR2State * ps, bool exp, int id) : state(ps),
                                                           expected(exp),
                                                           id(id) {};
    ~NondetSuccessor();
};

PR2State * generate_nondet_successors(PR2State * current_state,
                                          const PR2OperatorProxy * op,
                                          vector<NondetSuccessor *> &successors);

#endif
