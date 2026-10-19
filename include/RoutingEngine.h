#ifndef ROUTING_ENGINE_H
#define ROUTING_ENGINE_H

#include <string>
#include <vector>

// Forward declaration of Network to prevent tight header coupling
class Network;

/**
 * @brief Represents the outcome of a route discovery query.
 * Contains path availability, sequence of router IDs, total link cost, and hop count.
 */
struct RouteResult {
    bool found = false;
    std::vector<std::string> path;
    long long totalCost = 0;
    int hops = 0;
};

/**
 * @class RoutingEngine
 * @brief Computes shortest paths using Dijkstra's algorithm and generates routing tables.
 * 
 * Single Responsibility:
 * Encapsulates pathfinding algorithms without polluting network storage or packet logic.
 */
class RoutingEngine {
public:
    RouteResult findShortestRoute(const Network& network, const std::string& source, const std::string& destination);
    void displayRoutingTable(const Network& network, const std::string& source) const;
};

#endif // ROUTING_ENGINE_H
