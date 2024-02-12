#pragma once
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"

#include <string>
#include <vector>
#include <set>

using namespace std;

class DatalogProgram {
private:
    vector<Predicate> scheme_list;
    vector<Predicate> fact_list;
    vector<Rule> rule_list;
    vector<Predicate> query_list;
    set<string> domain_set;

public:

    //---------------constructor functions------------------------//
    DatalogProgram(vector<Predicate> in_scheme_list, vector<Predicate> in_fact_list, vector<Rule> in_rule_list, vector<Predicate> in_query_list) {
        scheme_list = in_scheme_list;
        fact_list = in_fact_list;
        rule_list = in_rule_list;
        query_list = in_query_list;
        populate_domain_set();
    }



    void populate_domain_set() {
        for (auto cur_fact : fact_list) { {
            vector<Parameter> cur_parameter_list = cur_fact.get_parameter_list();
            for (auto cur_parameter : cur_parameter_list) {
                domain_set.insert(cur_parameter.to_string());
            }
        }
//            fact_list.at(i).to_string());
        }
    }

//---------------to string functions-------------------------------------//
    template <typename T>
    string header_to_string(string header, T input_list) {
        return header + "(" + to_string(input_list.size()) + "):\n";
    }

    template <typename T>
    string list_to_string(T input_list, string tail) {
        string t_string;
        for (int i = 0; i < input_list.size(); i++) {
            t_string += "  " + input_list.at(i).to_string() + tail + "\n";
        }
        return t_string;
    }

    // note: to_string(int var) is how to convert integers to strings, so the function is renamed
    string to_string_datalog() {
        string temp_string;
        // Schemes
        temp_string += header_to_string("Schemes", scheme_list);
        temp_string += list_to_string(scheme_list, "");

        // Facts header
        temp_string += header_to_string("Facts", fact_list);
        temp_string += list_to_string(fact_list, ".");

        // Rules header
        temp_string += header_to_string("Rules", rule_list);
        temp_string += list_to_string(rule_list, ".");

        // Queries header
        temp_string += header_to_string("Queries", query_list);
        temp_string += list_to_string(query_list, "?");

        // Domain header
        temp_string += header_to_string("Domain", domain_set);
        for (auto cur_string : domain_set) {
            temp_string += "  " + cur_string + "\n";
        }

        return temp_string;
    }

//----------get functions------------------------//
    vector<Predicate> get_scheme_list() {
        return scheme_list;
    }

    vector<Predicate> get_fact_list() {
        return fact_list;
    }

    vector<Rule> get_rule_list() {
        return rule_list;
    }

    vector<Predicate> get_query_list() {
        return query_list;
    }

    set<string> get_domain_set() {
        return domain_set;
    }
};