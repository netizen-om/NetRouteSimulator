#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>
#include "Network.h"

// Simple lightweight test runner
static int totalTests = 0;
static int passedTests = 0;

#define TEST_ASSERT(cond, message) do { \
    totalTests++; \
    if (cond) { \
        passedTests++; \
        std::cout << "  [PASS] " << message << "\n"; \
    } else { \
        std::cerr << "  [FAIL] " << message << " (Line " << __LINE__ << ")\n"; \
    } \
} while(0)

void testAddRouters() {
    std::cout << "Testing: Add Multiple Routers...\n";
    Network net;
    TEST_ASSERT(net.isEmpty(), "Initial network is empty");
    TEST_ASSERT(net.getRouterCount() == 0, "Initial router count is 0");

    int id1 = net.addRouter();
    int id2 = net.addRouter();
    int id3 = net.addRouter();

    TEST_ASSERT(id1 == 1, "First router ID is 1");
    TEST_ASSERT(id2 == 2, "Second router ID is 2");
    TEST_ASSERT(id3 == 3, "Third router ID is 3");
    TEST_ASSERT(net.getRouterCount() == 3, "Router count updated to 3");
    TEST_ASSERT(!net.isEmpty(), "Network is no longer empty");
}

void testConnectionValidation() {
    std::cout << "\nTesting: Connection Validation...\n";
    Network net;
    net.addRouter(); // ID 1
    net.addRouter(); // ID 2
    net.addRouter(); // ID 3

    // Valid connection
    ConnectionResult res1 = net.connectRouters(1, 2, 10);
    TEST_ASSERT(res1 == ConnectionResult::Success, "Connect router 1 and 2 with cost 10 succeeds");
    TEST_ASSERT(net.hasConnection(1, 2), "Connection exists 1 -> 2");
    TEST_ASSERT(net.hasConnection(2, 1), "Connection exists 2 -> 1 (bidirectional)");

    // Reject invalid router IDs
    ConnectionResult resInvalid1 = net.connectRouters(0, 2, 5);
    TEST_ASSERT(resInvalid1 == ConnectionResult::InvalidRouterId, "Rejects router ID 0 (below range)");
    ConnectionResult resInvalid2 = net.connectRouters(1, 99, 5);
    TEST_ASSERT(resInvalid2 == ConnectionResult::InvalidRouterId, "Rejects router ID 99 (above range)");

    // Reject self-connection
    ConnectionResult resSelf = net.connectRouters(2, 2, 5);
    TEST_ASSERT(resSelf == ConnectionResult::SelfConnection, "Rejects self-connection (2 to 2)");

    // Reject non-positive costs
    ConnectionResult resZero = net.connectRouters(2, 3, 0);
    TEST_ASSERT(resZero == ConnectionResult::NonPositiveCost, "Rejects zero cost");
    ConnectionResult resNeg = net.connectRouters(2, 3, -15);
    TEST_ASSERT(resNeg == ConnectionResult::NonPositiveCost, "Rejects negative cost");

    // Reject duplicate connections
    ConnectionResult resDup1 = net.connectRouters(1, 2, 20);
    TEST_ASSERT(resDup1 == ConnectionResult::DuplicateConnection, "Rejects duplicate connection 1 -> 2");
    ConnectionResult resDup2 = net.connectRouters(2, 1, 20);
    TEST_ASSERT(resDup2 == ConnectionResult::DuplicateConnection, "Rejects duplicate reverse connection 2 -> 1");
}

void testIsolatedRouters() {
    std::cout << "\nTesting: Isolated Routers...\n";
    Network net;
    net.addRouter(); // ID 1
    net.addRouter(); // ID 2
    net.addRouter(); // ID 3 (isolated)

    net.connectRouters(1, 2, 5);

    TEST_ASSERT(net.getNeighbors(1).size() == 1, "Router 1 has 1 connection");
    TEST_ASSERT(net.getNeighbors(2).size() == 1, "Router 2 has 1 connection");
    TEST_ASSERT(net.getNeighbors(3).empty(), "Router 3 is isolated (0 connections)");
}

void testShortestRouteIntermediate() {
    std::cout << "\nTesting: Shortest Route Through Intermediate Routers...\n";
    // Topology: 1 --(4)-- 2 --(5)-- 3
    Network net;
    net.addRouter(); // 1
    net.addRouter(); // 2
    net.addRouter(); // 3
    net.connectRouters(1, 2, 4);
    net.connectRouters(2, 3, 5);

    RouteResult res = net.findShortestRoute(1, 3);
    TEST_ASSERT(res.success, "Query between valid routers succeeded");
    TEST_ASSERT(res.routeExists, "Route exists between 1 and 3");
    TEST_ASSERT(res.totalCost == 9, "Total cost is 9 (4 + 5)");
    std::vector<int> expectedPath = {1, 2, 3};
    TEST_ASSERT(res.path == expectedPath, "Path is 1 -> 2 -> 3");
}

void testMultiplePathsLowestCost() {
    std::cout << "\nTesting: Multiple Paths Verification (Chooses Lowest Cost)...\n";
    // Topology:
    // 1 --(12)-- 3
    // 1 --(4)-- 2 --(5)-- 3
    Network net;
    net.addRouter(); // 1
    net.addRouter(); // 2
    net.addRouter(); // 3
    net.connectRouters(1, 2, 4);
    net.connectRouters(2, 3, 5);
    net.connectRouters(1, 3, 12);

    RouteResult res = net.findShortestRoute(1, 3);
    TEST_ASSERT(res.routeExists, "Route exists");
    TEST_ASSERT(res.totalCost == 9, "Chooses indirect cost 9 over direct cost 12");
    std::vector<int> expectedPath = {1, 2, 3};
    TEST_ASSERT(res.path == expectedPath, "Path is 1 -> 2 -> 3");

    // Also verify reverse direction (undirected graph)
    RouteResult revRes = net.findShortestRoute(3, 1);
    TEST_ASSERT(revRes.totalCost == 9, "Reverse route cost is 9");
    std::vector<int> expectedRevPath = {3, 2, 1};
    TEST_ASSERT(revRes.path == expectedRevPath, "Reverse path is 3 -> 2 -> 1");
}

void testSourceEqualsDestination() {
    std::cout << "\nTesting: Source Equals Destination...\n";
    Network net;
    net.addRouter(); // 1
    net.addRouter(); // 2

    RouteResult res = net.findShortestRoute(1, 1);
    TEST_ASSERT(res.success, "Query valid");
    TEST_ASSERT(res.sameSourceDest, "sameSourceDest is true");
    TEST_ASSERT(res.routeExists, "routeExists is true");
    TEST_ASSERT(res.totalCost == 0, "Cost to self is 0");
    std::vector<int> expectedPath = {1};
    TEST_ASSERT(res.path == expectedPath, "Path is [1]");
}

void testDisconnectedRouters() {
    std::cout << "\nTesting: Disconnected Routers (Unreachable Destination)...\n";
    Network net;
    net.addRouter(); // 1
    net.addRouter(); // 2
    net.addRouter(); // 3 (isolated)
    net.connectRouters(1, 2, 7);

    RouteResult res = net.findShortestRoute(1, 3);
    TEST_ASSERT(res.success, "Query valid");
    TEST_ASSERT(!res.routeExists, "Route does NOT exist to isolated router 3");
    TEST_ASSERT(res.path.empty(), "Path is empty when no route exists");
}

void testInvalidRouterQueries() {
    std::cout << "\nTesting: Invalid Router Queries...\n";
    Network net;
    net.addRouter(); // 1

    RouteResult res1 = net.findShortestRoute(1, 5);
    TEST_ASSERT(!res1.success, "Query with out-of-range destination fails");

    RouteResult res2 = net.findShortestRoute(0, 1);
    TEST_ASSERT(!res2.success, "Query with out-of-range source fails");
}

int main() {
    std::cout << "========================================\n";
    std::cout << "   NETROUTE SIMULATOR AUTOMATED TESTS   \n";
    std::cout << "========================================\n\n";

    testAddRouters();
    testConnectionValidation();
    testIsolatedRouters();
    testShortestRouteIntermediate();
    testMultiplePathsLowestCost();
    testSourceEqualsDestination();
    testDisconnectedRouters();
    testInvalidRouterQueries();

    std::cout << "\n========================================\n";
    std::cout << "TEST RESULTS: " << passedTests << " / " << totalTests << " passed.\n";
    std::cout << "========================================\n";

    return (passedTests == totalTests) ? 0 : 1;
}
