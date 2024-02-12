#pragma once
#include "../project1/Token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
#include "DatalogProgram.h"

#include <stdexcept>
#include <iostream>
#include <vector>



using namespace std;

class Parser {
private:
    vector<Token> tokens;
    vector<Predicate> scheme_list_sum;
    vector<Predicate> fact_list_sum;
    vector<Rule> rule_list_sum;
    vector<Predicate> query_list_sum;

    vector<Parameter> cache_parameter_list = {}; // note: the cache is cleared before each new predicate is parsed
    vector<Predicate> cache_predicates = {}; // note: the cache is cleared before each new list is parsed

public:
//---------------constructor------------------------------------//
    Parser(const vector<Token>& tokens) : tokens(tokens) {}

    // start the recursive parsing algorithm by calling the start symbol
    DatalogProgram parse() {
        try {
             datalog_program();
        }
        catch (Token& problem_token) {
            cout << "Failure!" << endl;
            cout << "  " << problem_token.toString() << endl;
            exit(0);
        }
        cout << "Success!" << endl;
        DatalogProgram datalog_program_temp = DatalogProgram(scheme_list_sum, fact_list_sum, rule_list_sum, query_list_sum);
        cout << datalog_program_temp.to_string_datalog();
        return datalog_program_temp;
    }

//---------------parser functions for Grammar Rules-------------------------//
//-------start symbol--------------------------//
    void datalog_program() {
        if (tokenType() == SCHEMES) {
            match(SCHEMES);
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
            throwError();
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
    // COMPLETE
    Predicate scheme() { // this function creates a scheme predicate
        if (tokenType() == ID) {
            cache_parameter_list.clear(); // clears the cache for this predicate
            string temp_name = match(ID); // This is the name for the predicate
            match(LEFT_PAREN);
            add_string_to_cache_parameter_list(match(ID)); // This ID is a parameter for a scheme predicate
            idList();
            match(RIGHT_PAREN);
            return {temp_name, cache_parameter_list};
        } else {
            throwError();
        }
    }

    // COMPLETE
    Predicate fact() { // this function creates a fact predicate
        if (tokenType() == ID) {
            cache_parameter_list.clear(); // clears the cache for this predicate
            string temp_name = match(ID); // This is the name for the predicate
            match(LEFT_PAREN);
            add_string_to_cache_parameter_list(match(STRING)); // This STRING is a parameter for a fact predicate
            stringList();
            match(RIGHT_PAREN);
            match(PERIOD);
            return {temp_name, cache_parameter_list};
        } else {
            throwError();
        }
    }

    // COMPLETE
    Rule rule() { // this function creates a rule
        Predicate temp_head_predicate = headPredicate();
        if (tokenType() == COLON_DASH) {
            cache_predicates.clear(); // clears the cache
            match(COLON_DASH);
            cache_predicates.push_back(predicate());
            predicateList();
            match(PERIOD);
            return {temp_head_predicate, cache_predicates};
        } else {
            throwError();
        }
    }

    // COMPLETE
    Predicate query() {
        Predicate temp_predicate = predicate();
        if (tokenType() == Q_MARK) {
            match(Q_MARK);
            return temp_predicate;
        } else {
            throwError();
        }
    }


//-------predicate grammar rules-------------//
    // COMPLETE
    Predicate headPredicate() {
        if (tokenType() == ID) {
            cache_parameter_list.clear(); // clears the cache for this predicate
            string temp_name = match(ID); // This is the name for the predicate
            match(LEFT_PAREN);
            add_string_to_cache_parameter_list(match(ID)); // This ID is a parameter for a rule's head predicate
            idList();
            match(RIGHT_PAREN);
            return {temp_name, cache_parameter_list};
        } else {
            throwError();
        }
    }

    // COMPLETE
    Predicate predicate() {
        if (tokenType() == ID) {
            cache_parameter_list.clear(); // clears the cache for this predicate
            string temp_name = match(ID); // This is the name for the predicate
            match(LEFT_PAREN);
            cache_parameter_list.push_back(parameter()); // This is a parameter for the predicate
            parameterList();
            match(RIGHT_PAREN);
            return {temp_name, cache_parameter_list};
        } else {
            throwError();
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

    // COMPLETE
    void parameterList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            cache_parameter_list.push_back(parameter()); // This is a parameter for a predicate
            parameterList();
        } // else: lambda
    }

    // COMPLETE
    void stringList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            add_string_to_cache_parameter_list(match(STRING)); // This is a parameter for a fact predicate
            stringList();
        // else: lambda
        }
    }
    // COMPLETE
    void idList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            add_string_to_cache_parameter_list(match(ID)); // This is a parameter for a scheme predicate
            idList();
        } // else: lambda
    }

//-------Dead end grammar rules-------//
    // COMPLETE
    Parameter parameter() {
        if (tokenType() == STRING) {
            string temp_string = match(STRING);
            return {temp_string};
        } else if (tokenType() == ID) {
            string temp_string = match(ID);
            return {temp_string};
        } else {
            throwError();
        }
    }


//---------------helper functions------------------------------//
    //------get functions------//
    Token current_token() {
    return tokens.at(0);
    }

    //--------parsing helper functions----------//
    TokenType tokenType() const {
        return tokens.at(0).get_token_type();
    }

    void throwError() {
//            cout << "error" << endl; // DEBUG?
        throw current_token();
    }

    void add_string_to_cache_parameter_list(string temp) {
        cache_parameter_list.push_back(Parameter(temp));
    }


    string get_token_string() {
        return current_token().toString();
    }

    string advanceToken() {
        string temp = get_token_string();
        tokens.erase(tokens.begin());
        return temp;
    }

    string match(TokenType t) {
//            cout << "match: " << t << endl; // DEBUG
        // if the current token type matches t
        if (tokenType() == t) {
            // advance to the next token
            return advanceToken();
        } else {
            // report a syntax error
            throwError();
        }
    }

};

