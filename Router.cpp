// ============================================================================
// File: Router.cpp
// Description: Implementation of Router class methods.
// ============================================================================

#include "ProjectDeclarations.cpp"

Router::Router(const std::string& id)
    : id_(id), active_(true) {}

const std::string& Router::getId() const {
    return id_;
}

bool Router::isActive() const {
    return active_;
}

void Router::setActive(bool active) {
    active_ = active;
}
