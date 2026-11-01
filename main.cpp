#include <iostream>
#include "Network.h"

using namespace std;

int main() {
    Network network;

    int choice;

    do {
        cout << "\n========================================\n";
        cout << "          NETROUTE SIMULATOR\n";
        cout << "========================================\n";

        cout << "1. Add Router\n";
        cout << "2. Connect Routers\n";
        cout << "3. Remove Router\n";
        cout << "4. Display Network\n";
        cout << "5. Find Router Neighbors\n";
        cout << "6. Find Shortest Route\n";
        cout << "7. Check Network Connectivity\n";
        cout << "0. Exit\n";

        cout << "========================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                network.addRouter();
                break;

            case 2:
                network.connectRouters();
                break;

            case 3:
                network.removeRouter();
                break;

            case 4:
                network.displayNetwork();
                break;

            case 5:
                network.findNeighbors();
                break;

            case 6:
                network.findShortestRoute();
                break;

            case 7:
                network.checkConnectivity();
                break;

            case 0:
                cout << "Exiting NetRoute Simulator. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}