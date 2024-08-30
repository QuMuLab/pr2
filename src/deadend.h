#ifndef DEADEND_H
#define DEADEND_H

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "pr2.h"
#include "policy.h"

#include "fd_integration/pr2_proxies.h"
#include "fd_integration/partial_state.h"
#include "fd_integration/fsap_penalized_ff_heuristic.h"


class PR2OperatorProxy;
class OperatorID;

class PR2State;

struct DeadendTuple {
    PR2State *de_state;
    PR2State *prev_state;
    const PR2OperatorProxy *prev_op;

    DeadendTuple(PR2State *ds, PR2State *ps, const PR2OperatorProxy *op) : de_state(ds), prev_state(ps), prev_op(op) {}
    ~DeadendTuple() {};
};

struct FSAP : PolicyItem {

    PR2OperatorProxy *op; // The nondet action id we are forbidding

    FSAP(PR2State *s, PR2OperatorProxy o);
    FSAP(PR2State *s) : PolicyItem(s), op(NULL) {}

    ~FSAP() {}

    bool operator< (const FSAP& other) const;

    string get_name();
    int get_index();
    void dump() const;
};

struct Deadend : FSAP {
    Deadend(PR2State *s) : FSAP(s) {};
    void dump() const;
};

void update_deadends(vector< DeadendTuple * > &failed_states);

bool is_deadend(PR2State &state);
bool is_forbidden(PR2State &state, const PR2OperatorProxy op);

bool generalize_deadend(PR2State &state);


#endif
