#include "Network.h"

#include <iostream>
#include <algorithm>

OperationResult Network::addRouter(const std::string& id) {
    if (id.empty()) {
        return {false, "Router ID cannot be empty."};
    }
    if (routers_.find(id) != routers_.end()) {
        return {false, "Router '" + id + "' already exists."};
    }

    routers_.emplace(id, Router(id));
    adjacencyList_[id] = {};
    return {true, "Router '" + id + "' added successfully."};
}

OperationResult Network::removeRouter(const std::string& id) {
    if (routers_.find(id) == routers_.end()) {
        return {false, "Router '" + id + "' does not exist."};
    }

    // Remove node from router table and adjacency list
    routers_.erase(id);
    adjacencyList_.erase(id);

    // Remove links referencing this router from all other nodes to eliminate dangling links
    for (auto& entry : adjacencyList_) {
        auto& neighbors = entry.second;
        neighbors.erase(
            std::remove_if(neighbors.begin(), neighbors.end(),
                           [&id](const std::pair<std::string, int>& link) {
                               return link.first == id;
                           }),
            neighbors.end()
        );
    }

    return {true, "Router '" + id + "' and all associated connections removed."};
}

OperationResult Network::connectRouters(const std::string& sourceId, const std::string& destId, int cost) {
    if (routers_.find(sourceId) == routers_.end()) {
        return {false, "Router '" + sourceId + "' does not exist."};
    }
    if (routers_.find(destId) == routers_.end()) {
        return {false, "Router '" + destId + "' does not exist."};
    }
    if (sourceId == destId) {
        return {false, "Cannot connect a router to itself (self-links are not permitted)."};
    }
    if (cost <= 0) {
        return {false, "Link cost must be a positive integer greater than zero."};
    }

    // Check if an existing link already exists
    const auto& srcNeighbors = adjacencyList_[sourceId];
    for (const auto& link : srcNeighbors) {
        if (link.first == destId) {
            return {false, "Link already exists between '" + sourceId + "' and '" + destId + "'."};
        }
    }

    // Add bidirectional edge (undirected connection)
    adjacencyList_[sourceId].push_back({destId, cost});
    adjacencyList_[destId].push_back({sourceId, cost});

    return {true, "Connected '" + sourceId + "' <---> '" + destId + "' with cost " + std::to_string(cost) + "."};
}

OperationResult Network::disconnectRouters(const std::string& sourceId, const std::string& destId) {
    if (routers_.find(sourceId) == routers_.end()) {
        return {false, "Router '" + sourceId + "' does not exist."};
    }
    if (routers_.find(destId) == routers_.end()) {
        return {false, "Router '" + destId + "' does not exist."};
    }

    auto& srcNeighbors = adjacencyList_[sourceId];
    auto itSrc = std::find_if(srcNeighbors.begin(), srcNeighbors.end(),
                              [&destId](const std::pair<std::string, int>& link) {
                                  return link.first == destId;
                              });

    if (itSrc == srcNeighbors.end()) {
        return {false, "No direct link exists between '" + sourceId + "' and '" + destId + "'."};
    }

    srcNeighbors.erase(itSrc);

    auto& dstNeighbors = adjacencyList_[destId];
    auto itDst = std::find_if(dstNeighbors.begin(), dstNeighbors.end(),
                              [&sourceId](const std::pair<std::string, int>& link) {
                                  return link.first == sourceId;
                              });
    if (itDst != dstNeighbors.end()) {
        dstNeighbors.erase(itDst);
    }

    return {true, "Disconnected '" + sourceId + "' and '" + destId + "'."};
}

bool Network::hasRouter(const std::string& id) const {
    return routers_.find(id) != routers_.end();
}

bool Network::isRouterActive(const std::string& id) const {
    auto it = routers_.find(id);
    if (it != routers_.end()) {
        return it->second.isActive();
    }
    return false;
}

OperationResult Network::setRouterActive(const std::string& id, bool active) {
    auto it = routers_.find(id);
    if (it == routers_.end()) {
        return {false, "Router '" + id + "' does not exist."};
    }
    it->second.setActive(active);
    std::string stateStr = active ? "ACTIVE" : "INACTIVE";
    return {true, "Router '" + id + "' is now " + stateStr + "."};
}

const std::vector<std::pair<std::string, int>>& Network::getNeighbors(const std::string& routerId) const {
    static const std::vector<std::pair<std::string, int>> emptyNeighbors;
    auto it = adjacencyList_.find(routerId);
    if (it != adjacencyList_.end()) {
        return it->second;
    }
    return emptyNeighbors;
}

size_t Network::getRouterCount() const {
    return routers_.size();
}

size_t Network::getLinkCount() const {
    size_t totalHalfEdges = 0;
    for (const auto& entry : adjacencyList_) {
        totalHalfEdges += entry.second.size();
    }
    return totalHalfEdges / 2;
}

void Network::displayTopology() const {
    std::cout << "\nNETWORK TOPOLOGY\n";
    std::cout << "----------------\n";

    if (routers_.empty()) {
        std::cout << "(Network is empty. No routers found.)\n";
        return;
    }

    // Collect router IDs and sort them alphabetically
    std::vector<std::string> sortedIds;
    sortedIds.reserve(routers_.size());
    for (const auto& pair : routers_) {
        sortedIds.push_back(pair.first);
    }
    std::sort(sortedIds.begin(), sortedIds.end());

    for (const auto& rId : sortedIds) {
        auto rIt = routers_.find(rId);
        bool active = (rIt != routers_.end()) && rIt->second.isActive();

        std::cout << "Router " << rId << (active ? "" : " [INACTIVE]") << ": ";

        auto it = adjacencyList_.find(rId);
        if (it == adjacencyList_.end() || it->second.empty()) {
            std::cout << "(No connections)";
        } else {
            // Copy and sort neighbors by neighbor router ID alphabetically
            auto neighbors = it->second;
            std::sort(neighbors.begin(), neighbors.end(),
                      [](const auto& a, const auto& b) {
                          return a.first < b.first;
                      });

            for (size_t i = 0; i < neighbors.size(); ++i) {
                if (i > 0) {
                    std::cout << ", ";
                }
                std::cout << neighbors[i].first << " (Cost: " << neighbors[i].second << ")";
            }
        }
        std::cout << "\n";
    }
}

OperationResult Network::loadDemonstrationNetwork() {
    // Adds routers A, B, C, D and connections as specified:
    // A-B: 4, A-C: 2, C-B: 1, B-D: 3, C-D: 7
    addRouter("A");
    addRouter("B");
    addRouter("C");
    addRouter("D");

    connectRouters("A", "B", 4);
    connectRouters("A", "C", 2);
    connectRouters("C", "B", 1);
    connectRouters("B", "D", 3);
    connectRouters("C", "D", 7);

    return {true, "Demonstration network successfully loaded (Routers: A, B, C, D)."};
}

const std::unordered_map<std::string, Router>& Network::getRouters() const {
    return routers_;
}

const std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>& Network::getAdjacencyList() const {
    return adjacencyList_;
}
