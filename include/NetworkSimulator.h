#ifndef NETWORK_SIMULATOR_H
#define NETWORK_SIMULATOR_H

#include "Network.h"
#include "RoutingEngine.h"
#include "PacketSimulator.h"

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
    void handleSetRouterActive();
    void handleLoadDemonstrationNetwork();
    void printMenu() const;

public:
    void run();
};

#endif // NETWORK_SIMULATOR_H
