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

//void print_token_list(std::vector<Token> const &a) {
//    cout << "The vector elements are : ";
//
//    for(int i=0; i < a.size(); i++)
//        cout << a.at(i).toString() << ' ';
//    cout << endl;
//}

int main(int argc, char* argv[]) {
    // declar variables
    string input_filename = argv[1];
    ifstream input_file;
    input_file.open(input_filename);
    stringstream ss;

//    cout << "before processing string" << endl;
    ss << input_file.rdbuf();
    string input_string = ss.str();
    input_file.close();

//    cout << "file has been processed into a string: " << input_string << endl;
//

    Scanner s = Scanner(input_string);
    while (true) {
        Token t = s.scanToken();
        cout << t.toString() << endl;

//        print_token_list(s.get_token_list());

        if (t.get_token_type() == EOFILE) {
            cout << "Total Tokens = " << s.get_token_list().size() << endl;
            break;
        }
    }
}