#ifndef POLICY_H
#define POLICY_H

#include "pr2.h"

#include "match_tree.h"

struct SolutionStep;
class PR2State;

struct PolicyItem : MatchtreeItem {

    PR2State *state;
    int _generality;
    bool is_active; // If false, then the item was pruned out

    PolicyItem(PR2State *s) : state(s), _generality(-1), is_active(true) {}
    virtual ~PolicyItem() {}

    virtual string get_name() = 0;
    virtual void dump() const = 0;

    virtual bool check_relevance(const PR2State &) {
        cout << "\n\nWarning: Calling item.relevant(ps) on a non-RegressableOperator item.\n" << endl;
        return false;
    }

    int generality();
    bool is_relevant(const PR2State &curr);

    int value(int var);
    bool entails(int var, int val) { return value(var) == val; }
    bool isset(int var) { return value(var) != -1; }
    bool consistent(int var, int val) {
        return entails(var,val) || !isset(var);
    }

    vector< pair<int,int> > * varvals();

};

struct FSAP : PolicyItem {

    PR2OperatorProxy *op; // The nondet action id we are forbidding

    FSAP(PR2State *s, PR2OperatorProxy *o);
    FSAP(PR2State *s) : PolicyItem(s), op(NULL) {}

    ~FSAP() {}

    bool operator< (const FSAP& other) const;

    string get_name();
    int get_index();
    void dump() const;
};

template<typename T> class Policy {
    MatchtreeBase *root;

    // private copy constructor to forbid copying;
    // typical idiom for classes with non-trivial destructors
    Policy(const Policy &copy);

public:

    Policy() : root(nullptr) {};
    ~Policy();

    list<T *> all_items;

    void dump(bool fsap = false) const;
    void write_policy(string fname, bool fsap = false);
    void generate_cpp_input(ofstream &outfile) const;

    void add_item(T *item);
    void update_policy(list<T *> &reg_items);

    bool check_consistent_match(const PR2State &curr);
    bool check_entailed_match(const PR2State &curr);

    void rebuild();

    bool empty() { return (nullptr == root); }

    int size() { return all_items.size(); }

    // We need to define these inline since they are templated
    template <class T1>
    void generate_consistent_items(const PR2State &curr, vector<T1 *> &reg_items, bool only_if_relevant) {
        vector<MatchtreeItem *> mtis;
        if (root)
            root->generate_consistent_items(curr, mtis, only_if_relevant);
        for (auto item : mtis)
            reg_items.push_back((T *)item);
    }

    template <class T1>
    void generate_entailed_items(const PR2State &curr, vector<T1 *> &reg_items) {
        vector<MatchtreeItem *> mtis;
        if (root)
            root->generate_entailed_items(curr, mtis);
        for (auto item : mtis)
            reg_items.push_back((T *)item);
    }

    void record_snapshot(ofstream &outfile, string indent) {
        outfile << indent << "\"policy\": \"Coming soon...\"," << endl;
    }

};

bool fsap_compare(PolicyItem *first, PolicyItem *second);
bool solstep_compare(PolicyItem *first, PolicyItem *second);

#endif
