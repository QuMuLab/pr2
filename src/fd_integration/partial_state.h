#ifndef PARTIAL_STATE_H
#define PARTIAL_STATE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "../../task_proxy.h"

class PR2OperatorProxy;
class StateInterface;

class PR2State : public StateInterface {
    std::vector<int> vars; // values for vars
    std::vector< std::pair<int,int> > * _varvals = NULL; // varval pairs for partial states
    // void _allocate();
    // void _deallocate();
    // void _copy_buffer_from_state(const PR2State &state);

public:

    void unpack() const {};
    const std::vector<int> &get_unpacked_values() const {
        return vars;
    };

    PR2State &operator=(const PR2State &other);

    PR2State(); // Creates a state with -1 values for everything
    PR2State(std::vector<int> init_vals);
    PR2State(const PR2State &state);
    PR2State(const State &state);
    ~PR2State();

    bool is_undefined(VariableProxy var) const {
        return is_undefined(var.get_id());
    }

    bool is_undefined(int var) const {
        return vars[var] == -1;
    }

    int size() const;

    PR2State * progress(const PR2OperatorProxy &op);
    PR2State * regress(const PR2OperatorProxy &op, PR2State *context=NULL);

    bool triggers(const EffectProxy &effect);
    bool consistent_with(const PR2State &other);
    bool entails(const PR2State &other);
    void combine_with(const PR2State &state);
    std::vector< std::pair<int,int> > * varvals();

    int &operator[](int index) {
        // Reset varvals cache
        if (_varvals) {
            delete _varvals;
            _varvals = NULL;
        }
        return vars[index];
    }
    int operator[](int index) const {
        return vars[index];
    }

    void dump_pddl() const;
    void dump_fdr() const;

    bool operator==(const PR2State &other) const;
    bool operator<(const PR2State &other) const;

    // size_t hash() const;

    void record_snapshot(std::ofstream &outfile, std::string indent);

};

#endif
