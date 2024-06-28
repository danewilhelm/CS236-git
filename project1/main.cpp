#include <iostream>
#include "Token.h"
#include "Scanner.h"
#include <fstream>
using namespace std;

// Steps to complete Project 1
// 1.
    // Add scanning of other token types to 'scanToken' (Scanner.h)
    // Use separate functions for STRING, COMMENT, ID, etc.
    // Design these functions using state machines.
// 2.
    // Add code to keep track of line numbers, so each Token is created with the correct line number.
// 3.
    // Add reading from a file into the input string in the main function (main.cpp)


    // Intentional Bugs
        // when a string or a comment continues over a new line
            // the printed token toString() is separated by the new line



int main(int argc, char* argv[]) {
    // declare variables
    string input_filename = argv[1];
    ifstream input_file;
    input_file.open(input_filename);
    stringstream ss;

//  processing file into string
    ss << input_file.rdbuf();
    string input_string = ss.str();
    input_file.close();

// scanning string into a vector of tokens
    Scanner s = Scanner(input_string);
    s.scan_all_tokens();

//    while (true) {
//        Token t = s.scanToken();
//        cout << t.toString() << endl;
//
////        print_token_list(s.get_token_list());
//
//        if (t.get_token_type() == EOFILE) {
//            cout << "Total Tokens = " << s.get_token_list().size() << endl;
//            break;
//        }
//    }
}