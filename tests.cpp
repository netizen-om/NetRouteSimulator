#include <iostream>
#include <sstream>
#include "Network.h"

using namespace std;

int main() {
    cout << "========================================\n";
    cout << "    NETROUTE SIMULATOR BASIC TESTS\n";
    cout << "========================================\n";

    // Backup original cin buffer
    streambuf* origCin = cin.rdbuf();

    Network net;

    // Test 1: Add Routers
    cout << "\n[Test 1] Adding 3 routers:\n";
    net.addRouter(); // Router 1
    net.addRouter(); // Router 2
    net.addRouter(); // Router 3

    // Test 2: Connect Routers (1-2 with cost 4, 2-3 with cost 5, 1-3 with cost 12)
    cout << "\n[Test 2] Connecting routers:\n";
    stringstream connInputs;
    // 1 to 2 cost 4
    connInputs << "1 2 4\n";
    // 2 to 3 cost 5
    connInputs << "2 3 5\n";
    // 1 to 3 cost 12
    connInputs << "1 3 12\n";
    // Invalid router
    connInputs << "9 1 5\n";
    // Self connection
    connInputs << "1 1 5\n";
    // Non-positive cost
    connInputs << "1 2 -5\n";
    // Duplicate connection
    connInputs << "1 2 10\n";

    cin.rdbuf(connInputs.rdbuf());

    net.connectRouters(); // 1-2
    net.connectRouters(); // 2-3
    net.connectRouters(); // 1-3
    net.connectRouters(); // Invalid
    net.connectRouters(); // Self
    net.connectRouters(); // Negative
    net.connectRouters(); // Duplicate

    // Test 3: Display Network
    cout << "\n[Test 3] Display Network Topology:\n";
    net.displayNetwork();

    // Test 4: Shortest Route Queries
    cout << "\n[Test 4] Shortest Route from 1 to 3 (should pick path 1->2->3 with cost 9):\n";
    stringstream routeInputs;
    routeInputs << "1 3\n"; // 1 to 3
    routeInputs << "1 1\n"; // Same source/dest
    cin.rdbuf(routeInputs.rdbuf());
    net.findShortestRoute();

    cout << "\n[Test 5] Route where source equals destination (1 to 1):\n";
    net.findShortestRoute();

    // Test 6: Disconnected Router
    cout << "\n[Test 6] Adding isolated Router 4 and querying path from 1 to 4:\n";
    net.addRouter(); // Router 4
    stringstream discInputs;
    discInputs << "1 4\n";
    cin.rdbuf(discInputs.rdbuf());
    net.findShortestRoute();

    // Restore original cin
    cin.rdbuf(origCin);

    cout << "\n========================================\n";
    cout << "     ALL BASIC TESTS COMPLETED!\n";
    cout << "========================================\n";

    return 0;
}
