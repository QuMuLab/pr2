
#include "expand.h"


PR2State * generate_nondet_successors(PR2State * current_state, const PR2OperatorProxy * op, vector<NondetSuccessor *> &successors) {

    PR2State * expected = nullptr;
    for (auto oid : PR2.general.nondet_mapping[op->nondet_index]) {
        PR2OperatorProxy o = PR2.proxy->get_operators()[oid];
        successors.push_back(new NondetSuccessor(current_state->progress(o),
                                                 (oid == op->get_id()), o.nondet_outcome));
        if (o == *op)
            expected = successors.back()->state;
    }

    // Make sure that we have the right number of successors and expected state
    assert(successors.size() == PR2.general.nondet_mapping[op->nondet_index].size());
    assert(expected);

    return expected;

}

NondetSuccessor::~NondetSuccessor() {
    if (state)
        delete state;
}
