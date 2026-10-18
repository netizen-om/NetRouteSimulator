// ============================================================================
// File: Packet.cpp
// Description: Implementation of Packet class methods and status conversions.
// ============================================================================

#include "ProjectDeclarations.cpp"

Packet::Packet(const std::string& id, const std::string& source, const std::string& destination)
    : id_(id), source_(source), destination_(destination), status_(PacketStatus::Created) {}

const std::string& Packet::getId() const {
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

void Packet::setPath(const std::vector<std::string>& path) {
    path_ = path;
}

void Packet::setStatus(PacketStatus status) {
    status_ = status;
}

std::string Packet::statusToString(PacketStatus status) {
    switch (status) {
        case PacketStatus::Created:   return "Created";
        case PacketStatus::InTransit: return "In Transit";
        case PacketStatus::Delivered: return "Delivered";
        case PacketStatus::Dropped:   return "Dropped";
    }
    return "Unknown";
}
