#include "Router.h"

using namespace std;

// Constructor
Router::Router(int id) {
    routerID = id;
}

// Returns router ID
int Router::getRouterID() const {
    return routerID;
}

// Returns router connections
const vector<Edge>& Router::getConnections() const {
    return connections;
}