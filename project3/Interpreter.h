#include <string>
#include <vector>
//#include <set>
#include <sstream>
#include <map>

#include "Tuple.h"
#include "Scheme.h"
#include "Relation.h"
#include "Database.h"

#include "DatalogProgram.h"

#pragma once
using namespace std;


class Interpreter {

private:
    DatalogProgram datalog_input;
    Database database;

public:
    // constructor
    Interpreter(DatalogProgram datalog_in) : datalog_input(datalog_in) {}

    void interpret() {
        // a full relation needs:
            // a name (retrieved from the scheme_predicate's name)
            // a scheme (converted from the scheme_predicate's parameter list)
            // tuples (converted from the fact_predicate's parameter list)
        // schemes and facts contain data
        interpret_schemes_into_empty_relations();
        interpret_facts_into_tuples_and_add_to_associated_relations();
    //  queries are actions that manipulate the data
        evaluate_all_queries(); // WIP
    }

    void interpret_schemes_into_empty_relations() {
        // Initialize the scheme, then initialize the relation using the scheme
        for (auto& scheme_predicate : datalog_input.get_scheme_list()) {
            // extract the scheme values
            vector<string> scheme_values;
            for (auto& cur_parameter : scheme_predicate.get_parameter_list()) {
                scheme_values.push_back(cur_parameter.to_string());
            }

            // initialize the scheme object
            Scheme new_scheme(scheme_values);


            // initialize the relation using the scheme and the relation name
            string relation_name = scheme_predicate.get_name(); // name of scheme
            Relation new_relation(relation_name, new_scheme);

            // add the relation to the Database
            database.add_relation(new_relation);
        }
    }

    void interpret_facts_into_tuples_and_add_to_associated_relations() {
        // initialize the tuples and add them to the relation
        for (auto& cur_fact: datalog_input.get_fact_list()) {

            // extract the tuple values
            vector<string> tuple_values;
            for (auto &cur_parameter: cur_fact.get_parameter_list()) {
                tuple_values.push_back(cur_parameter.to_string());
            }

            // initialize the tuple
            Tuple new_tuple(tuple_values);

            // add the tuple to the appropriate relation in the database
            string tuple_relation_name = cur_fact.get_name();
            database.add_tuple_to_relation(new_tuple, tuple_relation_name);
        }
    }


    void evaluate_all_queries() {
        for (auto& query_predicate : datalog_input.get_query_list()) {
            evaluate_query(query_predicate);
        }
    }

    void evaluate_query(Predicate query_predicate) {
        // grab the relation from the database
        string queried_relation_name = query_predicate.get_name();
        Relation queried_relation = database.get_relation(queried_relation_name);

        // set up variable trackers
        // keep track of what variables we've already seen
        // AND the order we've seen them
        map<string, int> seen_variables_map;
        vector<string> seen_variables_order;


        // set up placeholder relation
        Relation result_relation = queried_relation;
        // go through the parameters
        int cur_index = 0;
        for (auto& parameter : query_predicate.get_parameter_list()) {
            string parameter_value = parameter.to_string();


            if (parameter.get_is_constant()) {
                result_relation = result_relation.select(cur_index, parameter_value);
                cur_index++;
                continue;
            }
            // else, the parameter value is a variable
            // if this is a new (aka untracked) variable, add it to the variable trackers
            if (seen_variables_map.find(parameter_value) == seen_variables_map.end()) {
                seen_variables_map.insert({parameter_value, cur_index});
                seen_variables_order.push_back(parameter_value);
            } else {
                // otherwise, we've seen this variable before
                // do a type 2 select operation where:
                // column 1 is the column where this variable was first seen
                // column 2 is the current cur_index column where this variable is
                int original_index = seen_variables_map.at(parameter_value);
                result_relation = result_relation.select(original_index, cur_index);
            }
            cur_index++;
        }
        //------after predicates are selected------------
        // 1. Project each column we have saved
        // The saved columns are all the variables declared in the query (not the constants)
        vector<int> projected_columns = {};
        for (auto& seen_variable : seen_variables_order) {
            int seen_variable_column_index = seen_variables_map.at(seen_variable);
            projected_columns.push_back(seen_variable_column_index);
        }
        result_relation = result_relation.project(projected_columns);

        // 2. rename columns to their corresponding variable names
        // the renamed scheme should contain:
        // the new variables from the query COMBINED with the old variables that were assigned a constant
//            for (int i = 0; i < ) // WIP
        Scheme renamed_scheme(seen_variables_order);
        result_relation = result_relation.rename(renamed_scheme);

        print_query_result(result_relation, query_predicate);
    }

    void print_query_result(Relation result_relation, Predicate query_predicate) {

        size_t relation_size = result_relation.get_tuple_set().size();

        cout << query_predicate.to_string() << "? ";
        if (relation_size > 0) {
            cout << "Yes(" << relation_size << ")" << endl;
            cout << result_relation.toString();
        } else {
            cout << "No" << endl;
        }
    }

    Database get_database() {
        return database;
    }
};




// holds a Database object AND a Datalog Program Object

// evaluate the Datalog Program using the Database
// 1. Evaluate schemes
// For each scheme in the Datalog program
// add an empty Relation to the Database
// Use the scheme name as the name of the relation and the attribute list from the scheme
// as the scheme of the relation
// 2. Evaluate facts
// For each fact in the Datalog program
// add a Tuple to a Relation
// Use the predicate name from the fact to determine the Relation to which the Tuple should be added.
// Use the values listed in the fact to provide the values for the Tuple.
// 3. Evaluate Queries
// See website


// Create an output statement for the result of evaluating each query

