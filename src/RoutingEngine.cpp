#include "RoutingEngine.h"
#include "Network.h"

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

RouteResult RoutingEngine::findShortestRoute(const Network& network, string source, string destination) {
    RouteResult result;

    // Check if source and destination exist in the network
    if (!network.hasRouter(source) || !network.hasRouter(destination)) {
        return result;
    }
    // Check if source and destination are active
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

    // 1. How distances are initialized:
    // Set all router distances to infinity, except the source router which starts at 0.
    const long long INF = numeric_limits<long long>::max();
    unordered_map<string, long long> dist;
    unordered_map<string, string> predecessor;

    for (const auto& pair : network.getRouters()) {
        dist[pair.first] = INF;
    }
    dist[source] = 0;

    // 2. How the next router with the lowest known distance is selected:
    // We use a min-priority queue storing pairs of (distance, router ID).
    // The top of the queue always gives the router with the smallest tentative distance.
    priority_queue<pair<long long, string>, vector<pair<long long, string>>, greater<pair<long long, string>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        long long currentDist = pq.top().first;
        string u = pq.top().second;
        pq.pop();

        // Skip stale entries if we already found a shorter path to u
        if (currentDist > dist[u]) {
            continue;
        }

        // Early-exit optimization: stop once destination is reached
        if (u == destination) {
            break;
        }

        // 3. How neighboring routers are updated (relaxation):
        // For each neighbor, check if traveling through router u gives a cheaper distance.
        const auto& neighbors = network.getNeighbors(u);
        for (const auto& edge : neighbors) {
            string v = edge.first;
            int weight = edge.second;

            // Skip inactive neighboring routers
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

    // If destination distance remains infinity, it is unreachable
    if (dist[destination] == INF) {
        result.found = false;
        return result;
    }

    // 4. How the final path is reconstructed:
    // Walk backward from the destination to the source using the predecessor map,
    // then reverse the path to get the correct source-to-destination order.
    vector<string> path;
    string curr = destination;
    while (curr != source) {
        path.push_back(curr);
        if (predecessor.find(curr) == predecessor.end()) {
            return result;
        }
        curr = predecessor[curr];
    }
    path.push_back(source);

    reverse(path.begin(), path.end());

    result.found = true;
    result.path = path;
    result.totalCost = dist[destination];
    result.hops = static_cast<int>(path.size() - 1);
    return result;
}

void RoutingEngine::displayRoutingTable(const Network& network, string source) const {
    if (!network.hasRouter(source)) {
        cout << "[Error] Source router '" << source << "' does not exist in the network.\n";
        return;
    }
    if (!network.isRouterActive(source)) {
        cout << "[Error] Source router '" << source << "' is currently INACTIVE.\n";
        return;
    }

    // Compute shortest paths from source to all reachable routers in a single Dijkstra run
    const long long INF = numeric_limits<long long>::max();
    unordered_map<string, long long> dist;
    unordered_map<string, string> predecessor;

    for (const auto& pair : network.getRouters()) {
        dist[pair.first] = INF;
    }
    dist[source] = 0;

    priority_queue<pair<long long, string>, vector<pair<long long, string>>, greater<pair<long long, string>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        long long currentDist = pq.top().first;
        string u = pq.top().second;
        pq.pop();

        if (currentDist > dist[u]) {
            continue;
        }

        for (const auto& edge : network.getNeighbors(u)) {
            string v = edge.first;
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

    // Sort destination router IDs alphabetically
    vector<string> dests;
    for (const auto& pair : network.getRouters()) {
        dests.push_back(pair.first);
    }
    sort(dests.begin(), dests.end());

    cout << "\n======================================================================\n";
    cout << "               ROUTING TABLE FOR ROUTER [" << source << "]\n";
    cout << "======================================================================\n";
    cout << left << setw(15) << "Destination"
         << "| " << setw(15) << "Total Cost"
         << "| " << setw(15) << "Next Hop"
         << "| Status\n";
    cout << "----------------------------------------------------------------------\n";

    for (const string& dest : dests) {
        cout << left << setw(15) << dest << "| ";

        if (dest == source) {
            cout << setw(15) << "0"
                 << "| " << setw(15) << source
                 << "| Local (Self)\n";
        } else if (dist[dest] == INF) {
            cout << setw(15) << "UNREACHABLE"
                 << "| " << setw(15) << "None"
                 << "| Unreachable\n";
        } else {
            // Trace backward from dest to find the first router after source
            string curr = dest;
            while (predecessor.find(curr) != predecessor.end() && predecessor.at(curr) != source) {
                curr = predecessor.at(curr);
            }
            string nextHop = curr;

            cout << setw(15) << dist[dest]
                 << "| " << setw(15) << nextHop
                 << "| Reachable\n";
        }
    }
    cout << "======================================================================\n";
}
