#include "weak_planning_task.h"

using namespace std;

namespace extra_tasks {
WeakPlanningTask::WeakPlanningTask(
    const shared_ptr<AbstractTask> &parent,
    vector<int> &&initial_state_values,
    vector<FactPair> &&goals)
    : DelegatingTask(parent),
      initial_state_values(move(initial_state_values)),
      goals(move(goals)) {
}

int WeakPlanningTask::get_num_goals() const {
    return goals.size();
}

FactPair WeakPlanningTask::get_goal_fact(int index) const {
    return goals[index];
}

std::vector<int> WeakPlanningTask::get_initial_state_values() const {
    return initial_state_values;
}

}
