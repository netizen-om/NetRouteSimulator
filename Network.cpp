#include "Network.h"
#include <algorithm>
#include <queue>

Network::Network() : routerCount(0), adjList(1) {
    // adjList[0] is initialized but unused to support natural 1-based router indexing
}

int Network::addRouter() {
    routerCount++;
    adjList.resize(routerCount + 1);
    return routerCount;
}

ConnectionResult Network::connectRouters(int router1, int router2, int cost) {
    // Validate router existence
    if (!isValidRouterId(router1) || !isValidRouterId(router2)) {
        return ConnectionResult::InvalidRouterId;
    }

    // Validate against self-connection (self-loop)
    if (router1 == router2) {
        return ConnectionResult::SelfConnection;
    }

    // Validate positive cost requirement
    if (cost <= 0) {
        return ConnectionResult::NonPositiveCost;
    }

    // Check for existing connection between the two routers
    if (hasConnection(router1, router2)) {
        return ConnectionResult::DuplicateConnection;
    }

    // Add bidirectional communication link
    adjList[router1].emplace_back(router2, cost);
    adjList[router2].emplace_back(router1, cost);

    return ConnectionResult::Success;
}

RouteResult Network::findShortestRoute(int source, int destination) const {
    RouteResult result;

    // Validate router IDs
    if (!isValidRouterId(source) || !isValidRouterId(destination)) {
        result.success = false;
        return result;
    }
    result.success = true;

    // Trivial case: source is identical to destination
    if (source == destination) {
        result.sameSourceDest = true;
        result.routeExists = true;
        result.totalCost = 0;
        result.path = {source};
        return result;
    }

    const int INF = 1000000000; // Value representing unreachable distance

    // distance[i] stores the minimum cost from source to router i
    std::vector<int> distance(routerCount + 1, INF);

    // parent[i] tracks the predecessor on the optimal path for reconstruction
    std::vector<int> parent(routerCount + 1, -1);

    // visited[i] tracks whether the shortest distance to router i has been finalized
    std::vector<bool> visited(routerCount + 1, false);

    // Min-priority queue storing pairs of (cost, routerId)
    // std::greater ensures the router with the lowest tentative cost is popped first
    std::priority_queue<std::pair<int, int>,
                        std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>> pq;

    // Step 1: Initialize source distance to 0 and push into the queue
    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        // Step 2: Extract router with the smallest tentative distance
        int currentCost = pq.top().first;
        int currentRouter = pq.top().second;
        pq.pop();

        // Skip if already visited
        if (visited[currentRouter]) {
            continue;
        }
        visited[currentRouter] = true;

        // Early stopping: destination reached with confirmed shortest distance
        if (currentRouter == destination) {
            break;
        }

        // Step 3: Examine all neighbors
        for (const Edge& edge : adjList[currentRouter]) {
            int neighbor = edge.to;
            int linkCost = edge.cost;

            // Relaxation step: update neighbor distance if a shorter path is found
            if (!visited[neighbor] && currentCost + linkCost < distance[neighbor]) {
                distance[neighbor] = currentCost + linkCost;
                parent[neighbor] = currentRouter;
                pq.push({distance[neighbor], neighbor});
            }
        }
    }

    // Step 4: Check if destination was reachable
    if (distance[destination] == INF) {
        result.routeExists = false;
        return result;
    }

    // Step 5: Reconstruct path backwards from destination to source
    result.routeExists = true;
    result.totalCost = distance[destination];

    std::vector<int> reconstructedPath;
    int curr = destination;
    while (curr != -1) {
        reconstructedPath.push_back(curr);
        curr = parent[curr];
    }
    std::reverse(reconstructedPath.begin(), reconstructedPath.end());
    result.path = std::move(reconstructedPath);

    return result;
}

int Network::getRouterCount() const {
    return routerCount;
}

bool Network::isValidRouterId(int routerId) const {
    return routerId >= 1 && routerId <= routerCount;
}

bool Network::hasConnection(int router1, int router2) const {
    if (!isValidRouterId(router1) || !isValidRouterId(router2)) {
        return false;
    }

    for (const Edge& edge : adjList[router1]) {
        if (edge.to == router2) {
            return true;
        }
    }
    return false;
}

const std::vector<Edge>& Network::getNeighbors(int routerId) const {
    static const std::vector<Edge> emptyNeighbors;
    if (!isValidRouterId(routerId)) {
        return emptyNeighbors;
    }
    return adjList[routerId];
}

bool Network::isEmpty() const {
    return routerCount == 0;
}
