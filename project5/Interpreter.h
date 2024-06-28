#include <string>
#include <vector>
//#include <set>
#include <sstream>
#include <map>
#include <stack>


#include "Tuple.h"
#include "Scheme.h"
#include "Relation.h"
#include "Node.h"
#include "Graph.h"
#include "Database.h"

#include "../project2/DatalogProgram.h"

#pragma once
using namespace std;


class Interpreter {

private:
    DatalogProgram datalog_input;
    Database database;

public:
    //-----------------------------Main Functions-------------------------------------------
    // constructor
    Interpreter(DatalogProgram datalog_in) : datalog_input(datalog_in) {}

    void interpret() {
        interpret_schemes_into_empty_relations();
        interpret_facts_into_tuples_and_add_to_associated_relations();
        evaluate_all_SCCs();
        evaluate_all_queries();
    }

    //-----------------------------Interpreting Schemes and Facts----------------------------
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

    //-----------------------------Basic Algorithm of Evaluating Rules--------------------------------------
    int evaluate_all_given_rules(vector<Rule> given_rules) {
        // Track the number of times this while loop takes (used when printing out the Rule Evaluation)
        int evaluate_rules_loop_counter = 0; // Rule Output helper
        while (true) {
            evaluate_rules_loop_counter++; // Rule Output helper
            // start the while loop with this flag set to false
            bool new_tuple_was_added = false;

            // evaluate ALL the rules, checking if any new tuples were added
            for (Rule current_rule : given_rules) {
                // count the tuples before
                Relation rule_relation_before = get_rule_relation_from_database(current_rule);
                size_t tuple_count_before = rule_relation_before.get_tuple_set().size();

                // evaluate the rule
                cout << current_rule.to_string() << "." << endl; // Rule Evaluation Output
                evaluate_rule(current_rule);

                // count the tuples after
                Relation rule_relation_after = get_rule_relation_from_database(current_rule);
                size_t tuple_count_after = rule_relation_after.get_tuple_set().size();

                // check if new tuples were added
                if (tuple_count_before != tuple_count_after) {
                    new_tuple_was_added = true;
                }
            }

            // if  no new tuples were added, the fixed-point algorithm is complete
            if (!new_tuple_was_added) {
                break;
            }
            // otherwise, continue to the next while loop
        }
        return evaluate_rules_loop_counter;
    }

    void evaluate_rule(Rule given_rule) {
        Predicate head_predicate = given_rule.get_head_predicate();
        vector<Predicate> body_predicate_list = given_rule.get_body_predicate_list();

        //------Steps 1 and 2---------
        // Evaluate the initial body predicate because of the join operation in the for loop
        Relation result_rule_relation = evaluate_query(body_predicate_list.front());
        // remove the initial body predicate
        body_predicate_list.erase(body_predicate_list.begin());

        for (auto& rule_predicate : body_predicate_list) {
            // Step 1: Evaluate the predicates on the right-hand side of the rule
            Relation result_body_predicate_relation = evaluate_query(rule_predicate);
            // Step 2: Join the relations that result
            result_rule_relation = result_rule_relation.join(result_body_predicate_relation);
        }

        // Step 3: Project the columns that appear in the head predicate
        // find the head column index values that match the head predicate (in the order of the head predicate)
        vector<int> columns_to_project = {};
        for (Parameter& head_column_parameter : head_predicate.get_parameter_list()) {
            string head_column_name = head_column_parameter.to_string();
            int body_column_index = 0;
            for (string& body_column_name : result_rule_relation.get_scheme()) {
                if (head_column_name == body_column_name) {
                    columns_to_project.push_back(body_column_index);
                }
                body_column_index++;
            }
        }
        // project the found columns from the relation of joined body predicate relations
        result_rule_relation = result_rule_relation.project(columns_to_project);

        // Step 4: Rename the relation to make it union-compatible
        // retrieve the original scheme in of the associated relation in the database
        Relation database_relation = get_rule_relation_from_database(given_rule);
        Scheme original_database_relation_scheme = database_relation.get_scheme();

        // rename the relation
        result_rule_relation = result_rule_relation.rename(original_database_relation_scheme);
        // set the name of the relation to the name of the head predicate
        // This name is the same name given in the Rules and the Schemes
        // allows this relation to be unionized with the corresponding relation in the database
        string original_database_relation_name = head_predicate.get_name();
        result_rule_relation.set_name(original_database_relation_name); // set name of relation to the rule's head predicate's name

        // Step 5: Union with the relation in the database
        // if there is a relation in the database with the same scheme, union the relations
        // This is intentionally caused when repeatedly evaluating the same ruled
//        Relation database_relation = database.get_relation(result_rule_relation_name);
        database.unionize_existing_relation_with_new_relation(result_rule_relation);
    }

    Relation get_rule_relation_from_database(Rule given_rule) {
        string rule_relation_name = given_rule.get_head_predicate().get_name();
        Relation retrieved_relation = database.get_relation(rule_relation_name);
        return retrieved_relation;
    }

    // ----------------------------Evaluating Rules based on SCCs (optimized rule evaluation)---------------------
    static Graph makeGraph(const vector<Rule>& rules, string graph_direction) {
        Graph graph(rules.size());
        if (graph_direction != "forward" and graph_direction != "reverse") {
            cout << "ERROR: given graph direction (" << graph_direction <<  ") is invalid" << endl;
        }
        // This loop identifies the 'from' end of possible edges in the graph.
        int node_ID_from = 0;
        for (Rule cur_rule_from : rules) {
//            cout << "from rule R" << node_ID_from << ": " << cur_rule_from.to_string() << endl; // DEBUG

            // loops over the predicates in the body of the current rule.
            for (Predicate body_predicate_from : cur_rule_from.get_body_predicate_list()) {
//                cout << "from body predicate: " << body_predicate_from.to_string() << endl; // DEBUG

                // This loop identifies the 'to' end of possible edges in the graph
                int node_ID_to = 0;
                for (Rule cur_rule_to : rules) {
//                    cout << "to rule R" << node_ID_to << ": " << cur_rule_to.to_string() << endl; // DEBUG
                    if (body_predicate_from.get_name() == cur_rule_to.get_head_predicate().get_name()) {
                        if (graph_direction == "reverse") {
                            graph.addEdge(node_ID_to, node_ID_from);
                        } else {
                            graph.addEdge(node_ID_from, node_ID_to);
//                            cout << "dependency found: (R" << node_ID_from << ",R" << node_ID_to << ")" << endl; // DEBUG
                        }
                    }
                    node_ID_to++;
                }
            }
            node_ID_from++;
        }
        return graph;
    }

    static stack<int> DFS_forest_post_order(Graph& reverse_graph) {
        stack<int> post_order;
        for (auto node_pair : reverse_graph.get_nodes_map()) {
            post_order = DFS_tree_post_order(reverse_graph, node_pair.first, post_order);
        }

// DEBUG BLOCK
//        cout << "post order complete" << endl;
//        while (post_order.size() > 0) {
//            cout << post_order.top() << endl;
//            post_order.pop();
//        }
        return post_order;
    }

    static stack<int> DFS_tree_post_order(Graph& reverse_graph, int start_node_ID, stack<int> post_order) {
        // base case: the node has already been explored
        map<int, Node> nodes_map = reverse_graph.get_nodes_map();
        Node start_node = nodes_map.at(start_node_ID);
        if (start_node.has_been_visited()) {
//            cout << "This node has already been explored. Moving on...\n" << endl; // DEBUG
            return post_order;
        }
        // else: mark this node as visited
        reverse_graph.mark_node_as_visited(start_node_ID);


        // recursive case: the node has not been explored
            // for every unexplored adjacent node, explore it
        for (int adjacent_node_ID : start_node.get_adjacent_node_IDs()) {
            Node adjacent_node = nodes_map.at(adjacent_node_ID);
            if (adjacent_node.has_been_visited()) {
                continue;
            } else {
                post_order = DFS_tree_post_order(reverse_graph, adjacent_node_ID, post_order);
            }
        }
        // after all adjacent nodes of the start node have been pushed onto the stack...
        // push this node ID onto the stack (this is the definition of post order)
        post_order.push(start_node_ID);
        return post_order;
    }

    static vector<set<int>> DFS_forest_search_for_SCCs(Graph& forward_graph, stack<int> post_order) {
        // SCC stands for Strongly Connected Component

        // reset all nodes in graph to unvisited
        forward_graph.reset_all_markers_to_unvisited();

        // from the given post_order, do a DFS_forest algorithm to find the SCCs
        vector<set<int>> list_of_SCCs;
        while(not post_order.empty()) {
            int cur_node_ID = post_order.top();
            post_order.pop();
            // search for nodes in the SCC
            set<int> found_SCC = DFS_find_SCC(forward_graph, cur_node_ID, {});
            if (found_SCC.empty()) {
                continue;
            } else {
                list_of_SCCs.push_back(found_SCC);
            }
        }
        return list_of_SCCs;
    }

    static set<int> DFS_find_SCC(Graph& forward_graph, int start_node_ID, set<int> nodes_in_SCC) {
        // base case: the node has already been explored
        map<int, Node> nodes_map = forward_graph.get_nodes_map();
        Node start_node = nodes_map.at(start_node_ID);
        if (start_node.has_been_visited()) {
            return nodes_in_SCC;
        }
        // else: mark this node as visited
        forward_graph.mark_node_as_visited(start_node_ID);
        // add this node to the SCC (pre-order: add the parent node before any of its children)
        nodes_in_SCC.insert(start_node_ID);

        // recursive case: the node has not been explored
            // for every unexplored adjacent node, explore it
        for (int adjacent_node_ID : start_node.get_adjacent_node_IDs()) {
            Node adjacent_node = nodes_map.at(adjacent_node_ID);
            if (adjacent_node.has_been_visited()) {
                continue;
            } else {
                nodes_in_SCC = DFS_find_SCC(forward_graph, adjacent_node_ID, nodes_in_SCC);
            }
        }
        return nodes_in_SCC;
    }

    vector<set<int>> find_all_SCCs_from_datalog() {
        vector<Rule> rules = datalog_input.get_rule_list();
        // Finding SCCs Algorithm
        // Step 1: create forward dependency graph
        Graph forward_dependency_graph = makeGraph(rules, "forward");
        cout << "Dependency Graph" << endl; // Dependency Graph Output
        cout << forward_dependency_graph.toString() << endl; // Dependency Graph Output
        // step 2: create reverse dependency graph
        Graph reverse_dependency_graph = makeGraph(rules, "reverse");


        // step 3: get the post order path of the DFS_forest traversing through reverse graph (in numerical order)
        stack<int> post_order = DFS_forest_post_order(reverse_dependency_graph);

        // step 4: search for SCCs by doing a DFS_forest on the forward graph
        // (in post order from step 3 , from greatest to least (thus the stack))
        // Note: the order the nodes are visited is the order within an SCC (pre-order)
        return DFS_forest_search_for_SCCs(forward_dependency_graph, post_order);
    }

    void evaluate_all_SCCs() {

        // set up variables from other classes
        vector<Rule> rules = datalog_input.get_rule_list();
        map<int, Node> nodes_map = makeGraph(rules, "forward").get_nodes_map();

        // find all the SCCs
        vector<set<int>> list_of_SCCs = find_all_SCCs_from_datalog();

        // iterate over each SCC separately and evaluate the rules inside
        // if the SCC only contains 1 rule node, and that rule node is NOT dependent on itself, then evaluate it only once
        cout << "Rule Evaluation" << endl; // Rule Evaluation Output
        for (set<int>& cur_SCC: list_of_SCCs) {
            cout << SCC_to_string(cur_SCC, true); // Rule Evaluation Output
            if (cur_SCC.size() == 1) {
                int SCC_solo_node_ID = *cur_SCC.begin();
                Node SCC_solo_node = nodes_map.at(SCC_solo_node_ID);
                set<int> adjacent_nodes_set = SCC_solo_node.get_adjacent_node_IDs();
                if (adjacent_nodes_set.find(SCC_solo_node_ID) == adjacent_nodes_set.end()) {
                    Rule SCC_solo_rule = rules.at(SCC_solo_node_ID);
                    cout << SCC_solo_rule.to_string() << "." << endl; // Rule Evaluation Output
                    evaluate_rule(SCC_solo_rule);
                    cout << "1 passes: " << SCC_to_string(cur_SCC, false);
                    continue;
                }
                // else, it is a single node SCC that is dependent on itself
                // It must be evaluated like multi-node SCCs using fixed-point algorithm
            }
            // else, perform the fixed point algorithm on the SCC

            // retrieve the sub-list of rules for this SCC
            vector<Rule> SCC_rules;
            for (int SCC_node_ID : cur_SCC) {
                Rule SCC_rule = rules.at(SCC_node_ID);
                SCC_rules.push_back(SCC_rule);
            }
            int passes_taken = evaluate_all_given_rules(SCC_rules);
            // Rule Evaluation Output
            cout << passes_taken << " passes: " << SCC_to_string(cur_SCC, false);
        }
    }

    string SCC_to_string(set<int> cur_SCC, bool include_header) {
        stringstream out;
        if (include_header) {
            out << "SCC: ";
        }
        size_t loop_counter = 0;
        for (int SCC_node_ID : cur_SCC) {
            out << "R" << SCC_node_ID;
            if (loop_counter + 1 < cur_SCC.size()) {
                out << ", ";
            }
            loop_counter++;
        }
        out << "\n";
        return out.str();
    }

    //-----------------------------Interpreting Queries--------------------------------------
    void evaluate_all_queries() {
        cout << "\nQuery Evaluation" << endl; // Query Output
        for (auto& query_predicate : datalog_input.get_query_list()) {
            Relation result_query_relation = evaluate_query(query_predicate);
            print_query_result(result_query_relation, query_predicate);
        }
    }

    Relation evaluate_query(Predicate query_predicate) {
        // Can also evaluate body predicates from a rule
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
        return result_relation;
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

    // ----------------------------Misc. Helper Functions------------------------------------
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
// 3. Evaluate Rules
// See website
// 4. Evaluate Queries
// See website


// a full relation needs:
// a name (retrieved from the scheme_predicate's name)
// a scheme (converted from the scheme_predicate's parameter list)
// tuples (converted from the fact_predicate's parameter list)
// schemes and facts contain data
// The scheme describes the data (column header of a table)
// the associated facts are the data values (column values of a table)
// Relations are created to store the data from schemes and facts
// the scheme data is stored in a scheme object
// The facts are stored in tuples

// Rules are logic based on the schemes and facts
// They generate new tuples for the relations generated by the schemes and facts
// New tuples are added to the existing relations
// These new tuples are considered rule tuples, not fact tuples

// queries are actions that manipulate the data
