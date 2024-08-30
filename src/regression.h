
#ifndef REGRESSION_H
#define REGRESSION_H

#include "pr2.h"

#include "policy.h"

class PR2OperatorProxy;
class PR2State;

struct RegressableOperator : PolicyItem {
    const PR2OperatorProxy op;

    RegressableOperator(const PR2OperatorProxy &o, PR2State *s) : PolicyItem(s), op(o) {}

    virtual ~RegressableOperator() {}

    bool check_relevance(const PR2State &ps);

    virtual string get_name();
    virtual void dump() const;
};

void generate_regressable_ops();

#endif
