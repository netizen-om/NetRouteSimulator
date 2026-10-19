#include "Network.h"

#include <iostream>
#include <algorithm>

using namespace std;

OperationResult Network::addRouter(string id) {
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

OperationResult Network::removeRouter(string id) {
    if (routers_.find(id) == routers_.end()) {
        return {false, "Router '" + id + "' does not exist."};
    }

    // Remove router from router collection and adjacency list
    routers_.erase(id);
    adjacencyList_.erase(id);

    // Remove any links to this router from other routers' neighbor lists
    for (auto& entry : adjacencyList_) {
        vector<pair<string, int>>& neighbors = entry.second;
        vector<pair<string, int>> updatedNeighbors;
        for (const auto& link : neighbors) {
            if (link.first != id) {
                updatedNeighbors.push_back(link);
            }
        }
        neighbors = updatedNeighbors;
    }

    return {true, "Router '" + id + "' and all associated connections removed."};
}

OperationResult Network::connectRouters(string sourceId, string destId, int cost) {
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

    // Add bidirectional connection
    adjacencyList_[sourceId].push_back({destId, cost});
    adjacencyList_[destId].push_back({sourceId, cost});

    return {true, "Connected '" + sourceId + "' <---> '" + destId + "' with cost " + to_string(cost) + "."};
}

OperationResult Network::disconnectRouters(string sourceId, string destId) {
    if (routers_.find(sourceId) == routers_.end()) {
        return {false, "Router '" + sourceId + "' does not exist."};
    }
    if (routers_.find(destId) == routers_.end()) {
        return {false, "Router '" + destId + "' does not exist."};
    }

    // Check if link exists from source to destination
    auto& srcNeighbors = adjacencyList_[sourceId];
    int foundIndex = -1;
    for (size_t i = 0; i < srcNeighbors.size(); ++i) {
        if (srcNeighbors[i].first == destId) {
            foundIndex = static_cast<int>(i);
            break;
        }
    }

    if (foundIndex == -1) {
        return {false, "No direct link exists between '" + sourceId + "' and '" + destId + "'."};
    }

    srcNeighbors.erase(srcNeighbors.begin() + foundIndex);

    // Remove the reverse link from destination to source
    auto& dstNeighbors = adjacencyList_[destId];
    for (size_t i = 0; i < dstNeighbors.size(); ++i) {
        if (dstNeighbors[i].first == sourceId) {
            dstNeighbors.erase(dstNeighbors.begin() + i);
            break;
        }
    }

    return {true, "Disconnected '" + sourceId + "' and '" + destId + "'."};
}

bool Network::hasRouter(string id) const {
    return routers_.find(id) != routers_.end();
}

bool Network::isRouterActive(string id) const {
    auto it = routers_.find(id);
    if (it != routers_.end()) {
        return it->second.isActive();
    }
    return false;
}

OperationResult Network::setRouterActive(string id, bool active) {
    auto it = routers_.find(id);
    if (it == routers_.end()) {
        return {false, "Router '" + id + "' does not exist."};
    }
    it->second.setActive(active);
    string stateStr = active ? "ACTIVE" : "INACTIVE";
    return {true, "Router '" + id + "' is now " + stateStr + "."};
}

const vector<pair<string, int>>& Network::getNeighbors(string routerId) const {
    static const vector<pair<string, int>> emptyNeighbors;
    auto it = adjacencyList_.find(routerId);
    if (it != adjacencyList_.end()) {
        return it->second;
    }
    return emptyNeighbors;
}

int Network::getRouterCount() const {
    return static_cast<int>(routers_.size());
}

int Network::getLinkCount() const {
    int totalHalfEdges = 0;
    for (const auto& entry : adjacencyList_) {
        totalHalfEdges += static_cast<int>(entry.second.size());
    }
    return totalHalfEdges / 2;
}

void Network::displayTopology() const {
    cout << "\nNETWORK TOPOLOGY\n";
    cout << "----------------\n";

    if (routers_.empty()) {
        cout << "(Network is empty. No routers found.)\n";
        return;
    }

    // Collect router IDs and sort them alphabetically
    vector<string> sortedIds;
    for (const auto& pair : routers_) {
        sortedIds.push_back(pair.first);
    }
    sort(sortedIds.begin(), sortedIds.end());

    for (const string& rId : sortedIds) {
        auto rIt = routers_.find(rId);
        bool active = (rIt != routers_.end()) && rIt->second.isActive();

        cout << "Router " << rId << (active ? "" : " [INACTIVE]") << ": ";

        auto it = adjacencyList_.find(rId);
        if (it == adjacencyList_.end() || it->second.empty()) {
            cout << "(No connections)";
        } else {
            // Copy and sort neighbors alphabetically
            auto neighbors = it->second;
            sort(neighbors.begin(), neighbors.end());

            for (size_t i = 0; i < neighbors.size(); ++i) {
                if (i > 0) {
                    cout << ", ";
                }
                cout << neighbors[i].first << " (Cost: " << neighbors[i].second << ")";
            }
        }
        cout << "\n";
    }
}

OperationResult Network::loadDemonstrationNetwork() {
    // Adds routers A, B, C, D and connections:
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

const unordered_map<string, Router>& Network::getRouters() const {
    return routers_;
}

const unordered_map<string, vector<pair<string, int>>>& Network::getAdjacencyList() const {
    return adjacencyList_;
}
