// ============================================================================
// File: ProjectDeclarations.cpp
// Description: Shared declaration file containing standard includes, enums,
//              structs, and class definitions for the NetRoute Simulator project.
//              Used as a declaration-only inclusion file across translation units.
// ============================================================================

#ifndef PROJECT_DECLARATIONS_CPP
#define PROJECT_DECLARATIONS_CPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <utility>

// ============================================================================
// SECTION 1: ENUMS
// ============================================================================

/**
 * @brief Represents the lifecycle states of a network packet.
 */
enum class PacketStatus {
    Created,
    InTransit,
    Delivered,
    Dropped
};

// ============================================================================
// SECTION 2: DATA STRUCTURES
// ============================================================================

/**
 * @brief Represents the outcome of a network configuration operation.
 * Separates core network validation/manipulation from user-interface display.
 */
struct OperationResult {
    bool success = false;
    std::string message;
};

/**
 * @brief Represents the outcome of a route discovery query.
 * Contains path availability, sequence of router IDs, total link cost, and hop count.
 */
struct RouteResult {
    bool exists = false;
    std::vector<std::string> path;
    int totalCost = 0;
    int hopCount = 0;
};

// ============================================================================
// SECTION 3: CLASS DECLARATIONS
// ============================================================================

/**
 * @class Router
 * @brief Represents a single router node in the simulated network.
 * 
 * Single Responsibility:
 * Encapsulates router identity and operational state (active vs. inactive).
 */
class Router {
private:
    std::string id_;
    bool active_;

public:
    explicit Router(const std::string& id);

    const std::string& getId() const;
    bool isActive() const;
    void setActive(bool active);
};

/**
 * @class Network
 * @brief Manages the graph topology: routers, links, and adjacency relationships.
 * 
 * Single Responsibility:
 * Maintains the network nodes and connections, validates topology modifications,
 * and manages adjacency data without mixing in console I/O.
 */
class Network {
private:
    // Collection of routers indexed by their unique ID
    std::unordered_map<std::string, Router> routers_;

    // Adjacency list: router ID -> list of (neighbor router ID, link weight)
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adjacencyList_;

public:
    OperationResult addRouter(const std::string& id);
    OperationResult removeRouter(const std::string& id);
    OperationResult connectRouters(const std::string& sourceId, const std::string& destId, int cost);
    OperationResult disconnectRouters(const std::string& sourceId, const std::string& destId);

    bool hasRouter(const std::string& id) const;
    const std::vector<std::pair<std::string, int>>& getNeighbors(const std::string& routerId) const;

    size_t getRouterCount() const;
    size_t getLinkCount() const;

    void displayTopology() const;

    const std::unordered_map<std::string, Router>& getRouters() const;
    const std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>& getAdjacencyList() const;
};

/**
 * @class RoutingEngine
 * @brief Computes shortest paths and routing tables.
 * 
 * Single Responsibility:
 * Encapsulates pathfinding algorithms (e.g. Dijkstra) without polluting
 * network storage or packet logic.
 */
class RoutingEngine {
public:
    RouteResult findShortestPath(const Network& network, const std::string& source, const std::string& destination);
    void displayRoutingTable(const Network& network) const;
};

/**
 * @class Packet
 * @brief Represents a single data packet travelling across the network.
 * 
 * Single Responsibility:
 * Holds packet metadata (id, source, destination, hop path, current status).
 */
class Packet {
private:
    std::string id_;
    std::string source_;
    std::string destination_;
    std::vector<std::string> path_;
    PacketStatus status_;

public:
    Packet(const std::string& id, const std::string& source, const std::string& destination);

    const std::string& getId() const;
    const std::string& getSource() const;
    const std::string& getDestination() const;
    const std::vector<std::string>& getPath() const;
    PacketStatus getStatus() const;

    void setPath(const std::vector<std::string>& path);
    void setStatus(PacketStatus status);

    static std::string statusToString(PacketStatus status);
};

/**
 * @class PacketSimulator
 * @brief Manages packet forwarding, delivery histories, and network stats.
 * 
 * Single Responsibility:
 * Handles packet transmission simulation, tracking history and delivery metrics.
 */
class PacketSimulator {
private:
    std::vector<Packet> packetHistory_;
    int packetsCreated_ = 0;
    int packetsDelivered_ = 0;
    int packetsDropped_ = 0;

public:
    void sendPacket(Packet packet, const Network& network, RoutingEngine& engine);
    void displayHistory() const;
    void displayStatistics() const;
};

/**
 * @class NetworkSimulator
 * @brief Top-level coordinator that manages user interaction and routes commands.
 * 
 * Single Responsibility:
 * Coordinates the CLI menu, reads user choices, and delegates actions to
 * Network, RoutingEngine, and PacketSimulator.
 */
class NetworkSimulator {
private:
    Network network_;
    RoutingEngine routingEngine_;
    PacketSimulator packetSimulator_;

    void handleAddRouter();
    void handleRemoveRouter();
    void handleConnectRouters();
    void handleDisconnectRouters();
    void handleDisplayTopology() const;
    void handleFindShortestRoute();
    void handleDisplayRoutingTable() const;
    void handleSendPacket();
    void handleDisplayPacketHistory() const;
    void handleDisplayStatistics() const;
    void printMenu() const;

public:
    void run();
};

#endif // PROJECT_DECLARATIONS_CPP
