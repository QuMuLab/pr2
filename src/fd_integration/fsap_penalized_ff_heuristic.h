#ifndef HEURISTICS_FSAP_PENALIZED_FF_HEURISTIC_H
#define HEURISTICS_FSAP_PENALIZED_FF_HEURISTIC_H

#include <set>

#include "../../heuristics/relaxation_heuristic.h"

#include "../../algorithms/priority_queues.h"
#include "../../utils/collections.h"

#include <cassert>

class State;
class PR2State;
struct FSAP;

using namespace std;

namespace fsap_penalized_ff_heuristic {
using relaxation_heuristic::PropID;
using relaxation_heuristic::OpID;
using relaxation_heuristic::NO_OP;
using relaxation_heuristic::Proposition;
using relaxation_heuristic::UnaryOperator;

class FSAPPenalizedFFHeuristic : public relaxation_heuristic::RelaxationHeuristic {
    /* Costs larger than MAX_COST_VALUE are clamped to max_value. The
       precise value (100M) is a bit of a hack, since other parts of
       the code don't reliably check against overflow as of this
       writing. With a value of 100M, we want to ensure that even
       weighted A* with a weight of 10 will have f values comfortably
       below the signed 32-bit int upper bound.
     */
    static const int MAX_COST_VALUE = 100000000;

    priority_queues::AdaptiveQueue<PropID> queue;
    bool did_write_overflow_warning;

    set<int> forbidden_ops; // The operators (non-det indices) that are currently forbidden
    set< FSAP* > seen_fsaps; // Keeps track of the enabled FSAPs during a heuristic computation

    // Relaxed plans are represented as a set of operators implemented
    // as a bit vector.
    typedef std::vector<bool> RelaxedPlan;
    RelaxedPlan relaxed_plan;

    void setup_exploration_queue();
    void setup_exploration_queue_state(const State &state);
    void setup_exploration_queue_state(const PR2State &state);
    void relaxed_exploration();
    void mark_preferred_operators(const State &state, PropID goal_id);
    void mark_preferred_operators_and_relaxed_plan(
        const State &state, PropID goal_id);

    int compute_fsap_penalty(int op_num);

    void enqueue_if_necessary(PropID prop_id, int cost, OpID op_id);

    void increase_cost(int &cost, int amount) {
        assert(cost >= 0);
        assert(amount >= 0);
        cost += amount;
        if (cost > MAX_COST_VALUE) {
            write_overflow_warning();
            cost = MAX_COST_VALUE;
        }
    }

    void write_overflow_warning();

protected:
    virtual int compute_heuristic(const State &state);
    virtual int compute_heuristic(const PR2State &state);
    // virtual int compute_heuristic(const State &global_state);

public:
    // explicit FSAPPenalizedFFHeuristic(const plugins::Options &options);
    explicit FSAPPenalizedFFHeuristic(
        shared_ptr<AbstractTask> task,
        bool cache_estimates,
        const string desc,
        utils::Verbosity verb);
    ~FSAPPenalizedFFHeuristic();

    // Reset things properly when we're about to replan
    virtual void reset(std::vector<FactPair> &goal_facts);
    virtual void reset();

    // Common part of h^add and h^ff computation.
    int compute_add_and_ff(const State &state);
    int compute_add_and_ff(const PR2State &state);

    void compute_heuristic_for_cegar(const State &state);

    int get_cost_for_cegar(int var, int value) const {
        return get_proposition(var, value)->cost;
    }
};
}

#endif
