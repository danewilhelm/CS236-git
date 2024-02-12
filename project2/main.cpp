#include "Token.h"
#include "Scanner.h"

#include "Parser.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
#include "DatalogProgram.h"

#include <vector>
#include <iostream>
#include <fstream>


void test_predicate() {
    vector<Parameter> t_parameter_list = {Parameter("S"), Parameter("N"), Parameter("A"), Parameter("P  ")};
    Predicate test_predicate1 = Predicate("snap", t_parameter_list);
    cout << test_predicate1.to_string() << endl;
}


Predicate create_predicate4(string name, string a, string b, string c, string d) {
    vector<Parameter> t_parameter_list = {Parameter(a), Parameter(b), Parameter(c), Parameter(d)};
    return Predicate(name, t_parameter_list);
}

Predicate create_predicate2(string name, string a, string b) {
    vector<Parameter> t_parameter_list = {Parameter(a), Parameter(b)};
    return Predicate(name, t_parameter_list);
}

Rule test_rule() {
    // testing rule
    vector<Parameter> vp1 = {Parameter("X"), Parameter("Y")};
    Predicate t_head_predicate = Predicate("HasSameAddress", vp1);

    vector<Parameter> vp2 = {Parameter("A"), Parameter("X"), Parameter("B"), Parameter("C")};
    Predicate t_body_predicate1 = Predicate("snap", vp2);

    vector<Parameter> vp3 = {Parameter("D"), Parameter("Y"), Parameter("B"), Parameter("E")};
    Predicate t_body_predicate2 = Predicate("snap", vp3);

    vector<Predicate> t_body_predicate_list = {t_body_predicate1, t_body_predicate2};
    Rule t_rule = Rule(t_head_predicate, t_body_predicate_list);
//    cout << t_rule.to_string() << endl;
    return t_rule;
}



void test_datalog_program() {
    vector<Predicate> t_scheme_list;
    t_scheme_list.push_back(create_predicate4("snap", "S", "N", "A", "P"));
    t_scheme_list.push_back(create_predicate2("HasSameAddress", "X", "Y"));

    vector<Predicate> t_fact_list;
    t_fact_list.push_back(create_predicate4("snap", "'12345'", "'C. Brown'", "'12 Apple'", "'555-1234'"));
    t_fact_list.push_back(create_predicate4("snap", "'33333'", "'Snoopy'", "'12 Apple'", "'555-1234'"));

    vector<Rule> t_rule_list;
    t_rule_list.push_back(test_rule());

    vector<Predicate> t_query_list;
    t_query_list.push_back(create_predicate2("hasSameAddress", "'Snoopy'", "Who"));

    DatalogProgram test_datalog = DatalogProgram(t_scheme_list, t_fact_list, t_rule_list, t_query_list);
    cout << test_datalog.to_string_datalog() << endl;
}

void test_clear() {
    vector<Parameter> t_parameter_list = {Parameter("S"), Parameter("N"), Parameter("A"), Parameter("P")};
    Predicate test_predicate1 = Predicate("snap", t_parameter_list);

    vector<Predicate> predicate_list_1 = {test_predicate1};
    vector<Predicate> predicate_list_2 = {test_predicate1};
    predicate_list_1.clear();
    cout << "after clearing list1, list2 has size: " << predicate_list_2.size() << endl;
    cout << "after clearing list1, list2 has predicate: " << predicate_list_2.at(0).to_string() << endl;
}

using namespace std;

int main(int argc, char* argv[]) {
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

    // testing
//    cout << "---testing classes---" << endl;

// calling testing functions
//    test_predicate();
//    test_rule();
//    test_datalog_program();
//    test_clear();



}