#include <string>
#include <vector>

#pragma once
using namespace std;


// a scheme is a list of attribute names
class Scheme : public vector<string> {

public:

    Scheme(vector<string> names) : vector<string>(names) { }

};