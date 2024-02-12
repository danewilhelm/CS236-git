#pragma once
#include "Parameter.h"
#include "Predicate.h"

#include <string>
#include <vector>

using namespace std;

class Rule {
private:
    Predicate head_predicate;
    vector<Predicate> body_predicate_list;

public:
    // constructor
    Rule(Predicate in_head, vector<Predicate> in_body)
        : head_predicate(in_head), body_predicate_list(in_body) {}

    string to_string() {
        string temp_string = head_predicate.to_string() + " :- ";
        for (int i = 0; i < body_predicate_list.size(); i++) {
            temp_string += body_predicate_list.at(i).to_string();
            if (i + 1 < body_predicate_list.size()) {
                temp_string += ",";
            }
        }
        return temp_string;
    }

    Predicate get_head_predicate() {
        return head_predicate;
    }

    vector<Predicate> get_body_predicate_list() {
        return body_predicate_list;
    }

};