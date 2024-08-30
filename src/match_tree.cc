
#include "match_tree.h"

#include "fd_integration/pr2_proxies.h"


/********
 * Base *
 ********/

MatchtreeBase *MatchtreeBase::create_generator(list<MatchtreeItem *> &items, set<int> &vars_seen) {
    if (items.empty())
        return new MatchtreeEmpty;

    // If every item is done, then we create a leaf node
    bool all_done = true;
    for (auto item : items) {
        if (!item_done(item, vars_seen)) {
            all_done = false;
            break;
        }
    }

    if (all_done)
        return new MatchtreeLeaf(items);
    else
        return new MatchtreeSwitch(items, vars_seen);
}

int MatchtreeBase::get_best_var(list<MatchtreeItem *> &items, set<int> &vars_seen) {

    // (count,varnum)
    vector< pair<int,int> > var_count = vector< pair<int,int> >(PR2.general.num_vars);

    for (unsigned i = 0; i < PR2.general.num_vars; i++) {
        var_count[i] = pair<int,int>(0, (int)i);
    }

    for (auto item : items) {
        for (unsigned var = 0; var < PR2.general.num_vars; var++) {
            if (item->isset((int)var)) {
                var_count[var].first++;
            }
        }
    }

    sort(var_count.begin(), var_count.end());

    for (int i = var_count.size() - 1; i >= 0; i--)
        if (vars_seen.count(var_count[i].second) <= 0)
            return var_count[i].second;

    assert(false);
    return -1;
}

bool MatchtreeBase::item_done(MatchtreeItem *item, set<int> &vars_seen) {
    for (unsigned var = 0; var < PR2.general.num_vars; var++)
        if (item->isset(var) && (vars_seen.count(var) <= 0))
            return false;

    return true;
}


/**********
 * Switch *
 **********/

MatchtreeSwitch::MatchtreeSwitch(int switch_variable,
                                 list<MatchtreeItem *> &items,
                                 const vector<MatchtreeBase *> &gen_for_val,
                                 MatchtreeBase *default_gen)
    : switch_var(switch_variable),
      generator_for_value(gen_for_val),
      default_generator(default_gen) {

    immediate_items.swap(items);
}

MatchtreeSwitch::MatchtreeSwitch(list<MatchtreeItem *> &items, set<int> &vars_seen) {
    switch_var = get_best_var(items, vars_seen);

    vector< list<MatchtreeItem *> > value_items;
    list<MatchtreeItem *> default_items;

    // Initialize the value_items
    for (int i = 0; i < PR2.proxy->get_variables()[switch_var].get_domain_size(); i++)
        value_items.push_back(list<MatchtreeItem *>());

    // Sort out the items into the proper bin
    for (auto item : items) {

        if (item_done(item, vars_seen))
            immediate_items.push_back(item);

        else if (item->isset(switch_var))
            value_items[item->value(switch_var)].push_back(item);

        else // == -1
            default_items.push_back(item);
    }

    vars_seen.insert(switch_var);

    // Create the switch generators
    for (auto item_list : value_items)
        generator_for_value.push_back(create_generator(item_list, vars_seen));

    // Create the default generator
    default_generator = create_generator(default_items, vars_seen);

    vars_seen.erase(switch_var);
}

MatchtreeSwitch::~MatchtreeSwitch() {
    for (auto child : generator_for_value)
        delete child;
    delete default_generator;
}

void MatchtreeSwitch::dump(string indent) const {
    cout << indent << "switch on " << PR2.proxy->get_variables()[switch_var].get_name() << endl;
    cout << indent << "immediately:" << endl;
    for (auto item : immediate_items)
        cout << indent << item->get_name() << endl;
    for (int i = 0; i < PR2.proxy->get_variables()[switch_var].get_domain_size(); i++) {
        cout << indent << "case " << i << ":" << endl;
        generator_for_value[i]->dump(indent + "  ");
    }
    cout << indent << "always:" << endl;
    default_generator->dump(indent + "  ");
}

void MatchtreeSwitch::generate_cpp_input(ofstream &outfile) const {
    outfile << "switch " << switch_var << endl;
    outfile << "check " << immediate_items.size() << endl;
    for (auto item : immediate_items)
        outfile << item->get_name() << endl;
    for (auto child : generator_for_value)
        child->generate_cpp_input(outfile);
    default_generator->generate_cpp_input(outfile);
}

MatchtreeBase *MatchtreeSwitch::update_policy(list<MatchtreeItem *> &items, set<int> &vars_seen) {
    vector< list<MatchtreeItem *> > value_items;
    list<MatchtreeItem *> default_items;

    // Initialize the value_items
    for (int i = 0; i < PR2.proxy->get_variables()[switch_var].get_domain_size(); i++)
        value_items.push_back(list<MatchtreeItem *>());

    // Sort out the items into the proper bin
    for (auto item : items) {

        if (item_done(item, vars_seen))
            immediate_items.push_back(item);

        else if (item->isset(switch_var))
            value_items[item->value(switch_var)].push_back(item);

        else // == -1
            default_items.push_back(item);
    }

    vars_seen.insert(switch_var);

    // Update the switch generators
    for (unsigned i = 0; i < value_items.size(); i++) {
        MatchtreeBase *newGen = generator_for_value[i]->update_policy(value_items[i], vars_seen);
        if (NULL != newGen) {
            delete generator_for_value[i];
            generator_for_value[i] = newGen;
        }
    }

    // Update the default generator
    MatchtreeBase *newGen = default_generator->update_policy(default_items, vars_seen);
    if (NULL != newGen) {
        delete default_generator;
        default_generator = newGen;
    }

    vars_seen.erase(switch_var);

    return NULL;
}

void MatchtreeSwitch::generate_consistent_items(const PR2State &curr, vector<MatchtreeItem *> &items, bool only_if_relevant) {
    for (auto item : immediate_items)
        if (!only_if_relevant || item->is_relevant(curr))
            items.push_back(item);

    if (!curr.is_undefined(switch_var))
        generator_for_value[curr[switch_var]]->generate_consistent_items(curr, items, only_if_relevant);
    else
        for (auto child : generator_for_value)
            child->generate_consistent_items(curr, items, only_if_relevant);

    default_generator->generate_consistent_items(curr, items, only_if_relevant);
}

void MatchtreeSwitch::generate_entailed_items(const PR2State &curr, vector<MatchtreeItem *> &items) {
    for (auto item : immediate_items)
        items.push_back(item);

    if (!curr.is_undefined(switch_var))
        generator_for_value[curr[switch_var]]->generate_entailed_items(curr, items);

    default_generator->generate_entailed_items(curr, items);
}

bool MatchtreeSwitch::check_consistent_match(const PR2State &curr) {
    if (immediate_items.size() > 0)
        return true;

    if ((!curr.is_undefined(switch_var)) && (generator_for_value[curr[switch_var]]->check_consistent_match(curr)))
        return true;

    if (curr.is_undefined(switch_var)) {
        for (auto child : generator_for_value)
            if (child->check_consistent_match(curr))
                return true;
    }

    if (default_generator->check_consistent_match(curr))
        return true;

    return false;
}

bool MatchtreeSwitch::check_entailed_match(const PR2State &curr) {
    if (immediate_items.size() > 0)
        return true;

    if ((!curr.is_undefined(switch_var)) && (generator_for_value[curr[switch_var]]->check_entailed_match(curr)))
        return true;

    if (default_generator->check_entailed_match(curr))
        return true;

    return false;
}


/********
 * Leaf *
 ********/

MatchtreeLeaf::MatchtreeLeaf(list<MatchtreeItem *> &items) {
    applicable_items.swap(items);
}

void MatchtreeLeaf::dump(string indent) const {
    for (auto item : applicable_items)
        cout << indent << item->get_name() << endl;
}

void MatchtreeLeaf::generate_cpp_input(ofstream &outfile) const {
    outfile << "check " << applicable_items.size() << endl;
    for (auto item : applicable_items)
        outfile << item->get_name() << endl;
}

MatchtreeBase *MatchtreeLeaf::update_policy(list<MatchtreeItem *> &items, set<int> &vars_seen) {
    if (items.empty())
        return NULL;

    bool all_done = true;
    for (auto item : items) {
        if (!item_done(item, vars_seen)) {
            all_done = false;
            break;
        }
    }

    if (all_done) {
        applicable_items.splice(applicable_items.end(), items);
        return NULL;
    } else {
        items.splice(items.end(), applicable_items);
        return new MatchtreeSwitch(items, vars_seen);
    }
}

void MatchtreeLeaf::generate_consistent_items(const PR2State &curr, vector<MatchtreeItem *> &items, bool only_if_relevant) {
    for (auto item : applicable_items)
        if (!only_if_relevant || item->is_relevant(curr))
            items.push_back(item);
}

void MatchtreeLeaf::generate_entailed_items(const PR2State &, vector<MatchtreeItem *> &items) {
    for (auto item : applicable_items)
        items.push_back(item);
}

bool MatchtreeLeaf::check_consistent_match(const PR2State &) {
    return (applicable_items.size() > 0);
}

bool MatchtreeLeaf::check_entailed_match(const PR2State &) {
    return (applicable_items.size() > 0);
}


/*********
 * Empty *
 *********/

void MatchtreeEmpty::dump(string indent) const {
    cout << indent << "<empty>" << endl;
}

void MatchtreeEmpty::generate_cpp_input(ofstream &outfile) const {
    outfile << "check 0" << endl;
}

MatchtreeBase *MatchtreeEmpty::update_policy(list<MatchtreeItem *> &items, set<int> &vars_seen) {
    if (items.empty())
        return NULL;

    bool all_done = true;
    for (auto item : items) {
        if (!item_done(item, vars_seen)) {
            all_done = false;
            break;
        }
    }

    if (all_done)
        return new MatchtreeLeaf(items);
    else
        return new MatchtreeSwitch(items, vars_seen);
}
