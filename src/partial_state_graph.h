#ifndef PARTIAL_STATE_GRAPH_H
#define PARTIAL_STATE_GRAPH_H

#include "solution.h"
#include "fond_search.h"

class PR2State;

struct PSGraph {

    SolutionStep * init;
    SolutionStep * goal;

    set<SolutionStep *> steps;

    PSGraph();
    ~PSGraph();

    void add_step(SolutionStep * step) { steps.insert(step); }
    void remove_step(SolutionStep * step) { steps.erase(step); }

    void fixed_point_regression(SolutionStep * src,
                                SolutionStep * old_dst,
                                SolutionStep * new_dst,
                                PR2SearchNode * src_node,
                                PR2SearchNode * dst_node,
                                int successor_id_for_dst,
                                map< SolutionStep* , set< PR2SearchNode * > * > &solstep2searchnode,
                                list<SolutionStep *> &new_steps,
                                bool make_connection = false);

    void fixed_point_marking(SolutionStep * node);
    void full_marking();

    void record_snapshot(ofstream &outfile, string indent, bool keyname = true);
    void crawl_steps(SolutionStep * n, bool reversed, set< SolutionStep * > &seen);
};

#endif
