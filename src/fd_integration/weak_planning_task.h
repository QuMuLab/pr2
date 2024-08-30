#ifndef TASKS_WEAK_PLANNING_TASK_H
#define TASKS_WEAK_PLANNING_TASK_H

#include "../../tasks/delegating_task.h"

#include <vector>

namespace extra_tasks {
class WeakPlanningTask : public tasks::DelegatingTask {
    const std::vector<int> initial_state_values;
    const std::vector<FactPair> goals;
public:
    WeakPlanningTask(
        const std::shared_ptr<AbstractTask> &parent,
        std::vector<int> &&initial_state_values,
        std::vector<FactPair> &&goals);
    ~WeakPlanningTask() = default;

    virtual int get_num_goals() const override;
    virtual FactPair get_goal_fact(int index) const override;
    
    virtual std::vector<int> get_initial_state_values() const override;
};
}

#endif
