//
// Created by vscode on 7/30/26.
//

#ifndef ELOQUENTLOGIC_FEEDBACK_VISITOR_HPP
#define ELOQUENTLOGIC_FEEDBACK_VISITOR_HPP

#include <memory>
#include <feedback_base.hpp>
namespace eloquent::logic
{
    class success;
    class error;
    class feedback_visitor
    {
        public:
        virtual void visit(std::shared_ptr<success> const& ) = 0;
        virtual void visit(std::shared_ptr<error> const&) = 0;

        virtual ~feedback_visitor() = default;
        feedback_visitor() = default;
        feedback_visitor(feedback_visitor const&) = delete;
        feedback_visitor(feedback_visitor&&) = default;
        feedback_visitor& operator=(feedback_visitor const&) = delete;
        feedback_visitor& operator=(feedback_visitor&&) = default;
    };
}

#endif //ELOQUENTLOGIC_FEEDBACK_VISITOR_HPP
