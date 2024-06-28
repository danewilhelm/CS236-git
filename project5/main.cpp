#include "Interpreter.h"
#include "../project1/Scanner.h"
#include "../project2/Parser.h"

#include <fstream>
#include <iostream>
#include <stack>

using namespace std;

void test_node_to_string() {
    Node node;
    node.addEdge(4);
    node.addEdge(8);
    node.addEdge(2);
    cout << node.toString() << endl;
}

void test_graph_to_string() {
    Graph graph(3);
    graph.addEdge(1,2);
    graph.addEdge(1,0);
    graph.addEdge(0,1);
    graph.addEdge(1,1);
    cout << graph.toString();
}

vector<Rule> generate_example_rules_1() {
    // predicate names for fake rules
    // first is name for head predicate
    // second is names for body predicates
    pair<string,vector<string>> ruleNames[] = {
            { "A", { "B", "C" } },
            { "B", { "A", "D" } },
            { "B", { "B" } },
            { "E", { "F", "G" } },
            { "E", { "E", "F" } },
    };

    vector<Rule> rules;

    for (auto& rulePair : ruleNames) {
        string headName = rulePair.first;
        Predicate headPredicate(headName, {});

        vector<string> bodyNames = rulePair.second;
        vector<Predicate> bodyPredicates;
        for (auto& bodyName : bodyNames) {
            Predicate bodyPredicate(bodyName, {});
            bodyPredicates.push_back(bodyPredicate);
        }
        Rule generatedRule(headPredicate, bodyPredicates);
        rules.push_back(generatedRule);
    }
    return rules;
}

Graph test_interpreter_make_forward_graph(vector<Rule> rules) {
    Graph forward_graph = Interpreter::makeGraph(rules, "forward");
//    cout << forward_graph.toString();
    return forward_graph;
}



Graph test_interpreter_make_reverse_graph(vector<Rule> rules) {
    Graph reverse_graph = Interpreter::makeGraph(rules, "reverse");
//    cout << reverse_graph.toString();
    return reverse_graph;
}

stack<int> test_interpreter_DFS_post_order() {
    Graph reverse_graph = test_interpreter_make_reverse_graph(generate_example_rules_1());
    stack<int> post_order = Interpreter::DFS_forest_post_order(reverse_graph);
    return post_order;
}

vector<set<int>> test_interpreter_DFS_search_for_SCCs() {

    vector<Rule> rules = generate_example_rules_1();
    // step 1: create forward dependency graph
    Graph forward_graph = test_interpreter_make_forward_graph(rules);
    // step 2: create reverse dependency graph
    // step 3: get the post order path of the DFS_forest traversing through reverse graph (in numerical order)
    stack<int> post_order = test_interpreter_DFS_post_order();
    // step 4: search for SCCs by doing a DFS_forest on the forward graph
    // (in post order from step 3 , from greatest to least (thus the stack))
    // Note: the order the nodes are visited is the order within an SCC (pre-order)
    vector<set<int>> list_of_SCCs = Interpreter::DFS_forest_search_for_SCCs(forward_graph, post_order);
//    for (vector<int> SCC)
//    cout << list_of_SCCs << endl;
    return list_of_SCCs;
};


DatalogProgram scan_and_parse(int argc, char* argv[]) {
    // declare variables
    string input_filename = argv[1];
    ifstream input_file;
    input_file.open(input_filename);
    stringstream ss;

    // processing file into string
    ss << input_file.rdbuf();
    string input_string = ss.str();
    input_file.close();

    // scanning string into a vector of tokens
    Scanner s = Scanner(input_string);
    s.scan_all_tokens();
    vector<Token> token_list = s.get_token_list();

    // parsing tokens into a datalog object
    Parser p = Parser(token_list);
    DatalogProgram test_datalog_program = p.parse();
    return test_datalog_program;
}

void interpret_input_file(int argc, char* argv[]) {
    DatalogProgram input_datalog_program = scan_and_parse(argc, argv);
    Interpreter new_interpreter(input_datalog_program);
    new_interpreter.interpret();
//    cout << "end of interpreting" << endl;
}



int main(int argc, char* argv[]) {
//    test_node_to_string();
//    test_graph_to_string();
//    test_interpreter_make_forward_graph(generate_example_rules_1());
//    test_interpreter_make_reverse_graph(generate_example_rules_1());
//    test_interpreter_DFS_post_order();
//    test_interpreter_DFS_search_for_SCCs();
    interpret_input_file(argc, argv);
}


// ----------TODO-----------
// DONE: Build the dependency graph.
// DONE: Build the reverse dependency graph.
// INCOMPLETE: Run DFS-Forest on the reverse dependency graph.
// INCOMPLETE: Find the strongly connected components (SCCs).
// INCOMPLETE: Evaluate the rules in each component.