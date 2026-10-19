#include "NetworkSimulator.h"

#include <iostream>
#include <string>

using namespace std;

// ----------------------------------------------------------------------------
// NetworkSimulator Member Implementations
// ----------------------------------------------------------------------------

void NetworkSimulator::handleAddRouter() {
    string id;
    cout << "Enter router ID: ";
    cin >> id;

    OperationResult result = network_.addRouter(id);
    if (result.success) {
        cout << "[Success] " << result.message << "\n";
    } else {
        cout << "[Error] " << result.message << "\n";
    }
}

void NetworkSimulator::handleRemoveRouter() {
    string id;
    cout << "Enter router ID to remove: ";
    cin >> id;

    OperationResult result = network_.removeRouter(id);
    if (result.success) {
        cout << "[Success] " << result.message << "\n";
    } else {
        cout << "[Error] " << result.message << "\n";
    }
}

void NetworkSimulator::handleConnectRouters() {
    string src, dst;
    int cost;
    cout << "Enter first router ID: ";
    cin >> src;
    cout << "Enter second router ID: ";
    cin >> dst;
    cout << "Enter link cost: ";
    cin >> cost;

    OperationResult result = network_.connectRouters(src, dst, cost);
    if (result.success) {
        cout << "[Success] " << result.message << "\n";
    } else {
        cout << "[Error] " << result.message << "\n";
    }
}

void NetworkSimulator::handleDisconnectRouters() {
    string src, dst;
    cout << "Enter first router ID: ";
    cin >> src;
    cout << "Enter second router ID: ";
    cin >> dst;

    OperationResult result = network_.disconnectRouters(src, dst);
    if (result.success) {
        cout << "[Success] " << result.message << "\n";
    } else {
        cout << "[Error] " << result.message << "\n";
    }
}

void NetworkSimulator::handleDisplayTopology() const {
    network_.displayTopology();
}

void NetworkSimulator::handleFindShortestRoute() {
    string src, dst;
    cout << "Enter source router ID: ";
    cin >> src;
    cout << "Enter destination router ID: ";
    cin >> dst;

    if (!network_.hasRouter(src)) {
        cout << "[Error] Source router '" << src << "' does not exist in the network.\n";
        return;
    }
    if (!network_.hasRouter(dst)) {
        cout << "[Error] Destination router '" << dst << "' does not exist in the network.\n";
        return;
    }
    if (!network_.isRouterActive(src)) {
        cout << "[Error] Source router '" << src << "' is currently INACTIVE.\n";
        return;
    }
    if (!network_.isRouterActive(dst)) {
        cout << "[Error] Destination router '" << dst << "' is currently INACTIVE.\n";
        return;
    }

    RouteResult result = routingEngine_.findShortestRoute(network_, src, dst);

    cout << "\n========================================\n"
         << "         SHORTEST ROUTE\n"
         << "========================================\n"
         << "Source Router: " << src << "\n"
         << "Destination Router: " << dst << "\n\n";

    if (result.found) {
        cout << "Shortest Route: ";
        for (size_t i = 0; i < result.path.size(); ++i) {
            if (i > 0) cout << " -> ";
            cout << result.path[i];
        }
        cout << "\n"
             << "Total Cost: " << result.totalCost << "\n"
             << "Number of Hops: " << result.hops << "\n"
             << "Status: Route Found\n";
    } else {
        cout << "Shortest Route: None\n"
             << "Total Cost: 0\n"
             << "Number of Hops: 0\n"
             << "Status: Destination Unreachable\n";
    }
    cout << "========================================\n";
}

void NetworkSimulator::handleDisplayRoutingTable() const {
    string src;
    cout << "Enter source router ID for routing table: ";
    cin >> src;
    routingEngine_.displayRoutingTable(network_, src);
}

void NetworkSimulator::handleSendPacket() {
    string src, dst;
    cout << "Enter source router ID: ";
    cin >> src;
    cout << "Enter destination router ID: ";
    cin >> dst;

    if (!network_.hasRouter(src)) {
        cout << "[Error] Source router '" << src << "' does not exist in the network.\n";
        return;
    }
    if (!network_.hasRouter(dst)) {
        cout << "[Error] Destination router '" << dst << "' does not exist in the network.\n";
        return;
    }
    if (!network_.isRouterActive(src)) {
        cout << "[Error] Source router '" << src << "' is currently INACTIVE.\n";
        return;
    }
    if (!network_.isRouterActive(dst)) {
        cout << "[Error] Destination router '" << dst << "' is currently INACTIVE.\n";
        return;
    }

    packetSimulator_.sendPacket(src, dst, network_, routingEngine_);
}

void NetworkSimulator::handleDisplayPacketHistory() const {
    packetSimulator_.displayHistory();
}

void NetworkSimulator::handleDisplayStatistics() const {
    packetSimulator_.displayStatistics(network_);
}

void NetworkSimulator::handleSetRouterActive() {
    string id;
    int stateChoice;
    cout << "Enter router ID: ";
    cin >> id;
    cout << "Set state (1 = Active, 0 = Inactive): ";
    cin >> stateChoice;

    OperationResult res = network_.setRouterActive(id, stateChoice != 0);
    if (res.success) {
        cout << "[Success] " << res.message << "\n";
    } else {
        cout << "[Error] " << res.message << "\n";
    }
}

void NetworkSimulator::handleLoadDemonstrationNetwork() {
    OperationResult res = network_.loadDemonstrationNetwork();
    cout << "[Success] " << res.message << "\n";
}

void NetworkSimulator::printMenu() const {
    cout << "\n========================================\n"
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
         << "11. Set Router Active/Inactive\n"
         << "12. Load Demonstration Network\n"
         << "0. Exit\n"
         << "Enter choice: ";
}

void NetworkSimulator::run() {
    int choice = -1;
    do {
        printMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            string dummy;
            cin >> dummy;
            cout << "[Warning] Invalid number. Please enter a valid integer.\n";
            continue;
        }

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
            case 11: handleSetRouterActive(); break;
            case 12: handleLoadDemonstrationNetwork(); break;
            case 0:
                cout << "Exiting NetRoute Simulator. Goodbye!\n";
                break;
            default:
                cout << "[Warning] Invalid choice. Please select an option between 0 and 12.\n";
                break;
        }
    } while (choice != 0);
}
