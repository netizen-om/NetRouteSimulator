#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#include "Router.h"

using namespace std;

class Network {
private:
    vector<Router> routers;
    static int nextRouterID;

    // Finds a router using its ID
    Router* findRouter(int id);

public:
    // Constructor
    Network();

    // Add a new router
    void addRouter();

    // Connect two routers
    void connectRouters();

    // Remove a router
    void removeRouter();

    // Display complete network
    void displayNetwork();

    // Find shortest route using Dijkstra's algorithm
    void findShortestRoute();

    // Check whether the network is connected
    void checkConnectivity();
};

#endif