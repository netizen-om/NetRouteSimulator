#ifndef NETWORK_H
#define NETWORK_H

#include "Router.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

using namespace std;

// Result of network operations (add, remove, connect, etc.)
struct OperationResult {
    bool success;
    string message;
};

// Manages network routers and connections (graph representation)
class Network {
private:
    // Map of router ID to Router object
    unordered_map<string, Router> routers_;

    // Adjacency list: router ID -> list of (neighbor router ID, link cost)
    unordered_map<string, vector<pair<string, int>>> adjacencyList_;

public:
    OperationResult addRouter(string id);
    OperationResult removeRouter(string id);
    OperationResult connectRouters(string sourceId, string destId, int cost);
    OperationResult disconnectRouters(string sourceId, string destId);

    bool hasRouter(string id) const;
    bool isRouterActive(string id) const;
    OperationResult setRouterActive(string id, bool active);

    const vector<pair<string, int>>& getNeighbors(string routerId) const;

    int getRouterCount() const;
    int getLinkCount() const;

    void displayTopology() const;
    OperationResult loadDemonstrationNetwork();

    const unordered_map<string, Router>& getRouters() const;
    const unordered_map<string, vector<pair<string, int>>>& getAdjacencyList() const;
};

#endif // NETWORK_H
