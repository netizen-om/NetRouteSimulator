// ============================================================================
// File: RoutingEngine.cpp
// Description: Implementation of RoutingEngine class methods.
//              Provides Dijkstra's shortest-path algorithm and single-source
//              routing table generation.
// ============================================================================

#include "ProjectDeclarations.cpp"

RouteResult RoutingEngine::findShortestRoute(const Network& network, const std::string& source, const std::string& destination) {
    RouteResult result;

    // 1. Validate endpoint existence
    if (!network.hasRouter(source) || !network.hasRouter(destination)) {
        return result; // found = false, path empty, cost = 0, hops = 0
    }

    // 4. Validate active state (inactive routers cannot act as source or destination)
    if (!network.isRouterActive(source) || !network.isRouterActive(destination)) {
        return result;
    }

    // Special case: source is destination
    if (source == destination) {
        result.found = true;
        result.path = {source};
        result.totalCost = 0;
        result.hops = 0;
        return result;
    }

    // 2. Initialize distances to infinity
    const long long INF = std::numeric_limits<long long>::max();
    std::unordered_map<std::string, long long> dist;
    std::unordered_map<std::string, std::string> predecessor;

    for (const auto& pair : network.getRouters()) {
        dist[pair.first] = INF;
    }

    // 3. Set source distance to zero
    dist[source] = 0;

    // 4. Min-priority queue: pair of (distance, router ID)
    using DistPair = std::pair<long long, std::string>;
    std::priority_queue<DistPair, std::vector<DistPair>, std::greater<DistPair>> pq;
    pq.push({0, source});

    // 5. Repeatedly extract the router with the smallest tentative distance
    while (!pq.empty()) {
        auto [currentDist, u] = pq.top();
        pq.pop();

        // 6. Ignore stale priority-queue entries
        if (currentDist > dist[u]) {
            continue;
        }

        // 9. Early-exit optimization once destination is finalized
        if (u == destination) {
            break;
        }

        // 7. Relax each valid neighboring edge
        const auto& neighbors = network.getNeighbors(u);
        for (const auto& edge : neighbors) {
            const std::string& v = edge.first;
            int weight = edge.second;

            // Inactive router handling: skip inactive intermediate routers
            if (!network.isRouterActive(v)) {
                continue;
            }

            long long newDist = currentDist + weight;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                predecessor[v] = u; // 8. Update predecessor
                pq.push({newDist, v});
            }
        }
    }

    // Destination unreachable
    if (dist[destination] == INF) {
        result.found = false;
        result.path.clear();
        result.totalCost = 0;
        result.hops = 0;
        return result;
    }

    // 10. Reconstruct the path by walking backward through predecessors
    std::vector<std::string> reconstructedPath;
    std::string curr = destination;
    while (curr != source) {
        reconstructedPath.push_back(curr);
        auto it = predecessor.find(curr);
        if (it == predecessor.end()) {
            // Unreachable safeguard
            return result;
        }
        curr = it->second;
    }
    reconstructedPath.push_back(source);

    // 11. Reverse to obtain source-to-destination order
    std::reverse(reconstructedPath.begin(), reconstructedPath.end());

    // 12. Hop count is the number of edges (hops = vertices - 1)
    result.found = true;
    result.path = reconstructedPath;
    result.totalCost = dist[destination];
    result.hops = static_cast<int>(reconstructedPath.size() - 1);

    return result;
}

void RoutingEngine::displayRoutingTable(const Network& network, const std::string& source) const {
    if (!network.hasRouter(source)) {
        std::cout << "[Error] Source router '" << source << "' does not exist in the network.\n";
        return;
    }
    if (!network.isRouterActive(source)) {
        std::cout << "[Error] Source router '" << source << "' is currently INACTIVE.\n";
        return;
    }

    // Run single-source Dijkstra without early exit to find paths to all reachable nodes
    const long long INF = std::numeric_limits<long long>::max();
    std::unordered_map<std::string, long long> dist;
    std::unordered_map<std::string, std::string> predecessor;

    for (const auto& pair : network.getRouters()) {
        dist[pair.first] = INF;
    }
    dist[source] = 0;

    using DistPair = std::pair<long long, std::string>;
    std::priority_queue<DistPair, std::vector<DistPair>, std::greater<DistPair>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [currentDist, u] = pq.top();
        pq.pop();

        if (currentDist > dist[u]) {
            continue;
        }

        const auto& neighbors = network.getNeighbors(u);
        for (const auto& edge : neighbors) {
            const std::string& v = edge.first;
            int weight = edge.second;

            if (!network.isRouterActive(v)) {
                continue;
            }

            long long newDist = currentDist + weight;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                predecessor[v] = u;
                pq.push({newDist, v});
            }
        }
    }

    // Sort destinations alphabetically for clean table presentation
    std::vector<std::string> dests;
    dests.reserve(network.getRouters().size());
    for (const auto& pair : network.getRouters()) {
        dests.push_back(pair.first);
    }
    std::sort(dests.begin(), dests.end());

    std::cout << "\n======================================================================\n";
    std::cout << "               ROUTING TABLE FOR ROUTER [" << source << "]\n";
    std::cout << "======================================================================\n";
    std::cout << std::left << std::setw(15) << "Destination"
              << "| " << std::setw(15) << "Total Cost"
              << "| " << std::setw(15) << "Next Hop"
              << "| " << "Status\n";
    std::cout << "----------------------------------------------------------------------\n";

    for (const auto& dest : dests) {
        std::cout << std::left << std::setw(15) << dest << "| ";

        if (dest == source) {
            std::cout << std::setw(15) << "0"
                      << "| " << std::setw(15) << source
                      << "| Local (Self)\n";
        } else if (dist[dest] == INF) {
            std::cout << std::setw(15) << "UNREACHABLE"
                      << "| " << std::setw(15) << "None"
                      << "| Unreachable\n";
        } else {
            // Find the immediate next hop from source to dest using predecessor chain
            std::string curr = dest;
            while (predecessor.find(curr) != predecessor.end() && predecessor.at(curr) != source) {
                curr = predecessor.at(curr);
            }
            std::string nextHop = curr;

            std::cout << std::setw(15) << dist[dest]
                      << "| " << std::setw(15) << nextHop
                      << "| Reachable\n";
        }
    }
    std::cout << "======================================================================\n";
}
