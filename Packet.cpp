// ============================================================================
// File: Packet.cpp
// Description: Implementation of Packet class methods and status conversions.
// ============================================================================

#include "ProjectDeclarations.cpp"

std::string packetStatusToString(PacketStatus status) {
    return Packet::statusToString(status);
}

Packet::Packet(int id, const std::string& source, const std::string& destination)
    : id_(id), source_(source), destination_(destination),
      status_(PacketStatus::CREATED), totalCost_(0), failureReason_("") {}

int Packet::getId() const {
    return id_;
}

const std::string& Packet::getSource() const {
    return source_;
}

const std::string& Packet::getDestination() const {
    return destination_;
}

const std::vector<std::string>& Packet::getPath() const {
    return path_;
}

PacketStatus Packet::getStatus() const {
    return status_;
}

long long Packet::getTotalCost() const {
    return totalCost_;
}

int Packet::getHops() const {
    if (path_.empty()) {
        return 0;
    }
    return static_cast<int>(path_.size() - 1);
}

const std::string& Packet::getFailureReason() const {
    return failureReason_;
}

void Packet::recordHop(const std::string& routerId) {
    path_.push_back(routerId);
}

void Packet::setPath(const std::vector<std::string>& path) {
    path_ = path;
}

void Packet::setStatus(PacketStatus status) {
    status_ = status;
}

void Packet::setTotalCost(long long cost) {
    totalCost_ = cost;
}

void Packet::setFailureReason(const std::string& reason) {
    failureReason_ = reason;
}

std::string Packet::statusToString(PacketStatus status) {
    switch (status) {
        case PacketStatus::CREATED:    return "CREATED";
        case PacketStatus::IN_TRANSIT:  return "IN_TRANSIT";
        case PacketStatus::DELIVERED:   return "DELIVERED";
        case PacketStatus::FAILED:      return "FAILED";
    }
    return "UNKNOWN";
}
