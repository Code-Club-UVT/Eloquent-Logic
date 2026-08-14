//
// Created by vscode on 7/30/26.
//

#ifndef ELOQUENTLOGIC_FEEDBACK_LISTENER_HPP
#define ELOQUENTLOGIC_FEEDBACK_LISTENER_HPP

namespace eloquent::logic
{
    class feedback_listener
    {
    public:
        virtual ~feedback_listener() = default;
        virtual void didStart(){}
        virtual void didFinish(){}
    };
}
#endif //ELOQUENTLOGIC_FEEDBACK_LISTENER_HPP
