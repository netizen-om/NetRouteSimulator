#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>
#include <vector>

using namespace std;

// Represents a connection to another router
struct Edge {
    int to;
    int cost;
};

// Represents an individual router
class Router {
private:
    int routerID;
    vector<Edge> connections;

    // Only Network can modify router connections
    friend class Network;

public:
    // Constructor
    Router(int id);

    // Returns router ID
    int getRouterID() const;

    // Returns all connections
    const vector<Edge>& getConnections() const;
};

#endif