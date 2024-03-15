#include <string>
#include <vector>
#include <set>
#include <sstream>

#include "Tuple.h"
#include "Scheme.h"

#pragma once
using namespace std;

// Relations are used as operands in relational operations (select, project, and rename)
class Relation {

private:

    string name;
    Scheme scheme;
    set<Tuple> tuples;

public:

    Relation(const string& name, const Scheme& scheme)
            : name(name), scheme(scheme) { }

    void addTuple(const Tuple& tuple) {
        tuples.insert(tuple);
    }

    string get_name() {
        return name;
    }

    Scheme get_scheme() {
        return scheme;
    }

    set<Tuple> get_tuple_set() {
        return tuples;
    }

    string toString() {
        stringstream out;
        // add code to print the Tuples, one per line
        for (auto& cur_tuple : tuples) {
            out << cur_tuple.toString(scheme) << endl;
        }
        return out.str();
    }

    // Select operation that compares the values in 1 column to the given value
    Relation select(int index, const string& value) {
        Relation result(name, scheme);
        // add tuples to the result if they meet the condition
        // for each tuple in the relation
        for (auto& cur_tuple : tuples) {
            // if the value at the given index equals the given value
            if (cur_tuple.at(index) == value) {
                // add the tuple to the result
                result.addTuple(cur_tuple);
            }
        }
        return result;
    }
    // Select operation that compares the values of 2 columns
    Relation select(int index_1, int index_2) {
        Relation result(name, scheme);
        // add tuples to the result if they meet the condition
        // for each tuple in the relation
        for (auto& cur_tuple : tuples) {
            // In the same tuple, if the value in column 1 equals the value in column 2
            if (cur_tuple.at(index_1) == cur_tuple.at(index_2)) {
                // add the tuple to the result
                result.addTuple(cur_tuple);
            }
        }
        return result;
    }

    // project operation that only keeps the given columns from the old relation
    Relation project(vector<int> columns) {
        vector<string> projected_scheme_values;
        // extract the values only at the columns specified
        for (int index : columns) {
            projected_scheme_values.push_back(scheme.at(index));
        }

        // create the projected scheme and an empty result relation
        Scheme projected_scheme(projected_scheme_values);
        Relation result(name, projected_scheme);
        // for every tuple
        for (auto& cur_tuple : tuples) {
            vector<string> projected_tuple_values;
            // extract the values only at the columns specified
            for (int index : columns) {
                projected_tuple_values.push_back(cur_tuple.at(index));
            }
            // create a new tuple
            Tuple projected_tuple(projected_tuple_values);
            // add tuple to the result
            result.addTuple(projected_tuple);
        }
        return result;
    }

    // implement rename operation
    Relation rename(Scheme renamed_scheme) {
        Relation result(name, renamed_scheme);
        for (auto& cur_tuple : tuples) {
            // add all the tuples to the result
            result.addTuple(cur_tuple);
        }
        return result;
    }

};