#pragma once
#include "Token.h"
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
//        cout << "Success!" << endl;
        DatalogProgram datalog_program_temp = DatalogProgram(scheme_list_sum, fact_list_sum, rule_list_sum, query_list_sum);
//        cout << datalog_program_temp.to_string_datalog();
        return datalog_program_temp;
    }

//---------------parser functions for Grammar Rules-------------------------//
//-------start symbol--------------------------//
    // COMPLETE
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
    // COMPLETE
    void schemeList() {
        if (tokenType() == ID) {
            scheme(); // appends the created predicate to the list
            schemeList();
        } // else: lambda
    }

    // COMPLETE
    void factList() {
        if (tokenType() == ID) {
            fact(); // appends the created predicate to the list
            factList();
        } // else: lambda
    }
    // COMPLETE
    void ruleList() {
        if (tokenType() == ID) {
            rule();
            ruleList();
        } // else: lambda
    }

    // COMPLETE
    void queryList() {
        if (tokenType() == ID) {
            query(); // appends the created predicate to the list
            queryList();
        } // else: lambda
    }


//------Individual lines grammar rules--------//
    // COMPLETE
    void scheme() { // this function creates a scheme predicate
        if (tokenType() == ID) {
            cache_parameter_list.clear(); // clears the cache for this predicate
            string temp_name = match(ID); // This is the name for the predicate
            match(LEFT_PAREN);
            add_string_to_cache_parameter_list(match(ID), false); // This ID is a parameter for a scheme predicate
            idList();
            match(RIGHT_PAREN);
            scheme_list_sum.push_back(Predicate(temp_name, cache_parameter_list)); // adds predicate to scheme list
        } else {
            throwError();
        }
    }

    // COMPLETE
    void fact() { // this function creates a fact predicate
        if (tokenType() == ID) {
            cache_parameter_list.clear(); // clears the cache for this predicate
            string temp_name = match(ID); // This is the name for the predicate
            match(LEFT_PAREN);
            add_string_to_cache_parameter_list(match(STRING), true); // This STRING is a parameter for a fact predicate
            stringList();
            match(RIGHT_PAREN);
            match(PERIOD);
            fact_list_sum.push_back(Predicate(temp_name, cache_parameter_list));
        } else {
            throwError();
        }
    }

    // COMPLETE
    void rule() { // this function creates a rule
        Predicate temp_head_predicate = headPredicate();
        if (tokenType() == COLON_DASH) {
            cache_predicates.clear(); // clears the cache
            match(COLON_DASH);
            cache_predicates.push_back(predicate());
            predicateList();
            match(PERIOD);
            rule_list_sum.push_back(Rule(temp_head_predicate, cache_predicates));
        } else {
            throwError();
        }
    }

    // COMPLETE
    void query() {
        Predicate temp_predicate = predicate();
        if (tokenType() == Q_MARK) {
            match(Q_MARK);
            query_list_sum.push_back(temp_predicate);
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
            add_string_to_cache_parameter_list(match(ID), false); // This ID is a parameter for a rule's head predicate
            idList();
            match(RIGHT_PAREN);
            return {temp_name, cache_parameter_list};
        } else {
            throwError();

            cout << "LOGIC ERROR IN HEADPREDICATE FUNCTION" << endl;
            return {"LOGIC ERROR IN HEADPREDICATE FUNCTION", {}};        }
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
        cout << "LOGIC ERROR IN PREDICATE FUNCTION" << endl;
        return {"LOGIC ERROR IN PREDICATE FUNCTION", {}};
    }

//-------ending list grammar rules------------//

    // COMPLETE
    void predicateList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            cache_predicates.push_back(predicate()); // This is one predicate for the body predicates for a rule
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
            add_string_to_cache_parameter_list(match(STRING), true); // This is a parameter for a fact predicate
            stringList();
        // else: lambda
        }
    }
    // COMPLETE
    void idList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            add_string_to_cache_parameter_list(match(ID), false); // This is a parameter for a scheme predicate
            idList();
        } // else: lambda
    }

//-------Dead end grammar rules-------//
    // COMPLETE
    Parameter parameter() {
        if (tokenType() == STRING) {
            string temp_string = match(STRING);
            return {temp_string, true};
        } else if (tokenType() == ID) {
            string temp_string = match(ID);
            return {temp_string, false};
        } else {
            throwError();
        }
        cout << "LOGIC ERROR IN PARAMETER FUNCTION" << endl;
        return {"LOGIC ERROR IN PARAMETER FUNCTION", true};
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

    void add_string_to_cache_parameter_list(string temp, bool is_constant) {
        cache_parameter_list.push_back(Parameter(temp, is_constant));
    }


    string getTokenValue() {
        return current_token().get_token_value();
    }

    string advanceToken() {
        string temp = getTokenValue();
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
        cout << "LOGIC ERROR IN MATCH FUNCTION" << endl;
        return "LOGIC ERROR IN MATCH FUNCTION";
    }

};

