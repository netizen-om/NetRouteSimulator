// ============================================================================
// NETROUTE SIMULATOR
// Object-Oriented Programming (OOP) Project
// Language: C++17
// Description: A command-line network routing and packet forwarding simulator.
// ============================================================================

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
// SECTION 3: CLASS DECLARATIONS & DEFINITIONS
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
    // Constructor initializes the router with an ID and active status by default.
    explicit Router(const std::string& id)
        : id_(id), active_(true) {}

    const std::string& getId() const {
        return id_;
    }

    bool isActive() const {
        return active_;
    }

    void setActive(bool active) {
        active_ = active;
    }
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
    // Add a new router to the network
    OperationResult addRouter(const std::string& id) {
        if (id.empty()) {
            return {false, "Router ID cannot be empty."};
        }
        if (routers_.find(id) != routers_.end()) {
            return {false, "Router '" + id + "' already exists."};
        }

        routers_.emplace(id, Router(id));
        adjacencyList_[id] = {};
        return {true, "Router '" + id + "' added successfully."};
    }

    // Remove an existing router and all incident links
    OperationResult removeRouter(const std::string& id) {
        if (routers_.find(id) == routers_.end()) {
            return {false, "Router '" + id + "' does not exist."};
        }

        // Remove node from router table and adjacency list
        routers_.erase(id);
        adjacencyList_.erase(id);

        // Remove links referencing this router from all other nodes to prevent dangling links
        for (auto& entry : adjacencyList_) {
            auto& neighbors = entry.second;
            neighbors.erase(
                std::remove_if(neighbors.begin(), neighbors.end(),
                               [&id](const std::pair<std::string, int>& link) {
                                   return link.first == id;
                               }),
                neighbors.end()
            );
        }

        return {true, "Router '" + id + "' and all associated connections removed."};
    }

    // Connect two routers with an undirected weighted link
    OperationResult connectRouters(const std::string& sourceId, const std::string& destId, int cost) {
        if (routers_.find(sourceId) == routers_.end()) {
            return {false, "Router '" + sourceId + "' does not exist."};
        }
        if (routers_.find(destId) == routers_.end()) {
            return {false, "Router '" + destId + "' does not exist."};
        }
        if (sourceId == destId) {
            return {false, "Cannot connect a router to itself (self-links are not permitted)."};
        }
        if (cost <= 0) {
            return {false, "Link cost must be a positive integer greater than zero."};
        }

        // Check if an existing link already exists
        const auto& srcNeighbors = adjacencyList_[sourceId];
        for (const auto& link : srcNeighbors) {
            if (link.first == destId) {
                return {false, "Link already exists between '" + sourceId + "' and '" + destId + "'."};
            }
        }

        // Add bidirectional edge (undirected connection)
        adjacencyList_[sourceId].push_back({destId, cost});
        adjacencyList_[destId].push_back({sourceId, cost});

        return {true, "Connected '" + sourceId + "' <---> '" + destId + "' with cost " + std::to_string(cost) + "."};
    }

    // Disconnect two routers
    OperationResult disconnectRouters(const std::string& sourceId, const std::string& destId) {
        if (routers_.find(sourceId) == routers_.end()) {
            return {false, "Router '" + sourceId + "' does not exist."};
        }
        if (routers_.find(destId) == routers_.end()) {
            return {false, "Router '" + destId + "' does not exist."};
        }

        auto& srcNeighbors = adjacencyList_[sourceId];
        auto itSrc = std::find_if(srcNeighbors.begin(), srcNeighbors.end(),
                                  [&destId](const std::pair<std::string, int>& link) {
                                      return link.first == destId;
                                  });

        if (itSrc == srcNeighbors.end()) {
            return {false, "No direct link exists between '" + sourceId + "' and '" + destId + "'."};
        }

        srcNeighbors.erase(itSrc);

        auto& dstNeighbors = adjacencyList_[destId];
        auto itDst = std::find_if(dstNeighbors.begin(), dstNeighbors.end(),
                                  [&sourceId](const std::pair<std::string, int>& link) {
                                      return link.first == sourceId;
                                  });
        if (itDst != dstNeighbors.end()) {
            dstNeighbors.erase(itDst);
        }

        return {true, "Disconnected '" + sourceId + "' and '" + destId + "'."};
    }

    // Check if a router exists in the network
    bool hasRouter(const std::string& id) const {
        return routers_.find(id) != routers_.end();
    }

    // Safely returns a router's neighbors (empty list if router not found)
    const std::vector<std::pair<std::string, int>>& getNeighbors(const std::string& routerId) const {
        static const std::vector<std::pair<std::string, int>> emptyNeighbors;
        auto it = adjacencyList_.find(routerId);
        if (it != adjacencyList_.end()) {
            return it->second;
        }
        return emptyNeighbors;
    }

    // Return the total count of routers
    size_t getRouterCount() const {
        return routers_.size();
    }

    // Return the total count of unique bidirectional links
    size_t getLinkCount() const {
        size_t totalHalfEdges = 0;
        for (const auto& entry : adjacencyList_) {
            totalHalfEdges += entry.second.size();
        }
        return totalHalfEdges / 2;
    }

    // Display the network topology sorted alphabetically
    void displayTopology() const {
        std::cout << "\nNETWORK TOPOLOGY\n";
        std::cout << "----------------\n";

        if (routers_.empty()) {
            std::cout << "(Network is empty. No routers found.)\n";
            return;
        }

        // Collect router IDs and sort them alphabetically
        std::vector<std::string> sortedIds;
        sortedIds.reserve(routers_.size());
        for (const auto& pair : routers_) {
            sortedIds.push_back(pair.first);
        }
        std::sort(sortedIds.begin(), sortedIds.end());

        for (const auto& rId : sortedIds) {
            std::cout << "Router " << rId << ": ";

            auto it = adjacencyList_.find(rId);
            if (it == adjacencyList_.end() || it->second.empty()) {
                std::cout << "(No connections)";
            } else {
                // Copy and sort neighbors by neighbor router ID alphabetically
                auto neighbors = it->second;
                std::sort(neighbors.begin(), neighbors.end(),
                          [](const auto& a, const auto& b) {
                              return a.first < b.first;
                          });

                for (size_t i = 0; i < neighbors.size(); ++i) {
                    if (i > 0) {
                        std::cout << ", ";
                    }
                    std::cout << neighbors[i].first << " (Cost: " << neighbors[i].second << ")";
                }
            }
            std::cout << "\n";
        }
    }

    // Read-only accessors
    const std::unordered_map<std::string, Router>& getRouters() const {
        return routers_;
    }

    const std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>& getAdjacencyList() const {
        return adjacencyList_;
    }
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
    // Computes shortest path between source and destination (skeleton stub)
    RouteResult findShortestPath(const Network& network, const std::string& source, const std::string& destination) {
        // Prevent unused parameter warnings during skeleton stage
        (void)network;
        (void)source;
        (void)destination;

        RouteResult result;
        std::cout << "[RoutingEngine] Shortest path algorithm will be implemented in the next stage.\n";
        return result;
    }

    // Displays the routing table for all known routers (skeleton stub)
    void displayRoutingTable(const Network& network) const {
        (void)network;
        std::cout << "[RoutingEngine] Routing table generation will be implemented in the next stage.\n";
    }
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
    Packet(const std::string& id, const std::string& source, const std::string& destination)
        : id_(id), source_(source), destination_(destination), status_(PacketStatus::Created) {}

    const std::string& getId() const { return id_; }
    const std::string& getSource() const { return source_; }
    const std::string& getDestination() const { return destination_; }
    const std::vector<std::string>& getPath() const { return path_; }
    PacketStatus getStatus() const { return status_; }

    void setPath(const std::vector<std::string>& path) { path_ = path; }
    void setStatus(PacketStatus status) { status_ = status; }

    static std::string statusToString(PacketStatus status) {
        switch (status) {
            case PacketStatus::Created:   return "Created";
            case PacketStatus::InTransit: return "In Transit";
            case PacketStatus::Delivered: return "Delivered";
            case PacketStatus::Dropped:   return "Dropped";
        }
        return "Unknown";
    }
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
    void sendPacket(Packet packet, const Network& network, RoutingEngine& engine) {
        (void)network;
        (void)engine;

        packetsCreated_++;
        std::cout << "[PacketSimulator] Simulating packet transmission for ID: " << packet.getId() << "\n";
        std::cout << "[PacketSimulator] Packet forwarding engine will be implemented in the next stage.\n";

        // Mark as created/logged in skeleton
        packetHistory_.push_back(packet);
    }

    void displayHistory() const {
        std::cout << "\n========================================\n";
        std::cout << "         PACKET HISTORY\n";
        std::cout << "========================================\n";

        if (packetHistory_.empty()) {
            std::cout << "No packets have been transmitted yet.\n";
            return;
        }

        for (const auto& pkt : packetHistory_) {
            std::cout << "Packet ID: " << pkt.getId()
                      << " | From: " << pkt.getSource()
                      << " -> To: " << pkt.getDestination()
                      << " | Status: " << Packet::statusToString(pkt.getStatus()) << "\n";
        }
        std::cout << "========================================\n";
    }

    void displayStatistics() const {
        std::cout << "\n========================================\n";
        std::cout << "         NETWORK STATISTICS\n";
        std::cout << "========================================\n";
        std::cout << "Total Packets Created   : " << packetsCreated_ << "\n";
        std::cout << "Total Packets Delivered : " << packetsDelivered_ << "\n";
        std::cout << "Total Packets Dropped   : " << packetsDropped_ << "\n";
        std::cout << "========================================\n";
    }
};

// ============================================================================
// SECTION 4: HELPER FUNCTIONS & INPUT UTILITIES
// ============================================================================

/**
 * @brief Safely reads a non-empty string line from standard input.
 */
static std::string readString(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, input)) {
            if (std::cin.eof()) {
                return "";
            }
            std::cin.clear();
            return "";
        }
        // Trim leading and trailing whitespace
        size_t start = input.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            std::cout << "[Warning] Input cannot be blank. Please try again.\n";
            continue;
        }
        size_t end = input.find_last_not_of(" \t\r\n");
        return input.substr(start, end - start + 1);
    }
}

/**
 * @brief Safely reads an integer from standard input with validation.
 */
static int readInteger(const std::string& prompt) {
    while (true) {
        std::string raw = readString(prompt);
        if (raw.empty() && std::cin.eof()) {
            return 0; // Graceful fallback on input stream closure
        }
        try {
            size_t idx = 0;
            int value = std::stoi(raw, &idx);
            if (idx == raw.length()) {
                return value;
            }
        } catch (...) {
            // Conversion failed
        }
        std::cout << "[Warning] Invalid number. Please enter a valid integer.\n";
    }
}

// ============================================================================
// SECTION 5: MENU HANDLING & SIMULATOR COORDINATION
// ============================================================================

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

    void handleAddRouter() {
        std::string id = readString("Enter router ID: ");
        OperationResult result = network_.addRouter(id);
        if (result.success) {
            std::cout << "[Success] " << result.message << "\n";
        } else {
            std::cout << "[Error] " << result.message << "\n";
        }
    }

    void handleRemoveRouter() {
        std::string id = readString("Enter router ID to remove: ");
        OperationResult result = network_.removeRouter(id);
        if (result.success) {
            std::cout << "[Success] " << result.message << "\n";
        } else {
            std::cout << "[Error] " << result.message << "\n";
        }
    }

    void handleConnectRouters() {
        std::string src = readString("Enter first router ID: ");
        std::string dst = readString("Enter second router ID: ");
        int cost = readInteger("Enter link cost: ");
        OperationResult result = network_.connectRouters(src, dst, cost);
        if (result.success) {
            std::cout << "[Success] " << result.message << "\n";
        } else {
            std::cout << "[Error] " << result.message << "\n";
        }
    }

    void handleDisconnectRouters() {
        std::string src = readString("Enter first router ID: ");
        std::string dst = readString("Enter second router ID: ");
        OperationResult result = network_.disconnectRouters(src, dst);
        if (result.success) {
            std::cout << "[Success] " << result.message << "\n";
        } else {
            std::cout << "[Error] " << result.message << "\n";
        }
    }

    void handleDisplayTopology() const {
        network_.displayTopology();
    }

    void handleFindShortestRoute() {
        std::string src = readString("Enter source router ID: ");
        std::string dst = readString("Enter destination router ID: ");
        routingEngine_.findShortestPath(network_, src, dst);
    }

    void handleDisplayRoutingTable() const {
        routingEngine_.displayRoutingTable(network_);
    }

    void handleSendPacket() {
        std::string pid = readString("Enter packet ID (e.g., PKT-101): ");
        std::string src = readString("Enter source router ID: ");
        std::string dst = readString("Enter destination router ID: ");

        if (!network_.hasRouter(src) || !network_.hasRouter(dst)) {
            std::cout << "[Error] Both source and destination routers must exist in the network.\n";
            return;
        }

        Packet pkt(pid, src, dst);
        packetSimulator_.sendPacket(pkt, network_, routingEngine_);
    }

    void handleDisplayPacketHistory() const {
        packetSimulator_.displayHistory();
    }

    void handleDisplayStatistics() const {
        packetSimulator_.displayStatistics();
    }

    void printMenu() const {
        std::cout << "\n========================================\n"
                  << "         NETROUTE SIMULATOR\n"
                  << "========================================\n"
                  << "1. Add Router\n"
                  << "2. Remove Router\n"
                  << "3. Connect Routers\n"
                  << "4. Disconnect Routers\n"
                  << "5. Display Network Topology\n"
                  << "6. Find Shortest Route\n"
                  << "7. Display Routing Table\n"
                  << "8. Send Packet\n"
                  << "9. Display Packet History\n"
                  << "10. Display Network Statistics\n"
                  << "0. Exit\n"
                  << "Enter choice: ";
    }

public:
    void run() {
        bool running = true;
        while (running) {
            printMenu();
            int choice = readInteger("");

            switch (choice) {
                case 1:  handleAddRouter(); break;
                case 2:  handleRemoveRouter(); break;
                case 3:  handleConnectRouters(); break;
                case 4:  handleDisconnectRouters(); break;
                case 5:  handleDisplayTopology(); break;
                case 6:  handleFindShortestRoute(); break;
                case 7:  handleDisplayRoutingTable(); break;
                case 8:  handleSendPacket(); break;
                case 9:  handleDisplayPacketHistory(); break;
                case 10: handleDisplayStatistics(); break;
                case 0:
                    std::cout << "Exiting NetRoute Simulator. Goodbye!\n";
                    running = false;
                    break;
                default:
                    std::cout << "[Warning] Invalid choice. Please select an option between 0 and 10.\n";
                    break;
            }
        }
    }
};

// ============================================================================
// SECTION 6: MAIN FUNCTION
// ============================================================================

int main() {
    NetworkSimulator simulator;
    simulator.run();
    return 0;
}
