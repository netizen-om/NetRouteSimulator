#ifndef ROUTER_H
#define ROUTER_H

#include <string>

/**
 * @class Router
 * @brief Represents a single router node in the simulated network.
 * 
 * Single Responsibility:
 * Encapsulates router identity and operational state (active vs. inactive).
 */
class Router {
private:
    std::string id_;
    bool active_;

public:
    explicit Router(const std::string& id);

    const std::string& getId() const;
    bool isActive() const;
    void setActive(bool active);
};

#endif // ROUTER_H
