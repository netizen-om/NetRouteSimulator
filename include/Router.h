#ifndef ROUTER_H
#define ROUTER_H

#include <string>

using namespace std;

// Represents a network router
class Router {
private:
    string id_;
    bool active_;

public:
    Router(string id);

    string getId() const;
    bool isActive() const;
    void setActive(bool active);
};

#endif // ROUTER_H
