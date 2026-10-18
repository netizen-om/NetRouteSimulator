// ============================================================================
// File: PacketSimulator.cpp
// Description: Implementation of PacketSimulator class methods.
//              Handles simulation history, statistics, and placeholders for
//              packet forwarding (to be implemented in subsequent prompts).
// ============================================================================

#include "ProjectDeclarations.cpp"

void PacketSimulator::sendPacket(Packet packet, const Network& network, RoutingEngine& engine) {
    (void)network;
    (void)engine;

    packetsCreated_++;
    std::cout << "[PacketSimulator] Simulating packet transmission for ID: " << packet.getId() << "\n";
    std::cout << "[PacketSimulator] Packet forwarding engine will be implemented in the next stage.\n";

    // Mark as created and log into history for skeleton verification
    packetHistory_.push_back(packet);
}

void PacketSimulator::displayHistory() const {
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

void PacketSimulator::displayStatistics() const {
    std::cout << "\n========================================\n";
    std::cout << "         NETWORK STATISTICS\n";
    std::cout << "========================================\n";
    std::cout << "Total Packets Created   : " << packetsCreated_ << "\n";
    std::cout << "Total Packets Delivered : " << packetsDelivered_ << "\n";
    std::cout << "Total Packets Dropped   : " << packetsDropped_ << "\n";
    std::cout << "========================================\n";
}
