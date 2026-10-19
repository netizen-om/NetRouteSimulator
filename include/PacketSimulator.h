#ifndef PACKET_SIMULATOR_H
#define PACKET_SIMULATOR_H

#include "Packet.h"

#include <vector>
#include <string>

using namespace std;

class Network;
class RoutingEngine;

// Handles packet creation, forwarding simulation, history, and statistics
class PacketSimulator {
private:
    int nextPacketId_ = 1001;
    vector<Packet> packetHistory_;
    int packetsCreated_ = 0;
    int packetsDelivered_ = 0;
    int packetsFailed_ = 0;
    long long totalDeliveredHops_ = 0;

public:
    void sendPacket(string source, string destination, const Network& network, RoutingEngine& engine);
    void displayHistory() const;
    void displayStatistics(const Network& network) const;
};

#endif // PACKET_SIMULATOR_H
