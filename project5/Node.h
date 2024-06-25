#include <set>
#include <string>
#include <sstream>


using namespace std;
#pragma once

class Node {

private:

    set<int> adjacentNodeIDs;
    bool been_visited = false;
    // You may want add other variables to your Node class later, perhaps a visited flag or a post-order number
    // RECOMMENDED TO NOT STORE POST-ORDER NUMBER IN THE NODE OBJECT

public:

    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }

    string toString() {
        stringstream out;
        size_t loop_counter = 0;
        for (int node_ID : adjacentNodeIDs) {
            out << "R" << node_ID;
            if (loop_counter + 1 < adjacentNodeIDs.size()) {
                out << ",";
            }
            loop_counter++;
        }
        return out.str();
    }

    bool has_been_visited() {
        return been_visited;
    }

    set<int> get_adjacent_node_IDs() {
        return adjacentNodeIDs;
    }

    void set_as_visited() {
        if (been_visited) {
            cout << "ERROR: trying to mark this node as visited when already visited" << endl;
        }
        been_visited = true;
    }

    void reset_as_unvisited() {
        been_visited = false;
    }

};