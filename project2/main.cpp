#include "../project1/Token.h"
#include "../project1/Scanner.h"

#include "Parser.h"
#include "Parameter.h"
#include "Predicate.h"

#include <vector>
#include <iostream>
#include <fstream>


void test_predicate() {
    vector<Parameter> t_parameter_list = {Parameter("S"), Parameter("N"), Parameter("A"), Parameter("P")};
    Predicate test_predicate1 = Predicate("snap", t_parameter_list);
    cout << test_predicate1.to_string() << endl;
}

void test_rule() {
    // testing rule
    vector<Parameter> vp1 = {Parameter("X"), Parameter("Y")};
    Predicate t_head_predicate = Predicate("HasSameAddress", vp1);

    vector<Parameter> vp2 = {Parameter("A"), Parameter("X"), Parameter("B"), Parameter("C")};
    Predicate t_body_predicate1 = Predicate("snap", vp2);

    vector<Parameter> vp3 = {Parameter("D"), Parameter("Y"), Parameter("B"), Parameter("E")};
    Predicate t_body_predicate2 = Predicate("snap", vp3);

    vector<Predicate> t_body_predicate_list = {t_body_predicate1, t_body_predicate2};
    Rule t_rule = Rule(t_head_predicate, t_body_predicate_list);
    cout << t_rule.to_string() << endl;



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

    // parsing
    try {
        Parser p = Parser(token_list);
        p.parse();
    }
    catch (Token& problem_token) {
        cout << "Failure!" << endl;
        cout << "  " << problem_token.toString() << endl;
        return -1;
    }
    cout << "Success!" << endl;
    cout << "---testing classes---" << endl;

    // Testing predicate and parameter
//    test_predicate();
    test_rule();




}