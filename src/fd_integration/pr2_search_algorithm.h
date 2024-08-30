#ifndef PR2_SEARCH_ALGORITHM_H
#define PR2_SEARCH_ALGORITHM_H

#include "../../plugins/options.h"
#include "../../search_algorithm.h"
#include "../../task_utils/successor_generator.h"
#include "../../utils/timer.h"
#include "../../open_lists/best_first_open_list.h"

#include "fsap_penalized_ff_heuristic.h"
#include "pr2_proxies.h"

namespace fsap_penalized_ff_heuristic {
    class FSAPPenalizedFFHeuristic;
}


namespace successor_generator {
    class SuccessorGenerator;
}

#include <memory>

namespace plugins {
class Options;
}

struct DeadendAwareSuccessorGenerator {
    void generate_applicable_ops(const PR2State &curr, vector<OperatorID> &ops) const;
};

namespace pr2_search {
class PR2Search : public SearchAlgorithm {

    // The heuristic we want to use. If there is ever more than one option,
    //  then a new superclass should be created to allow reseting and such.
    // Heuristic *h;
    std::shared_ptr<fsap_penalized_ff_heuristic::FSAPPenalizedFFHeuristic> h;

    vector<std::shared_ptr<Evaluator>> preferred_list;
    vector<std::shared_ptr<Evaluator>> preferred_list_scalar;

    std::unique_ptr<SearchAlgorithm> get_search_engine();
    virtual SearchStatus step() override;

    DeadendAwareSuccessorGenerator deadend_aware_successor_generator;

public:
    explicit PR2Search(const plugins::Options &opts);

    const successor_generator::SuccessorGenerator * get_orig_successor_generator() {
        return &successor_generator;
    };

    const DeadendAwareSuccessorGenerator * get_deadend_aware_successor_generator() {
        return &deadend_aware_successor_generator;
    };

    virtual void save_plan_if_necessary() override;
    virtual void print_statistics() const override;

    DeterministicPlan get_plan() const;
};
}

#endif
