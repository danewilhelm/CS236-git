#include "../project1/Token.h"
#include <stdexcept>
#include <iostream>
#include <vector>


using namespace std;

class Parser {
    private:
        vector<Token> tokens;
    public:
//---------------constructor------------------------------------//
        Parser(const vector<Token>& tokens) : tokens(tokens) {}

//---------------parser functions for Grammar Rules-------------------------//
    //-------start symbol--------------------------//
        void datalogProgram() {
            if (tokenType() == SCHEMES) {
                match(COLON);
                scheme();
                schemeList();

                match(FACTS);
                match(COLON);
                factList();

                match(RULES);
                match(COLON);
                ruleList();

                match(QUERIES);
                match(COLON);
                query();
                queryList();

                match(EOFILE);
            } else {
                throw current_token();
            }
        }



    //-------section list grammar rules------------//
        void schemeList() {
            if (tokenType() == ID) {
                scheme();
                schemeList();
            } // else: lambda
        }

        void factList() {
            if (tokenType() == ID) {
                fact();
                factList();
            } // else: lambda
        }

        void ruleList() {
            if (tokenType() == ID) {
                rule();
                ruleList();
            } // else: lambda
        }

        void queryList() {
            if (tokenType() == ID) {
                query();
                queryList();
            } // else: lambda
        }


    //------Individual lines grammar rules--------//
        void scheme() {
        // Grammar Rule
        // scheme -> ID LEFT_PAREN ID idList RIGHT_PAREN
        if (tokenType() == ID) {
            match(ID);
            match(LEFT_PAREN);
            match(ID);
            idList();
            match(RIGHT_PAREN);
        } else {
            throw current_token();
        }
    }

        void fact() {
            if (tokenType() == ID) {
                match(ID);
                match(LEFT_PAREN);
                match(STRING);
                stringList();
                match(RIGHT_PAREN);
                match(PERIOD);
            } else {
                throw current_token();
            }
        }

        void rule() {
            headPredicate();
            if (tokenType() == COLON_DASH) {
                match(COLON_DASH);
                predicate();
                predicateList();
                match(PERIOD);
            } else {
                throw current_token();
            }
        }

        void query() {
            predicate();
            if (tokenType() == Q_MARK) {
                match(Q_MARK);
            } else {
                throw current_token();
            }
        }


    //-------predicate grammar rules-------------//
        void headPredicate() {
            if (tokenType() == ID) {
                match(ID);
                match(LEFT_PAREN);
                match(ID);
                idList();
                match(RIGHT_PAREN);
            } else {
                throw current_token();
            }
        }

        void predicate() {
            if (tokenType() == ID) {
                match(ID);
                match(LEFT_PAREN);
                parameter();
                parameterList();
                match(RIGHT_PAREN);
            } else {
                throw current_token();
            }
        }

    //-------ending list grammar rules------------//

        void predicateList() {
            if (tokenType() == COMMA) {
                match(COMMA);
                predicate();
                predicateList();
            } // else: lambda
        }

        void parameterList() {
            if (tokenType() == COMMA) {
                match(COMMA);
                parameter();
                parameterList();
            } // else: lambda
        }

        void stringList() {
            if (tokenType() == COMMA) {
                match(COMMA);
                match(STRING);
                stringList();
            // else: lambda
            }
        }

        void idList() {
            if (tokenType() == COMMA) {
                match(COMMA);
                match(ID);
                idList();
            } // else: lambda
        }

    //-------Dead end grammar rules-------//
        void parameter() {
            if (tokenType() == STRING) {
                match(STRING);
            } else if (tokenType() == ID) {
                match(ID);
            } else {
                throw current_token();
            }
        }















//---------------helper functions------------------------------//

        TokenType tokenType() const {
            return tokens.at(0).get_token_type();
        }

        void advanceToken() {
            tokens.erase(tokens.begin());
        }

        Token current_token() {
        return tokens.at(0);
}
        void throwError() {
            cout << "error" << endl;
        }

        void match(TokenType t) {
            cout << "match: " << t << endl;
            // if the current token type matches t
            if (tokenType() == t) {
                // advance to the next token
                advanceToken();
            } else {
                // report a syntax error
                throwError();
            }
        }

};

