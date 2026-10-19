#include "Packet.h"

using namespace std;

string packetStatusToString(PacketStatus status) {
    return Packet::statusToString(status);
}

Packet::Packet(int id, string source, string destination) {
    id_ = id;
    source_ = source;
    destination_ = destination;
    status_ = PacketStatus::CREATED;
    totalCost_ = 0;
    failureReason_ = "";
}

int Packet::getId() const {
    return id_;
}

string Packet::getSource() const {
    return source_;
}

string Packet::getDestination() const {
    return destination_;
}

const vector<string>& Packet::getPath() const {
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

string Packet::getFailureReason() const {
    return failureReason_;
}

void Packet::recordHop(string routerId) {
    path_.push_back(routerId);
}

void Packet::setPath(const vector<string>& path) {
    path_ = path;
}

void Packet::setStatus(PacketStatus status) {
    status_ = status;
}

void Packet::setTotalCost(long long cost) {
    totalCost_ = cost;
}

void Packet::setFailureReason(string reason) {
    failureReason_ = reason;
}

string Packet::statusToString(PacketStatus status) {
    switch (status) {
        case PacketStatus::CREATED:    return "CREATED";
        case PacketStatus::IN_TRANSIT: return "IN_TRANSIT";
        case PacketStatus::DELIVERED:  return "DELIVERED";
        case PacketStatus::FAILED:     return "FAILED";
    }
    return "UNKNOWN";
}
