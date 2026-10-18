// ============================================================================
// File: NetworkSimulator.cpp
// Description: Implementation of NetworkSimulator class methods, CLI menu loop,
//              safe input utilities, and coordination between subsystems.
// ============================================================================

#include "ProjectDeclarations.cpp"

// ----------------------------------------------------------------------------
// Local Input Helper Functions
// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------
// NetworkSimulator Member Implementations
// ----------------------------------------------------------------------------

void NetworkSimulator::handleAddRouter() {
    std::string id = readString("Enter router ID: ");
    OperationResult result = network_.addRouter(id);
    if (result.success) {
        std::cout << "[Success] " << result.message << "\n";
    } else {
        std::cout << "[Error] " << result.message << "\n";
    }
}

void NetworkSimulator::handleRemoveRouter() {
    std::string id = readString("Enter router ID to remove: ");
    OperationResult result = network_.removeRouter(id);
    if (result.success) {
        std::cout << "[Success] " << result.message << "\n";
    } else {
        std::cout << "[Error] " << result.message << "\n";
    }
}

void NetworkSimulator::handleConnectRouters() {
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

void NetworkSimulator::handleDisconnectRouters() {
    std::string src = readString("Enter first router ID: ");
    std::string dst = readString("Enter second router ID: ");
    OperationResult result = network_.disconnectRouters(src, dst);
    if (result.success) {
        std::cout << "[Success] " << result.message << "\n";
    } else {
        std::cout << "[Error] " << result.message << "\n";
    }
}

void NetworkSimulator::handleDisplayTopology() const {
    network_.displayTopology();
}

void NetworkSimulator::handleFindShortestRoute() {
    std::string src = readString("Enter source router ID: ");
    std::string dst = readString("Enter destination router ID: ");
    routingEngine_.findShortestPath(network_, src, dst);
}

void NetworkSimulator::handleDisplayRoutingTable() const {
    routingEngine_.displayRoutingTable(network_);
}

void NetworkSimulator::handleSendPacket() {
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

void NetworkSimulator::handleDisplayPacketHistory() const {
    packetSimulator_.displayHistory();
}

void NetworkSimulator::handleDisplayStatistics() const {
    packetSimulator_.displayStatistics();
}

void NetworkSimulator::printMenu() const {
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

void NetworkSimulator::run() {
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
