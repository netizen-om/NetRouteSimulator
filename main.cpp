#include <iostream>
#include <string>
#include <vector>
#include "Network.h"

/**
 * @brief Handles user interface, console interaction, and application lifecycle.
 *
 * Demonstrates separation of concerns by keeping all I/O, menu logic,
 * and user-input validation completely decoupled from the core Network domain class.
 */
class NetworkApp {
private:
    Network network; ///< Composed Network object (Composition: NetworkApp has-a Network)

    /**
     * @brief Safely reads an integer from std::cin with robust error recovery.
     * @param outValue Reference where the valid parsed integer will be stored.
     * @return True if an integer was read successfully, false on invalid input or EOF.
     */
    static bool readInt(int& outValue) {
        int temp = 0;
        if (!(std::cin >> temp)) {
            if (std::cin.eof()) {
                return false;
            }
            std::cin.clear();
            std::string dummy;
            std::cin >> dummy;
            std::cout << "Invalid input. Please enter a valid number.\n";
            return false;
        }
        outValue = temp;
        return true;
    }

    /**
     * @brief Displays the interactive CLI menu options.
     */
    void printMenu() const {
        std::cout << "\n========================================\n";
        std::cout << "         NETROUTE SIMULATOR\n";
        std::cout << "========================================\n";
        std::cout << "1. Add Router\n";
        std::cout << "2. Connect Routers\n";
        std::cout << "3. Display Network\n";
        std::cout << "4. Find Shortest Route\n";
        std::cout << "0. Exit\n";
        std::cout << "========================================\n";
    }

    /**
     * @brief Option 1: Adds a new router node to the network.
     */
    void handleAddRouter() {
        int newId = network.addRouter();
        std::cout << "Router added successfully with ID: " << newId << "\n";
    }

    /**
     * @brief Option 2: Connects two existing routers with a weighted bidirectional link.
     */
    void handleConnectRouters() {
        int router1 = 0;
        int router2 = 0;
        int cost = 0;

        std::cout << "Enter first router ID: ";
        if (!readInt(router1)) {
            return;
        }
        std::cout << "Enter second router ID: ";
        if (!readInt(router2)) {
            return;
        }
        std::cout << "Enter link cost: ";
        if (!readInt(cost)) {
            return;
        }

        ConnectionResult status = network.connectRouters(router1, router2, cost);

        switch (status) {
            case ConnectionResult::InvalidRouterId:
                std::cout << "Error: One or both router IDs do not exist.\n";
                break;
            case ConnectionResult::SelfConnection:
                std::cout << "Error: Cannot connect a router to itself.\n";
                break;
            case ConnectionResult::NonPositiveCost:
                std::cout << "Error: Link cost must be a positive integer.\n";
                break;
            case ConnectionResult::DuplicateConnection:
                std::cout << "Error: A connection already exists between router "
                          << router1 << " and router " << router2 << ".\n";
                break;
            case ConnectionResult::Success:
                std::cout << "Successfully connected router " << router1 << " and router "
                          << router2 << " with cost " << cost << ".\n";
                break;
        }
    }

    /**
     * @brief Option 3: Displays the complete network topology including isolated routers.
     */
    void handleDisplayNetwork() const {
        if (network.isEmpty()) {
            std::cout << "The network is empty. No routers added yet.\n";
            return;
        }

        std::cout << "\n========================================\n";
        std::cout << "           NETWORK TOPOLOGY\n";
        std::cout << "========================================\n";

        for (int i = 1; i <= network.getRouterCount(); ++i) {
            std::cout << "Router " << i << " -> ";
            const std::vector<Edge>& neighbors = network.getNeighbors(i);
            if (neighbors.empty()) {
                std::cout << "[No connections (Isolated)]\n";
            } else {
                for (size_t j = 0; j < neighbors.size(); ++j) {
                    if (j > 0) {
                        std::cout << ", ";
                    }
                    std::cout << "Router " << neighbors[j].to
                              << " (Cost: " << neighbors[j].cost << ")";
                }
                std::cout << "\n";
            }
        }
        std::cout << "========================================\n";
    }

    /**
     * @brief Option 4: Finds and displays the shortest route using Dijkstra's algorithm.
     */
    void handleFindShortestRoute() const {
        int source = 0;
        int destination = 0;

        std::cout << "Enter source router ID: ";
        if (!readInt(source)) {
            return;
        }
        std::cout << "Enter destination router ID: ";
        if (!readInt(destination)) {
            return;
        }

        RouteResult result = network.findShortestRoute(source, destination);

        // Validation failure (e.g. router does not exist)
        if (!result.success) {
            std::cout << "Error: Invalid router ID. Router does not exist.\n";
            return;
        }

        // Trivial case: source is destination
        if (result.sameSourceDest) {
            std::cout << "\n========================================\n";
            std::cout << "            SHORTEST ROUTE\n";
            std::cout << "========================================\n";
            std::cout << "Source Router: " << source << "\n";
            std::cout << "Destination Router: " << destination << "\n";
            std::cout << "Shortest Path: " << source << "\n";
            std::cout << "Total Cost: 0\n";
            std::cout << "========================================\n";
            return;
        }

        // Unreachable destination
        if (!result.routeExists) {
            std::cout << "No route exists between these routers.\n";
            return;
        }

        // Output path and minimum cost
        std::cout << "\n========================================\n";
        std::cout << "            SHORTEST ROUTE\n";
        std::cout << "========================================\n";
        std::cout << "Source Router: " << source << "\n";
        std::cout << "Destination Router: " << destination << "\n";
        std::cout << "Shortest Path: ";
        for (size_t i = 0; i < result.path.size(); ++i) {
            if (i > 0) {
                std::cout << " -> ";
            }
            std::cout << result.path[i];
        }
        std::cout << "\nTotal Cost: " << result.totalCost << "\n";
        std::cout << "========================================\n";
    }

public:
    /**
     * @brief Launches the application loop.
     */
    void run() {
        int choice = -1;

        do {
            printMenu();
            std::cout << "Enter your choice: ";

            if (!readInt(choice)) {
                if (std::cin.eof()) {
                    std::cout << "\nEnd of input detected. Exiting.\n";
                    break;
                }
                continue;
            }

            switch (choice) {
                case 1:
                    handleAddRouter();
                    break;
                case 2:
                    handleConnectRouters();
                    break;
                case 3:
                    handleDisplayNetwork();
                    break;
                case 4:
                    handleFindShortestRoute();
                    break;
                case 0:
                    std::cout << "Exiting NetRoute Simulator. Goodbye!\n";
                    break;
                default:
                    std::cout << "Invalid choice. Please select an option between 0 and 4.\n";
                    break;
            }
        } while (choice != 0);
    }
};

int main() {
    NetworkApp app;
    app.run();
    return 0;
}
