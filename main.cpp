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

// Global network data
int routerCount = 0;              // Total number of routers in the network
vector<vector<Edge>> adjList;     // Adjacency list: adjList[u] stores edges connected to router u

// Function to add a router with an automatically assigned integer ID
void addRouter() {
    routerCount++;
    adjList.resize(routerCount + 1);
    cout << "Router added successfully with ID: " << routerCount << "\n";
}

// Function to connect two existing routers with a positive cost
void connectRouters() {
    int router1, router2, cost;

    cout << "Enter first router ID: ";
    cin >> router1;
    cout << "Enter second router ID: ";
    cin >> router2;
    cout << "Enter link cost: ";
    cin >> cost;

    // Validate that both routers exist
    if (router1 < 1 || router1 > routerCount || router2 < 1 || router2 > routerCount) {
        cout << "Error: One or both router IDs do not exist.\n";
        return;
    }

    // Validate self-connection
    if (router1 == router2) {
        cout << "Error: Cannot connect a router to itself.\n";
        return;
    }

    // Validate positive link cost
    if (cost <= 0) {
        cout << "Error: Link cost must be a positive integer.\n";
        return;
    }

    // Check if a connection already exists between router1 and router2
    for (const Edge& edge : adjList[router1]) {
        if (edge.to == router2) {
            cout << "Error: A connection already exists between router "
                 << router1 << " and router " << router2 << ".\n";
            return;
        }
    }

    // Add bidirectional connection
    adjList[router1].push_back({router2, cost});
    adjList[router2].push_back({router1, cost});

    cout << "Successfully connected router " << router1 << " and router "
         << router2 << " with cost " << cost << ".\n";
}

// Function to display the network topology
void displayNetwork() {
    if (routerCount == 0) {
        cout << "The network is empty. No routers added yet.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "           NETWORK TOPOLOGY\n";
    cout << "========================================\n";

    for (int i = 1; i <= routerCount; ++i) {
        cout << "Router " << i << " -> ";
        if (adjList[i].empty()) {
            cout << "[No connections (Isolated)]\n";
        } else {
            for (size_t j = 0; j < adjList[i].size(); ++j) {
                if (j > 0) {
                    cout << ", ";
                }
                cout << "Router " << adjList[i][j].to << " (Cost: " << adjList[i][j].cost << ")";
            }
            cout << "\n";
        }
    }
    cout << "========================================\n";
}

// Function to find the shortest route between two routers using Dijkstra's algorithm
void findShortestRoute() {
    int source, destination;

    cout << "Enter source router ID: ";
    cin >> source;
    cout << "Enter destination router ID: ";
    cin >> destination;

    // Validate router IDs
    if (source < 1 || source > routerCount || destination < 1 || destination > routerCount) {
        cout << "Error: Invalid router ID. Router does not exist.\n";
        return;
    }

    // Trivial case: source is destination
    if (source == destination) {
        cout << "\n========================================\n";
        cout << "            SHORTEST ROUTE\n";
        cout << "========================================\n";
        cout << "Source Router: " << source << "\n";
        cout << "Destination Router: " << destination << "\n";
        cout << "Shortest Path: " << source << "\n";
        cout << "Total Cost: 0\n";
        cout << "========================================\n";
        return;
    }

    const int INF = 1000000000; // Large number representing infinity

    // distance[i] stores the minimum cost from source to router i
    vector<int> distance(routerCount + 1, INF);

    // parent[i] stores the previous router on the shortest path
    vector<int> parent(routerCount + 1, -1);

    // visited[i] tracks whether the shortest distance to router i is finalized
    vector<bool> visited(routerCount + 1, false);

    // Min-priority queue storing pairs of (cost, routerId)
    // greater ensures the router with the lowest cost is at the top
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // Step 1: Initialize the source distance to 0 and push into queue
    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        // Step 2: Extract the router with the smallest known distance
        int currentCost = pq.top().first;
        int currentRouter = pq.top().second;
        pq.pop();

        // Skip if this router has already been visited
        if (visited[currentRouter]) {
            continue;
        }
        visited[currentRouter] = true;

        // Stop early if the destination router is reached
        if (currentRouter == destination) {
            break;
        }

        // Step 3: Check all neighbors of the current router
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

    // Step 4: Check if a path to the destination exists
    if (distance[destination] == INF) {
        cout << "No route exists between these routers.\n";
        return;
    }

    // Step 5: Reconstruct the path from destination back to source
    vector<int> path;
    int curr = destination;
    while (curr != -1) {
        path.push_back(curr);
        curr = parent[curr];
    }
    reverse(path.begin(), path.end());

    // Display the shortest route results
    cout << "\n========================================\n";
    cout << "            SHORTEST ROUTE\n";
    cout << "========================================\n";
    cout << "Source Router: " << source << "\n";
    cout << "Destination Router: " << destination << "\n";
    cout << "Shortest Path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) {
            cout << " -> ";
        }
        cout << path[i];
    }
    cout << "\nTotal Cost: " << distance[destination] << "\n";
    cout << "========================================\n";
}

int main() {
    int choice = -1;

    do {
        cout << "\n========================================\n";
        cout << "         NETROUTE SIMULATOR\n";
        cout << "========================================\n";
        cout << "1. Add Router\n";
        cout << "2. Connect Routers\n";
        cout << "3. Display Network\n";
        cout << "4. Find Shortest Route\n";
        cout << "0. Exit\n";
        cout << "========================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Handle invalid non-numeric input
        if (cin.fail()) {
            cin.clear();
            string dummy;
            cin >> dummy;
            cout << "Invalid input. Please enter a valid number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                addRouter();
                break;
            case 2:
                connectRouters();
                break;
            case 3:
                displayNetwork();
                break;
            case 4:
                findShortestRoute();
                break;
            case 0:
                cout << "Exiting NetRoute Simulator. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please select an option between 0 and 4.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
