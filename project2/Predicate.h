#pragma once
#include "Parameter.h"

#include <string>
#include <vector>


using namespace std;

class Predicate {
private:
    string name;
    vector<Parameter> parameter_list;
    string tail;
public:

    // constructor
    Predicate(string in_name, vector<Parameter> in_parameter_list, string in_tail = "") {
        name = in_name;
        parameter_list = in_parameter_list;
        tail = in_tail;
    }

    // converts object to string representation
    string to_string() {
        string temp_string = name + "(";
        for (int i = 0; i < parameter_list.size(); i++) {
            temp_string += parameter_list.at(i).to_string();
            if (i + 1 < parameter_list.size()) {
                temp_string += ",";
            }
        }
        temp_string += ")" + tail;
        return temp_string;
    }

    // get functions
    string get_name() {
        return name;
    }

    vector<Parameter> get_parameter_list() {
        return parameter_list;
    }

};