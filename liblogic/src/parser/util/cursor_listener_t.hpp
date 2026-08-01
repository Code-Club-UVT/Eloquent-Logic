//
// Created by vscode on 7/31/26.
//

#ifndef ELOQUENTLOGIC_CURSOR_LISTENER_T_HPP
#define ELOQUENTLOGIC_CURSOR_LISTENER_T_HPP
#include "feedback_listener.hpp"

namespace eloquent::logic
{
    class cursor_listener_t : public feedback_listener
    {
        public:
        virtual void didGoUp(){}

    };
}


#endif //ELOQUENTLOGIC_CURSOR_LISTENER_T_HPP
