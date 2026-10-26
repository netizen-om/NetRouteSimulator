#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// Represents a connection (link) to a neighboring router
struct Edge {
    int to;    // Neighbor router ID
    int cost;  // Cost/weight of the link
};

// Manages network routers, connections, and shortest-path routing
class Network {
private:
    int routerCount;              // Total number of routers in the network
    vector<vector<Edge>> adjList; // Adjacency list: adjList[u] stores edges connected to router u

public:
    // Constructor initializes an empty network
    Network();

    // Adds a router with an automatically assigned integer ID
    void addRouter();

    // Prompts user and connects two existing routers with a positive cost
    void connectRouters();

    // Displays the complete network topology
    void displayNetwork();

    // Prompts user and finds shortest route using Dijkstra's algorithm
    void findShortestRoute();
};

#endif // NETWORK_H
