#ifndef NETWORK_H
#define NETWORK_H

#include "Router.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <cstddef>

/**
 * @brief Represents the outcome of a network configuration operation.
 * Separates core network validation/manipulation from user-interface display.
 */
struct OperationResult {
    bool success = false;
    std::string message;
};

/**
 * @class Network
 * @brief Manages the graph topology: routers, links, and adjacency relationships.
 * 
 * Single Responsibility:
 * Maintains the network nodes and connections, validates topology modifications,
 * and manages adjacency data without mixing in console I/O.
 */
class Network {
private:
    // Collection of routers indexed by their unique ID
    std::unordered_map<std::string, Router> routers_;

    // Adjacency list: router ID -> list of (neighbor router ID, link weight)
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adjacencyList_;

public:
    OperationResult addRouter(const std::string& id);
    OperationResult removeRouter(const std::string& id);
    OperationResult connectRouters(const std::string& sourceId, const std::string& destId, int cost);
    OperationResult disconnectRouters(const std::string& sourceId, const std::string& destId);

    bool hasRouter(const std::string& id) const;
    bool isRouterActive(const std::string& id) const;
    OperationResult setRouterActive(const std::string& id, bool active);

    const std::vector<std::pair<std::string, int>>& getNeighbors(const std::string& routerId) const;

    size_t getRouterCount() const;
    size_t getLinkCount() const;

    void displayTopology() const;
    OperationResult loadDemonstrationNetwork();

    const std::unordered_map<std::string, Router>& getRouters() const;
    const std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>& getAdjacencyList() const;
};

#endif // NETWORK_H
