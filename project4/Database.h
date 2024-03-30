#include <string>
//#include <vector>
//#include <set>
#include <sstream>
#include <map>

#include "Tuple.h"
#include "Scheme.h"
#include "Relation.h"

#pragma once
using namespace std;

class Database {

private:
    map<string, Relation> relations;

public:
    // constructor
    Database() {}

    // add relation function
    void add_relation(Relation new_relation) {
        string relation_name = new_relation.get_name();
        relations.insert({relation_name, new_relation});
    }

    Relation get_relation(string relation_name) {
        return relations.at(relation_name);
    }

    void unionize_existing_relation_with_new_relation(Relation new_relation) {
        // retrieve the database relation
        string shared_relation_name = new_relation.get_name();
        Relation old_database_relation = relations.at(shared_relation_name);
        // join the tuples from the new relation below the tuples from the database relation
        Relation unionized_relation = old_database_relation.unionize(new_relation);
        // remove the old database relation
        relations.erase(shared_relation_name);
        relations.insert({shared_relation_name, unionized_relation});
    }

    void add_tuple_to_relation(Tuple new_tuple, string relation_name) {
        relations.at(relation_name).addTuple(new_tuple);
    }

    string toString() {
        stringstream out;
        for (auto& map_pair : relations) {
            string cur_relation_name = map_pair.first;
            Relation cur_relation = map_pair.second;
            out << "relation_name: " << cur_relation_name << endl;
            out << cur_relation.toString();
        }
        return out.str();
    }
};