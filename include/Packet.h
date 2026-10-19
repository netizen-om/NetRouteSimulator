#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <vector>

using namespace std;

// States of a packet during its lifecycle
enum class PacketStatus {
    CREATED,
    IN_TRANSIT,
    DELIVERED,
    FAILED
};

string packetStatusToString(PacketStatus status);

// Represents a data packet in the simulated network
class Packet {
private:
    int id_;
    string source_;
    string destination_;
    vector<string> path_;
    PacketStatus status_;
    long long totalCost_;
    string failureReason_;

public:
    Packet(int id, string source, string destination);

    int getId() const;
    string getSource() const;
    string getDestination() const;
    const vector<string>& getPath() const;
    PacketStatus getStatus() const;
    long long getTotalCost() const;
    int getHops() const;
    string getFailureReason() const;

    void recordHop(string routerId);
    void setPath(const vector<string>& path);
    void setStatus(PacketStatus status);
    void setTotalCost(long long cost);
    void setFailureReason(string reason);

    static string statusToString(PacketStatus status);
};

#endif // PACKET_H
