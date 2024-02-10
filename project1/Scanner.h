#pragma once
#include <string>
#include "Token.h"
#include <cctype>
#include <vector>
using namespace std;

class Scanner {
    private:
        string input;
        int cur_line;
        vector<Token> token_list;

    public:
        // insert other variables to track in constructor
        Scanner(const string& input) : input(input), cur_line(1) {}


        Token scanToken() {
//---------------Initial Checks----------------------------------//
//            cout << "DEBUG MAIN 0: initial checks.   current input: " << input << endl;
            if (is_EOF())
                return EOF_token(cur_line);

//            cout << "DEBUG MAIN 0.1" << endl;

            // removes any whitespace before scanning a token
            while (isspace(input.at(0))) {
//                cout << "checking for new line while removing whitespace" << endl;
                check_for_new_line();
                input = input.substr(1);
                if (is_EOF())
                    return EOF_token(cur_line);
            }
//            cout << "DEBUG MAIN 0.2" << endl;

//--------------Simple Tokens----------------------------------//
//            cout << "DEBUG MAIN 1: simple tokens" << endl;

            if (input_begins_with(','))
                return COMMA_token(cur_line);

            if (input_begins_with('.'))
                return PERIOD_token(cur_line);

            if (input_begins_with('?'))
                return Q_MARK_token(cur_line);

            if (input_begins_with('('))
                return LEFT_PAREN_token(cur_line);

            if (input_begins_with(')'))
                return RIGHT_PAREN_token(cur_line);

            if (input_begins_with(':')) {
                if (not is_EOF() and input.at(1) == '-')
                    return COLON_DASH_token(cur_line);
                else
                    return COLON_token(cur_line);
            }

            if (input_begins_with('*'))
                return MULTIPLY_token(cur_line);

            if (input_begins_with('+'))
                return ADD_token(cur_line);

//-----------------Complex Tokens----------------------------//
//            cout << "DEBUG MAIN 2: complex tokens" << endl;

            if (isalpha(input.at(0))) {
//                cout << "input.at(0) --> " <<  input.at(0) <<  " was detected as a letter" << endl;

                // CASE: input starts with a letter
                    // the token must be SCHEMES, FACTS, RULES, QUERIES, or ID

                // gather the string and create the correct token
                string ID_value;
                ID_value += input.at(0);
                advance_input(1);
                // 1. if the string spelled out Schemes, Facts, Rules, or Queries
                    // create and return the token
                // 2. otherwise, continue gathering alphanumeric characters
                // 3. the input gave a non-alphanumeric character
                    // create and return ID token
                while (true) {
//                    cout << "DEBUG: new loop: current input is \"" << input << "\"" << endl;

                    // base case: reached EOF
                    // base case: reached a non-alphanumeric character
                    if (is_EOF() or not (isalpha(input.at(0)) or isdigit(input.at(0)))) { // base case
                        if (ID_value == "Schemes") {
                            return SCHEMES_token(cur_line);
                        } if (ID_value == "Facts") {
                            return FACTS_token(cur_line);
                        } if (ID_value == "Rules") {
                            return RULES_token(cur_line);
                        } if (ID_value == "Queries") {
                            return QUERIES_token(cur_line);
                        }
                        // else, it is a normal ID token
                        return ID_token(cur_line, ID_value);
                    } else { // looping case
                        ID_value += input.at(0);
                        advance_input(1);
                    }
                }
            }
//            cout << "DEBUG MAIN 3: after ID tokens, before STRING tokens" << endl;

            if (input_begins_with('\'')) {
//                cout << "input.at(0) --> " <<  input.at(0) <<  " was detected as an apostrophe" << endl;

                // CASE: input begins with an apostrophe
                    // the token must be STRING or UNDEFINED

                // prep for while loop
                string string_value;
                string_value += input.at(0);
                advance_input(1);
                int string_start_line = cur_line;

                // while true
                while (true) {
                    // if the loop reached the end of file
                    if (is_EOF()) {
                        // create and return UNDEFINED token
                        return UNDEFINED_token(string_start_line, string_value);
                    }
                    // if current character is apostrophe
                    if (input_begins_with('\'')) {
                        string_value += input.at(0);
                        advance_input(1);
                        // if next character is apostrophe (double apostrophe)
                        if (input_begins_with('\'')) {
                            // include double apostrophe and move to new characters
                            string_value += input.at(0);
                            advance_input(1);
                            continue;
                        } else {
                            // create and return STRING token
                            return STRING_token(string_start_line, string_value);
                        }
                    }
                    // if current character is new line, update counter
//                    cout << "checking for new line within a string token" << endl;
                    check_for_new_line();

                    // no token was found yet, so move to the next one
                    string_value += input.at(0);
                    advance_input(1);
                }
            }

            if (input_begins_with('#')) {
//                cout << "input.at(0) --> " <<  input.at(0) <<  " was detected as an hashtag" << endl;

                string comment_value;
                comment_value += input.at(0);
                advance_input(1);
                int comment_start_line = cur_line;

                // if next character starts with |, the comment is a block comment
                if (input_begins_with('|')) {
//                    cout << "block comment was detected" << endl;

                    // while true
                    while (true) {
                        // if EOF is reached
                        if (is_EOF()) {
                            // create and return UNDEFINED token
//                            return UNDEFINED_token(comment_start_line, comment_value);
                        }

//                        cout << "new iteration of loop. input.at(0): " << input.at(0) << endl;

                        // if current character is new line, update counter
//                        cout << "checking for new line inside a block comment" << endl;
                        check_for_new_line();

                        // if | is reached
                        if (input_begins_with('|')) {
                            comment_value += input.at(0);
                            advance_input(1);
                            // if end of block comment is reached
                            if (input_begins_with('#')) {
                                // create and return COMMENT token
                                comment_value += "|#";
                                advance_input(2);
                                return COMMENT_token(comment_start_line, comment_value);
                            }
                        }
                        // otherwise, move to next character
                        comment_value += input.at(0);
                        advance_input(1);
                    }
                } else { // else, the comment is a single line comment
//                    cout << "single line comment was detected" << endl;
                    // while true
                    while (true) {

                        // if end of file or new line is reached
                            // create and return COMMENT token
//                        cout << "checking for new line inside a single line comment" << endl;
                        if (is_EOF()) {
                            return COMMENT_token(comment_start_line, comment_value);
                        }
                        if (check_for_new_line()) {
                            advance_input(1);
                            return COMMENT_token(comment_start_line, comment_value);
                        }
                        // else
                            // gather the comment string
                        comment_value += input.at(0);
                        advance_input(1);
                    }
                }



            }


            else {
                string temp_value;
                temp_value.push_back(input.at(0));
                advance_input(1);
                return UNDEFINED_token(cur_line, temp_value);
            }

            cout << "ERROR: did not return a token" << endl;
        }

//--------------Creating Tokens-----------------------------------//
    // remove the selected string from input
    // create and return the token

        Token append_token(Token temp_token) {
            token_list.push_back(temp_token);
            return temp_token;
        }

        // append_token(
        Token COMMA_token(int line) {
            advance_input(1);
            TokenType type = COMMA;
            return append_token(Token(type, ",", line));
        }

        Token PERIOD_token(int line) {
            advance_input(1);
            TokenType type = PERIOD;
            return append_token(Token(type, ".", line));
        }

        Token Q_MARK_token(int line) {
            advance_input(1);
            TokenType type = Q_MARK;
            return append_token(Token(type, "?", line));
        }

        Token LEFT_PAREN_token(int line) {
            advance_input(1);
            TokenType type = LEFT_PAREN;
            return append_token(Token(type, "(", line));
        }

        Token RIGHT_PAREN_token(int line) {
            advance_input(1);
            TokenType type = RIGHT_PAREN;
            return append_token(Token(type, ")", line));
        }

        Token COLON_token(int line) {
            advance_input(1);
            TokenType type = COLON;
            return append_token(Token(type, ":", line));
        }

        Token COLON_DASH_token(int line) {
            advance_input(2);
            TokenType type = COLON_DASH;
            return append_token(Token(type, ":-", line));
        }

        Token MULTIPLY_token(int line) {
            advance_input(1);
            TokenType type = MULTIPLY;
            return append_token(Token(type, "*", line));
        }

        Token ADD_token(int line) {
            advance_input(1);
            TokenType type = ADD;
            return append_token(Token(type, "+", line));
        }

        Token SCHEMES_token(int line) {
            TokenType type = SCHEMES;
            return append_token(Token(type, "Schemes", line));
        }

        Token FACTS_token(int line) {
            TokenType type = FACTS;
            return append_token(Token(type, "Facts", line));
        }

        Token RULES_token(int line) {
            TokenType type = RULES;
            return append_token(Token(type, "Rules", line));
        }

        Token QUERIES_token(int line) {
            TokenType type = QUERIES;
            return append_token(Token(type, "Queries", line));
        }

        Token ID_token(int line, string value) {
            TokenType type = ID;
            return append_token(Token(type, value, line));
        }

        Token STRING_token(int line, string value) {
            TokenType type = STRING;
            return append_token(Token(type, value, line));
        }

    Token COMMENT_token(int line, string value) {
        TokenType type = COMMENT;
        return append_token(Token(type, value, line));
    }

    Token UNDEFINED_token(int line, string value) {
        TokenType type = UNDEFINED;
        return append_token(Token(type, value, line));
    }

        Token EOF_token(int line) {
            TokenType type = EOFILE;
            return append_token(Token(type, "", line));
        }



//--------------Helper Functions---------------------------//
        // simple function to check if the first character in the input equals a certain character
        // Note: c++ errors whenever an empty character '' exists lol
        bool input_begins_with(char character) {
            if (is_EOF()) {
                return false;
            }
            return input.at(0) == character;
        }

        // simple function to advance the input a given number of characters
        void advance_input(int pos) {
            input = input.substr(pos);
        }

        bool is_EOF() {
            return input == "";
        }

        bool check_for_new_line() {
//            cout << "checking for new line" << endl;
            if (input_begins_with('\n')) {
//                cout << "new line found. updating counter" << endl;
                cur_line++;
                return true;
            }
            return false;
        }
//--------------Testing Functions-------------------------//
        string get_input() {
            return input;
        }

    vector<Token> get_token_list() {
        return token_list;
    }


};

