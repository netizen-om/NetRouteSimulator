#ifndef NETWORK_SIMULATOR_H
#define NETWORK_SIMULATOR_H

#include "Network.h"
#include "RoutingEngine.h"
#include "PacketSimulator.h"

using namespace std;

// Controller class: manages the menu and connects user inputs to simulator logic
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
    void handleSetRouterActive();
    void handleLoadDemonstrationNetwork();
    void printMenu() const;

public:
    void run();
};

#endif // NETWORK_SIMULATOR_H
