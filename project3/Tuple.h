#include <string>
#include <vector>
#include <sstream>

#include "Scheme.h"

#pragma once
using namespace std;


// A tuple is a list of attribute values
class Tuple : public vector<string> {

public:

    Tuple(vector<string> values) : vector<string>(values) { }

    string toString(const Scheme& scheme) const {
        const Tuple& tuple = *this;
        stringstream out;
        for (size_t i = 0; i < scheme.size(); i++) {
            out << scheme.at(i);
            out << "=";
            out << tuple.at(i);
            if (i + 1 < scheme.size()) {
                out << ", ";
            }
        }
        return out.str();
    }
};

