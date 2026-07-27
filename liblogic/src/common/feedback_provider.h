//
// Created by vscode on 7/26/26.
//

#ifndef ELOQUENTLOGIC_FEEDBACK_PROVIDER_H
#define ELOQUENTLOGIC_FEEDBACK_PROVIDER_H
#include <memory>

#include "feedback_base.hpp"
namespace eloquent::logic {
    class feedback_provider {
        public:
        virtual ~feedback_provider() = default;
        virtual void report(std::unique_ptr<feedback_base> f) {}
    };
};
#endif //ELOQUENTLOGIC_FEEDBACK_PROVIDER_H
