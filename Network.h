#ifndef NETWORK_H
#define NETWORK_H

#include <vector>

/**
 * @brief Represents a directed connection (link) to a neighboring router.
 *
 * Implemented as a lightweight struct (data holder) with public members,
 * following idiomatic C++ conventions for Plain Old Data (POD) records.
 */
struct Edge {
    int to;    ///< Destination neighbor router ID
    int cost;  ///< Non-negative transmission cost / weight of the link

    Edge() : to(0), cost(0) {}
    Edge(int neighborId, int linkCost) : to(neighborId), cost(linkCost) {}
};

/**
 * @brief Status codes returned when attempting to connect two routers.
 *
 * Promotes strong typing and decouples network validation from console I/O.
 */
enum class ConnectionResult {
    Success,
    InvalidRouterId,
    SelfConnection,
    NonPositiveCost,
    DuplicateConnection
};

/**
 * @brief Encapsulates the complete outcome of a shortest path query.
 *
 * Decouples Dijkstra's algorithm from console output, allowing both
 * user interfaces and automated tests to inspect the computed route.
 */
struct RouteResult {
    bool success;         ///< True if source and destination router IDs are valid
    bool sameSourceDest;   ///< True if source == destination (trivial 0-cost path)
    bool routeExists;      ///< True if a reachable path connects source to destination
    int totalCost;         ///< Minimum cumulative cost from source to destination
    std::vector<int> path; ///< Sequence of router IDs forming the optimal path

    RouteResult()
        : success(false),
          sameSourceDest(false),
          routeExists(false),
          totalCost(0) {}
};

/**
 * @brief Core domain class responsible for network management and routing.
 *
 * Encapsulates the internal graph representation (adjacency list) and router
 * count, preventing unauthorized external modification. Provides a clean,
 * abstract public interface for topology manipulation and pathfinding.
 */
class Network {
private:
    int routerCount;                     ///< Total number of routers in the network
    std::vector<std::vector<Edge>> adjList; ///< 1-indexed adjacency list: adjList[u] stores links from router u

public:
    /**
     * @brief Constructs an empty network.
     */
    Network();

    /**
     * @brief Adds a new router to the network with an auto-assigned ID.
     * @return The integer ID assigned to the new router (1, 2, 3, ...).
     */
    int addRouter();

    /**
     * @brief Connects two existing routers with a bidirectional weighted link.
     * @param router1 First router ID
     * @param router2 Second router ID
     * @param cost Positive link weight
     * @return ConnectionResult status enum indicating success or specific validation failure.
     */
    ConnectionResult connectRouters(int router1, int router2, int cost);

    /**
     * @brief Computes the shortest route between two routers using Dijkstra's algorithm.
     * @param source Source router ID
     * @param destination Destination router ID
     * @return RouteResult struct encapsulating path existence, total cost, and hop sequence.
     */
    RouteResult findShortestRoute(int source, int destination) const;

    /**
     * @brief Returns the total number of routers currently in the network.
     */
    int getRouterCount() const;

    /**
     * @brief Checks if a given router ID exists in the network.
     * @param routerId Router ID to check
     * @return True if 1 <= routerId <= routerCount, false otherwise.
     */
    bool isValidRouterId(int routerId) const;

    /**
     * @brief Checks whether a direct connection exists between two routers.
     * @param router1 First router ID
     * @param router2 Second router ID
     * @return True if connected, false otherwise.
     */
    bool hasConnection(int router1, int router2) const;

    /**
     * @brief Retrieves the list of edges connected to a specific router.
     * @param routerId Router ID (must be valid)
     * @return Const reference to the vector of outgoing Edge structs.
     */
    const std::vector<Edge>& getNeighbors(int routerId) const;

    /**
     * @brief Checks if the network contains zero routers.
     */
    bool isEmpty() const;
};

#endif // NETWORK_H
