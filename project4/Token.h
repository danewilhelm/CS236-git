#pragma once
#include <string>
#include <sstream>
using namespace std;

enum TokenType {
    COMMA,
    PERIOD,
    Q_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COLON_DASH,
    MULTIPLY,
    ADD,
    SCHEMES,
    FACTS,
    RULES,
    QUERIES,
    ID,
    STRING,
    COMMENT,
    UNDEFINED,
    EOFILE,
};

class Token {
    private:
        TokenType type;
        string value;
        int line;

    public:
        // constructor
        Token(TokenType type, string value, int line) : type(type), value(value), line(line) {}

        // converts token attributes into a string
        // From Lab 1
        string toString() const {
            stringstream out;
            out << "(" << typeName(type) << "," << "\"" << value << "\"" << "," << line << ")";
            return out.str();
        }

        // converts a token type integer to a string
        // from Lab1
        string typeName(TokenType type_var) const {
            // return the correct string for each TokenType value
            if (type_var == 0)
                return "COMMA";
            if (type_var == 1)
                return "PERIOD";
            if (type_var == 2)
                return "Q_MARK";
            if (type_var == 3)
                return "LEFT_PAREN";
            if (type_var == 4)
                return "RIGHT_PAREN";
            if (type_var == 5)
                return "COLON";
            if (type_var == 6)
                return "COLON_DASH";
            if (type_var == 7)
                return "MULTIPLY";
            if (type_var == 8)
                return "ADD";
            if (type_var == 9)
                return "SCHEMES";
            if (type_var == 10)
                return "FACTS";
            if (type_var == 11)
                return "RULES";
            if (type_var == 12)
                return "QUERIES";
            if (type_var == 13)
                return "ID";
            if (type_var == 14)
                return "STRING";
            if (type_var == 15)
                return "COMMENT";
            if (type_var == 16)
                return "UNDEFINED";
            if (type_var == 17)
                return "EOF";
            // else, error case
            return "INTERNAL ERROR: TypeName() did not detect a proper typename";
        }


//-----------Test Functions---------------------//
    TokenType get_token_type() const{
            return type;
        }

    string get_token_value() const{
        return value;
    }


};