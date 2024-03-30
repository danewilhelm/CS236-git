#pragma once
#include <string>

using namespace std;

class Parameter {
private:
    string name;
    bool is_constant;

public:
    // constructor
    Parameter(string in_name, bool in_is_constant) {
        name = in_name;
        is_constant = in_is_constant;
    }

    // get function
    // Note: this is the string representation of the object
    string to_string()
    {
        return name;
    }

    bool get_is_constant() {
        return is_constant;
    }
};
