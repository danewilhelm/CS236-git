#pragma once
#include <string>

using namespace std;

class Parameter {
private:
    string name;

public:
    // constructor
    Parameter(string in_name) {
        name = in_name;
    }

    // get function
    // Note: this is the string representation of the object
    string to_string()
    {
        return name;
    }
};
