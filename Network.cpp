#include "Network.h"

using namespace std;

// Constructor
Network::Network() {
    nextRouterID = 1;
}

// Finds a router using its ID
Router* Network::findRouter(int id) {
    for (Router& router : routers) {
        if (router.getRouterID() == id) {
            return &router;
        }
    }

    return nullptr;
}

// Add a new router
void Network::addRouter() {
    routers.push_back(Router(nextRouterID));

    cout << "Router added successfully with ID: "
         << nextRouterID << "\n";

    nextRouterID++;
}

// Connect two routers
void Network::connectRouters() {
    int router1, router2, cost;

    cout << "Enter first router ID: ";
    cin >> router1;

    cout << "Enter second router ID: ";
    cin >> router2;

    cout << "Enter link cost: ";
    cin >> cost;

    Router* r1 = findRouter(router1);
    Router* r2 = findRouter(router2);

    if (r1 == nullptr || r2 == nullptr) {
        cout << "Error: One or both router IDs do not exist.\n";
        return;
    }

    if (router1 == router2) {
        cout << "Error: Cannot connect a router to itself.\n";
        return;
    }

    if (cost <= 0) {
        cout << "Error: Link cost must be positive.\n";
        return;
    }

    for (const Edge& edge : r1->getConnections()) {
        if (edge.to == router2) {
            cout << "Error: Connection already exists.\n";
            return;
        }
    }

    // Add bidirectional connection
    r1->connections.push_back({router2, cost});
    r2->connections.push_back({router1, cost});

    cout << "Routers connected successfully.\n";
}

// Remove a router
void Network::removeRouter() {
    int id;

    cout << "Enter router ID to remove: ";
    cin >> id;

    Router* router = findRouter(id);

    if (router == nullptr) {
        cout << "Error: Router does not exist.\n";
        return;
    }

    // Remove connections to this router from other routers
    for (Router& r : routers) {
        vector<Edge>& edges = r.connections;

        edges.erase(
            remove_if(edges.begin(), edges.end(),
                [id](const Edge& edge) {
                    return edge.to == id;
                }),
            edges.end()
        );
    }

    // Remove the router itself
    routers.erase(
        remove_if(routers.begin(), routers.end(),
            [id](const Router& r) {
                return r.getRouterID() == id;
            }),
        routers.end()
    );

    cout << "Router " << id << " removed successfully.\n";
}

// Display network topology
void Network::displayNetwork() {
    if (routers.empty()) {
        cout << "The network is empty.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "           NETWORK TOPOLOGY\n";
    cout << "========================================\n";

    for (const Router& router : routers) {
        cout << "Router " << router.getRouterID() << " -> ";

        const vector<Edge>& edges = router.getConnections();

        if (edges.empty()) {
            cout << "[No connections (Isolated)]";
        } else {
            for (size_t i = 0; i < edges.size(); i++) {
                if (i > 0) {
                    cout << ", ";
                }

                cout << "Router " << edges[i].to
                     << " (Cost: " << edges[i].cost << ")";
            }
        }

        cout << "\n";
    }

    cout << "========================================\n";
}

// Check network connectivity using BFS
void Network::checkConnectivity() {
    if (routers.empty()) {
        cout << "The network is empty.\n";
        return;
    }

    vector<int> visited(nextRouterID, 0);
    queue<int> q;

    int startID = routers[0].getRouterID();

    q.push(startID);
    visited[startID] = 1;

    int count = 0;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        count++;

        Router* router = findRouter(current);

        for (const Edge& edge : router->getConnections()) {
            if (!visited[edge.to]) {
                visited[edge.to] = 1;
                q.push(edge.to);
            }
        }
    }

    cout << "\nNetwork Connectivity:\n";

    if (count == routers.size()) {
        cout << "The network is CONNECTED.\n";
        cout << "All routers are reachable from each other.\n";
    } else {
        cout << "The network is DISCONNECTED.\n";
        cout << "Some routers are not reachable.\n";
    }
}

// Find shortest route using Dijkstra's algorithm
void Network::findShortestRoute() {
    int source, destination;

    cout << "Enter source router ID: ";
    cin >> source;

    cout << "Enter destination router ID: ";
    cin >> destination;

    if (findRouter(source) == nullptr ||
        findRouter(destination) == nullptr) {
        cout << "Error: Invalid router ID.\n";
        return;
    }

    if (source == destination) {
        cout << "Shortest Path: " << source << "\n";
        cout << "Total Cost: 0\n";
        return;
    }

    const int INF = 1000000000;

    vector<int> distance(nextRouterID, INF);
    vector<int> parent(nextRouterID, -1);
    vector<bool> visited(nextRouterID, false);

    priority_queue<pair<int, int>,
                   vector<pair<int, int>>,
                   greater<pair<int, int>>> pq;

    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int currentCost = pq.top().first;
        int current = pq.top().second;
        pq.pop();

        if (visited[current]) {
            continue;
        }

        visited[current] = true;

        if (current == destination) {
            break;
        }

        Router* router = findRouter(current);

        for (const Edge& edge : router->getConnections()) {
            int neighbor = edge.to;
            int newCost = currentCost + edge.cost;

            if (!visited[neighbor] &&
                newCost < distance[neighbor]) {

                distance[neighbor] = newCost;
                parent[neighbor] = current;

                pq.push({newCost, neighbor});
            }
        }
    }

    if (distance[destination] == INF) {
        cout << "No route exists between these routers.\n";
        return;
    }

    vector<int> path;

    int current = destination;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    cout << "\n========================================\n";
    cout << "            SHORTEST ROUTE\n";
    cout << "========================================\n";

    cout << "Source Router: " << source << "\n";
    cout << "Destination Router: " << destination << "\n";

    cout << "Shortest Path: ";

    for (size_t i = 0; i < path.size(); i++) {
        if (i > 0) {
            cout << " -> ";
        }

        cout << path[i];
    }

    cout << "\nTotal Cost: " << distance[destination] << "\n";

    cout << "========================================\n";
}