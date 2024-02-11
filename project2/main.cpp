#include "../project1/Token.h"
#include "../project1/Scanner.h"

#include "Parser.h"

#include <vector>
#include <iostream>
#include <fstream>


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
}