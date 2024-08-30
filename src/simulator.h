#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "pr2.h"
#include "expand.h"

#include "fd_integration/pr2_search_algorithm.h"

class Solution;
class SolutionStep;

class Simulator {

    PR2State *current_state;
    PR2State *current_goal;

    void search();
    void setup_simulation(PR2State *cur = nullptr);
    void reset_goal();
    void set_local_goal();
    bool check_1safe();
    SolutionStep* record_plan();

    const PR2OperatorProxy pick_action(SolutionStep *step, int index = -1);

    bool last_run_hit_depth = false;
    int last_run_count = 0;

public:

    shared_ptr<pr2_search::PR2Search> engine;

    Simulator(shared_ptr<pr2_search::PR2Search> engine);
    ~Simulator() {};

    bool simulate_policy(Solution *sol, PR2State *cur = nullptr);
    bool simulate_graph(Solution *sol, PR2State *cur = nullptr);
    bool simulate_solution(Solution *sol, PR2State *cur = nullptr);

    void run_trials();

    SolutionStep* replan();

    void set_state(PR2State * s) { current_state = s; }
    void set_goal(PR2State * s) { current_goal = s; }

};

#endif
