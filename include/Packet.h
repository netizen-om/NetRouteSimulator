#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <vector>

/**
 * @brief Represents the lifecycle states of a network packet.
 */
enum class PacketStatus {
    CREATED,
    IN_TRANSIT,
    DELIVERED,
    FAILED
};

/**
 * @brief Helper function to convert PacketStatus enum to string.
 */
std::string packetStatusToString(PacketStatus status);

/**
 * @class Packet
 * @brief Represents a single data packet travelling across the network.
 * 
 * Single Responsibility:
 * Holds packet metadata (id, source, destination, hop path, current status, cost).
 */
class Packet {
private:
    int id_;
    std::string source_;
    std::string destination_;
    std::vector<std::string> path_;
    PacketStatus status_;
    long long totalCost_;
    std::string failureReason_;

public:
    Packet(int id, const std::string& source, const std::string& destination);

    int getId() const;
    const std::string& getSource() const;
    const std::string& getDestination() const;
    const std::vector<std::string>& getPath() const;
    PacketStatus getStatus() const;
    long long getTotalCost() const;
    int getHops() const;
    const std::string& getFailureReason() const;

    void recordHop(const std::string& routerId);
    void setPath(const std::vector<std::string>& path);
    void setStatus(PacketStatus status);
    void setTotalCost(long long cost);
    void setFailureReason(const std::string& reason);

    static std::string statusToString(PacketStatus status);
};

#endif // PACKET_H
