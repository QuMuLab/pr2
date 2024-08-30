#ifndef MATCH_TREE_H
#define MATCH_TREE_H

#include <set>
#include <list>
#include <vector>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <map>

#include "pr2.h"
#include "fd_integration/partial_state.h"

class PR2State;

using namespace std;

struct MatchtreeItem {
public:
    virtual bool entails(int var, int val) = 0;
    virtual bool consistent(int var, int val) = 0;
    virtual bool is_relevant(const PR2State &curr) = 0;
    virtual bool isset(int var) = 0;
    virtual int value(int var) = 0;
    virtual string get_name() = 0;
    virtual void dump() const = 0;
    virtual ~MatchtreeItem() {}
};


class MatchtreeBase {
public:
    virtual ~MatchtreeBase() {}
    virtual void dump(string indent) const = 0;

    virtual MatchtreeBase *update_policy(list<MatchtreeItem *> &items, set<int> &vars_seen) = 0;
    MatchtreeBase *create_generator(list<MatchtreeItem *> &items, set<int> &vars_seen);
    virtual void generate_cpp_input(ofstream &outfile) const = 0;

    virtual void generate_consistent_items(const PR2State &curr, vector<MatchtreeItem *> &items, bool only_if_relevant) = 0;
    virtual void generate_entailed_items(const PR2State &curr, vector<MatchtreeItem *> &items) = 0;

    virtual bool check_consistent_match(const PR2State &curr) = 0;
    virtual bool check_entailed_match(const PR2State &curr) = 0;

    int get_best_var(list<MatchtreeItem *> &items, set<int> &vars_seen);
    bool item_done(MatchtreeItem *item, set<int> &vars_seen);
};

class MatchtreeSwitch : public MatchtreeBase {
    int switch_var;
    list<MatchtreeItem *> immediate_items;
    vector<MatchtreeBase *> generator_for_value;
    MatchtreeBase *default_generator;

public:
    MatchtreeSwitch(int switch_variable,
                    list<MatchtreeItem *> &items,
                    const vector<MatchtreeBase *> &gen_for_val,
                    MatchtreeBase *default_gen);
    MatchtreeSwitch(list<MatchtreeItem *> &items, set<int> &vars_seen);
    virtual ~MatchtreeSwitch();

    virtual void dump(string indent) const;
    virtual void generate_cpp_input(ofstream &outfile) const;

    virtual MatchtreeBase *update_policy(list<MatchtreeItem *> &items, set<int> &vars_seen);

    virtual void generate_consistent_items(const PR2State &curr, vector<MatchtreeItem *> &items, bool only_if_relevant);
    virtual void generate_entailed_items(const PR2State &curr, vector<MatchtreeItem *> &items);

    virtual bool check_consistent_match(const PR2State &curr);
    virtual bool check_entailed_match(const PR2State &curr);
};

class MatchtreeLeaf : public MatchtreeBase {
    list<MatchtreeItem *> applicable_items;
public:
    MatchtreeLeaf(list<MatchtreeItem *> &items);

    virtual void dump(string indent) const;
    virtual void generate_cpp_input(ofstream &outfile) const;

    virtual MatchtreeBase *update_policy(list<MatchtreeItem *> &items, set<int> &vars_seen);

    virtual void generate_consistent_items(const PR2State &curr, vector<MatchtreeItem *> &items, bool only_if_relevant);
    virtual void generate_entailed_items(const PR2State &curr, vector<MatchtreeItem *> &items);

    virtual bool check_consistent_match(const PR2State &curr);
    virtual bool check_entailed_match(const PR2State &curr);
};

class MatchtreeEmpty : public MatchtreeBase {
public:
    virtual void dump(string indent) const;
    virtual void generate_cpp_input(ofstream &outfile) const;

    virtual MatchtreeBase *update_policy(list<MatchtreeItem *> &items, set<int> &vars_seen);

    virtual void generate_consistent_items(const PR2State &, vector<MatchtreeItem *> &, bool) {};
    virtual void generate_entailed_items(const PR2State &, vector<MatchtreeItem *> &) {};

    virtual bool check_consistent_match(const PR2State &) {return false;}
    virtual bool check_entailed_match(const PR2State &) {return false;}
};


#endif
