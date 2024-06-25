#include "Node.h"
#include <map>
#include <string>
#include <sstream>

#pragma once
using namespace std;

class Graph {

private:

    map<int,Node> nodes;

public:

    Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++)
            nodes[nodeID] = Node();
    }

    void addEdge(int fromNodeID, int toNodeID) {
        // the rule associated with "fromNode" depends on rule associated with "toNode"
        // Note: The graph stores a map, connecting the nodeID and the node object...
            // But the Node object itself stores adjacent node IDs, not the node objects
        nodes[fromNodeID].addEdge(toNodeID);
    }

    string toString() {
        stringstream out;
        for (auto& pair: nodes) {
            int nodeID = pair.first;
            Node node = pair.second;

            out << "R" << nodeID << ":";
            out << node.toString() << endl;
        }
        return out.str();
    }

    map<int, Node> get_nodes_map() {
        return nodes;
    }

    void mark_node_as_visited(int node_ID) {
        if (nodes.at(node_ID).has_been_visited()) {
            cout << "ERROR: Within a graph, trying to mark a node as visited when already visited" << endl;
        }
        nodes.at(node_ID).set_as_visited();
    }

    void reset_all_markers_to_unvisited() {
        for (auto& node_map_pair: nodes) {
            node_map_pair.second.reset_as_unvisited();
        }
    }
};