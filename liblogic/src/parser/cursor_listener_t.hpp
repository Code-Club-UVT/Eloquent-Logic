//
// Created by vscode on 7/31/26.
//

#ifndef ELOQUENTLOGIC_CURSOR_LISTENER_T_HPP
#define ELOQUENTLOGIC_CURSOR_LISTENER_T_HPP
#include "feedback_listener.hpp"
#include "node.h"

namespace eloquent::logic {
class cursor_listener_t : public feedback_listener {
  public:
    virtual void didGoUp() {}
    virtual void didTryInvalidPosition(NodeObsPtr, size_t) {}
    virtual void didTrySpawiningOverExistingNode(NodeObsPtr) {}
    virtual void spawnedNewChildNode(const NodeObsPtr &, const NodeObsPtr &) {}
    virtual void didPlaceNode() {}
    virtual void alreadyUp() {}
};
} // namespace eloquent::logic

#endif // ELOQUENTLOGIC_CURSOR_LISTENER_T_HPP
