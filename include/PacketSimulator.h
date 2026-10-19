#ifndef PACKET_SIMULATOR_H
#define PACKET_SIMULATOR_H

#include "Packet.h"

#include <vector>
#include <string>

// Forward declarations to decouple from Network and RoutingEngine headers
class Network;
class RoutingEngine;

/**
 * @class PacketSimulator
 * @brief Manages packet creation, forwarding simulation, history, and delivery stats.
 * 
 * Single Responsibility:
 * Handles logical packet transmission simulation, tracking history and metrics.
 */
class PacketSimulator {
private:
    int nextPacketId_ = 1001;
    std::vector<Packet> packetHistory_;
    int packetsCreated_ = 0;
    int packetsDelivered_ = 0;
    int packetsFailed_ = 0;
    long long totalDeliveredHops_ = 0;

public:
    void sendPacket(const std::string& source, const std::string& destination, const Network& network, RoutingEngine& engine);
    void displayHistory() const;
    void displayStatistics(const Network& network) const;
};

#endif // PACKET_SIMULATOR_H
