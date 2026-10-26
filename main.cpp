#include <iostream>
#include <string>
#include "Network.h"

using namespace std;

int main() {
    Network network;
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
                network.addRouter();
                break;
            case 2:
                network.connectRouters();
                break;
            case 3:
                network.displayNetwork();
                break;
            case 4:
                network.findShortestRoute();
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
