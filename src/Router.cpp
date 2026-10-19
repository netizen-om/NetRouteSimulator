#include "Router.h"

using namespace std;

Router::Router(string id) {
    id_ = id;
    active_ = true;
}

string Router::getId() const {
    return id_;
}

bool Router::isActive() const {
    return active_;
}

void Router::setActive(bool active) {
    active_ = active;
}
