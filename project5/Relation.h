#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iostream>
#include <map>




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

    void set_name(string new_name) {
        name = new_name;
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
            // if the tuple is empty, don't print it
            // occurs when a query contains a fact, so all the tuple columns are removed, but not the tuple itself
            if (cur_tuple.size() > 0) {
                out << "  " << cur_tuple.toString(scheme) << endl;
            }
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

    static bool are_tuples_joinable(const Scheme& leftScheme, const Scheme& rightScheme, const Tuple& leftTuple, const Tuple& rightTuple) {
        bool tuples_are_joinable = true;

        for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
            const string& leftName = leftScheme.at(leftIndex);
            const string& leftValue = leftTuple.at(leftIndex);
//            cout << "left name: " << leftName << " value: " << leftValue << endl; // DEBUG
            // nested for loop
            for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
                const string& rightName = rightScheme.at(rightIndex);
                const string& rightValue = rightTuple.at(rightIndex);
//                cout << "right name: " << rightName << " value: " << rightValue << endl; // DEBUG
                // are_tuples_joinable logic
                // if there are any matching scheme columns
                    // if the tuple values in the matching columns don't match, the tuples are not joinable
                if (leftName == rightName) {
                    if (leftValue != rightValue) {
                        tuples_are_joinable = false;
                    }
                }

            }
        }
        return tuples_are_joinable;
    }

//  joinSchemes: combines the schemes for the left and right relations into single scheme for the result relation.
    Scheme join_schemes(Scheme& left_scheme, Scheme& right_scheme) {
        // ----------join schemes------------
        // set up the initial joined scheme and the scheme value tracker
        // We can immediately add the left scheme values without checking for duplicates
        // (there are never duplicates within the same scheme)
        Scheme joined_scheme_result = left_scheme;
        set<string> seen_scheme_values;
        for (string& left_scheme_value : left_scheme) {
            seen_scheme_values.insert(left_scheme_value);
        }

        // for each right scheme value, if it's NOT a duplicate, add it to the joined scheme
        for (string& right_scheme_value : right_scheme) {
            if (seen_scheme_values.find(right_scheme_value) == seen_scheme_values.end()) {
                seen_scheme_values.insert(right_scheme_value);
                joined_scheme_result.push_back(right_scheme_value);
            }
        }
        return joined_scheme_result;
    }

//  joinTuples: combines tuples from the left and right relations into single tuple for the result relation.
    Tuple join_tuples(const Scheme& left_scheme, const Scheme& right_scheme, const Tuple& left_tuple, const Tuple& right_tuple) {
        Tuple joined_tuple = left_tuple;
        bool is_duplicate_column = false;
        for (size_t right_index = 0; right_index < right_scheme.size(); right_index++) {
            string right_name = right_scheme.at(right_index);
            for (auto& left_name : left_scheme) {
                if (left_name == right_name) {
                    is_duplicate_column = true;
                }
            }
            string right_value = right_tuple.at(right_index);
            if (!is_duplicate_column) {
                joined_tuple.push_back(right_value);
            }
            is_duplicate_column = false;
        }
        return joined_tuple;
    }

//  Your 'join' function can call these functions as well as the 'are_tuples_joinable' function to produce the relation that results from the join.
    Relation join(Relation& right) {
        Relation& left = *this;

        // join the schemes
        Scheme left_scheme = left.get_scheme();
        Scheme right_scheme = right.get_scheme();
        Scheme joined_scheme = join_schemes(left_scheme, right_scheme);

        // set up empty relation
        Relation joined_relation("unnamed joined relation", joined_scheme); // QUESTION: IDK what the name of the joined relation should be

        // join the tuples
        for (auto& left_tuple : left.tuples) {
            for (auto& right_tuple : right.tuples) {
//                cout << "left tuple: " << left_tuple.toString(left.scheme) << endl; // DEBUG
//                cout << "right tuple: " << right_tuple.toString(right.scheme) << endl; // DEBUG
                if (are_tuples_joinable(left_scheme, right_scheme, left_tuple, right_tuple)) {
                    Tuple joined_tuple = join_tuples(left_scheme, right_scheme, left_tuple, right_tuple);
                    joined_relation.addTuple(joined_tuple);
                }
            }
        }
        return joined_relation;
    }

    Relation unionize(Relation& right) {
        Relation& left = *this;
        if (left.get_scheme() != right.get_scheme()) {
            cout << "ERROR: relations are not union compatible" << endl;
        }

        Relation combined_relation = left;
        Scheme relation_scheme = combined_relation.get_scheme();
        for (Tuple right_tuple : right.get_tuple_set()) {
//            combined_relation.tuples.insert(right_tuple);
            bool insert_succeeded = combined_relation.tuples.insert(right_tuple).second; // DEBUG
            if (insert_succeeded) {
                // Rule Output
                cout << "  " << right_tuple.toString(relation_scheme) << endl;
            }
        }
        return combined_relation;
    }
};