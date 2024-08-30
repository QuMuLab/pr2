
#include "policy.h"
#include "solution.h"

int PolicyItem::generality() {
    if (-1 != _generality) {
        _generality = 0;
        for (unsigned i = 0; i < PR2.general.num_vars; i++) {
            if (-1 == value(i)) {
                _generality++;
            }
        }
    }

    return _generality;
}

bool PolicyItem::is_relevant(const PR2State &curr) {
    for (unsigned i = 0; i < PR2.general.num_vars; i++)
        if (isset(i) && (!curr.is_undefined(i)))
            return true;
    return false;
}

int PolicyItem::value(int var) {
    return (*state)[var];
}

vector< pair<int,int> > * PolicyItem::varvals() {
    return state->varvals();
}


Policy::~Policy() {
    if (root)
        delete root;

    for (auto item : all_items)
         delete item;
}

// TODO: Revisit this comparison -- just doing a pointer comparison for now
bool fsap_compare(PolicyItem* first, PolicyItem* second) {
    return ((const FSAP*)first) < ((const FSAP*)second);
}
bool solstep_compare(PolicyItem* first, PolicyItem* second) {
    return *((const SolutionStep*)first) < *((const SolutionStep*)second);
}
void Policy::write_policy(string fname, bool fsap) {

    if (!fsap)
        all_items.sort(fsap ? fsap_compare : solstep_compare);

    fstream outfile;
    outfile.open(fname, ios::out);

    for (auto item : all_items) {

        outfile << "\nIf holds:";
        PR2State *s = item->state;

        for (unsigned i = 0; i < PR2.general.num_vars; i++) {
            if (!(s->is_undefined(i))) {
                outfile << " ";
                outfile << PR2.proxy->get_variables()[i].get_name() << ":" << (*s)[i];
            }
        }
        outfile << endl;

        if (fsap)
            outfile << "Forbid: " << item->get_name() << endl;
        else
            outfile << "Execute: " << item->get_name() << endl;

    }

    outfile.close();
}

void Policy::dump(bool fsap) const {
    cout << (fsap ? "FSAP " : "") << "Policy:" << endl;
    for (auto item : all_items)
         item->dump();
}

void Policy::generate_cpp_input(ofstream &outfile) const {
    root->generate_cpp_input(outfile);
}

void Policy::add_item(PolicyItem *item) {
    list<PolicyItem *> reg_items;
    reg_items.push_back(item);
    update_policy(reg_items);
}

void Policy::update_policy(list<PolicyItem *> &reg_items) {

    list<MatchtreeItem *> mtis;
    for (auto item : reg_items)
        mtis.push_back((MatchtreeItem *)item);

    set<int> vars_seen;
    if (root)
        root->update_policy(mtis, vars_seen);
    else
        root = new MatchtreeSwitch(mtis, vars_seen);
    all_items.insert(all_items.end(), reg_items.begin(), reg_items.end());

}

bool Policy::check_consistent_match(const PR2State &curr) {
    if (root)
        return root->check_consistent_match(curr);
    else
        return false;
}

bool Policy::check_entailed_match(const PR2State &curr) {
    if (root)
        return root->check_entailed_match(curr);
    else
        return false;
}

void Policy::rebuild() {

    // We are only going to keep the items that are active
    list<MatchtreeItem *> mtis;
    list<PolicyItem *> new_items;
    for (auto item : all_items) {
        if (item->is_active) {
            mtis.push_back((MatchtreeItem *)item);
            new_items.push_back(item);
        } else {
            delete item;
        }
    }

    set<int> vars_seen;
    delete root;
    root = new MatchtreeSwitch(mtis, vars_seen);

    all_items.swap(new_items);

}
