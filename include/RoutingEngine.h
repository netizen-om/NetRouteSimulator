#ifndef ROUTING_ENGINE_H
#define ROUTING_ENGINE_H

#include <string>
#include <vector>

using namespace std;

// Forward declaration of Network to avoid tight coupling
class Network;

// Stores the outcome of a shortest-path calculation
struct RouteResult {
    bool found = false;
    vector<string> path;
    long long totalCost = 0;
    int hops = 0;
};

// Calculates shortest paths using Dijkstra's algorithm
class RoutingEngine {
public:
    RouteResult findShortestRoute(const Network& network, string source, string destination);
    void displayRoutingTable(const Network& network, string source) const;
};

#endif // ROUTING_ENGINE_H
