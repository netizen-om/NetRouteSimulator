#include "PacketSimulator.h"
#include "Network.h"
#include "RoutingEngine.h"

#include <iostream>
#include <iomanip>

using namespace std;

void PacketSimulator::sendPacket(string source, string destination,
                                 const Network& network, RoutingEngine& engine) {
    // 1. Create a packet with a unique auto-incrementing ID
    int id = nextPacketId_++;
    Packet packet(id, source, destination);
    packetsCreated_++;

    // 2. Ask the RoutingEngine for the shortest route
    RouteResult route = engine.findShortestRoute(network, source, destination);

    // 3. If no route exists, mark FAILED and store in history
    if (!route.found) {
        packet.setStatus(PacketStatus::FAILED);
        packet.setFailureReason("No route available");
        packetHistory_.push_back(packet);
        packetsFailed_++;

        cout << "\n========================================\n"
             << "           PACKET SIMULATION\n"
             << "========================================\n"
             << "Packet ID: " << packet.getId() << "\n"
             << "Source: " << packet.getSource() << "\n"
             << "Destination: " << packet.getDestination() << "\n\n"
             << "Status: FAILED\n"
             << "Reason: No route available\n"
             << "========================================\n";
        return;
    }

    // 4. Route exists: set status to IN_TRANSIT
    packet.setStatus(PacketStatus::IN_TRANSIT);

    cout << "\n========================================\n"
         << "           PACKET SIMULATION\n"
         << "========================================\n"
         << "Packet ID: " << packet.getId() << "\n"
         << "Source: " << packet.getSource() << "\n"
         << "Destination: " << packet.getDestination() << "\n\n"
         << "Forwarding:\n";

    // 5. Simulate forwarding along the path and record each visited router
    if (route.path.size() == 1) {
        cout << route.path[0] << " (Local delivery)\n";
        packet.recordHop(route.path[0]);
    } else {
        for (size_t i = 0; i < route.path.size() - 1; ++i) {
            cout << route.path[i] << " -> " << route.path[i + 1] << "\n";
            packet.recordHop(route.path[i]);
        }
        packet.recordHop(route.path.back());
    }

    // 6. Mark packet DELIVERED and record total cost
    packet.setStatus(PacketStatus::DELIVERED);
    packet.setTotalCost(route.totalCost);
    packetsDelivered_++;
    totalDeliveredHops_ += packet.getHops();

    // 7. Store completed packet in history
    packetHistory_.push_back(packet);

    // 8. Display final result
    cout << "\nActual Path: ";
    for (size_t i = 0; i < packet.getPath().size(); ++i) {
        if (i > 0) cout << " -> ";
        cout << packet.getPath()[i];
    }
    cout << "\nTotal Cost: " << packet.getTotalCost() << "\n"
         << "Number of Hops: " << packet.getHops() << "\n"
         << "Status: DELIVERED\n"
         << "========================================\n";
}

void PacketSimulator::displayHistory() const {
    cout << "\n======================================================================\n"
         << "                         PACKET HISTORY\n"
         << "======================================================================\n";

    if (packetHistory_.empty()) {
        cout << "No packets have been simulated yet. Use option 5 to send a packet.\n"
             << "======================================================================\n";
        return;
    }

    for (const auto& pkt : packetHistory_) {
        cout << "Packet ID: " << pkt.getId()
             << " | From: " << pkt.getSource()
             << " -> To: " << pkt.getDestination()
             << " | Status: " << Packet::statusToString(pkt.getStatus()) << "\n";

        if (pkt.getStatus() == PacketStatus::DELIVERED) {
            cout << "  Path: ";
            for (size_t i = 0; i < pkt.getPath().size(); ++i) {
                if (i > 0) cout << " -> ";
                cout << pkt.getPath()[i];
            }
            cout << "\n  Hops: " << pkt.getHops()
                 << " | Cost: " << pkt.getTotalCost() << "\n";
        } else if (pkt.getStatus() == PacketStatus::FAILED) {
            cout << "  Reason: " << pkt.getFailureReason() << "\n";
        }
        cout << "----------------------------------------------------------------------\n";
    }
    cout << "======================================================================\n";
}

void PacketSimulator::displayStatistics(const Network& network) const {
    double successRate = (packetsCreated_ > 0)
        ? (static_cast<double>(packetsDelivered_) * 100.0 / packetsCreated_)
        : 0.0;

    double avgHops = (packetsDelivered_ > 0)
        ? (static_cast<double>(totalDeliveredHops_) / packetsDelivered_)
        : 0.0;

    cout << "\n========================================\n"
         << "         NETWORK STATISTICS\n"
         << "========================================\n"
         << left << setw(28) << "Total Routers"
         << ": " << network.getRouterCount() << "\n"
         << left << setw(28) << "Total Bidirectional Links"
         << ": " << network.getLinkCount() << "\n"
         << left << setw(28) << "Total Packets Created"
         << ": " << packetsCreated_ << "\n"
         << left << setw(28) << "Successfully Delivered"
         << ": " << packetsDelivered_ << "\n"
         << left << setw(28) << "Failed Packets"
         << ": " << packetsFailed_ << "\n";

    cout << fixed << setprecision(2);
    if (packetsCreated_ > 0) {
        cout << left << setw(28) << "Delivery Success Rate"
             << ": " << successRate << "%\n";
    } else {
        cout << left << setw(28) << "Delivery Success Rate"
             << ": N/A (0 packets)\n";
    }

    if (packetsDelivered_ > 0) {
        cout << left << setw(28) << "Average Delivered Hops"
             << ": " << avgHops << "\n";
    } else {
        cout << left << setw(28) << "Average Delivered Hops"
             << ": N/A (0 delivered)\n";
    }
    cout << "========================================\n";
}
