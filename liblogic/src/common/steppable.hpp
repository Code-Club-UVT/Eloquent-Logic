//
// Created by vscode on 7/19/26.
//

#ifndef ELOQUENTLOGIC_STEPPABLE_HPP
#define ELOQUENTLOGIC_STEPPABLE_HPP
#include <cstdint>
#include <vector>
#include "feedback_base.hpp"
#include <generator>
#include "steppable_result.hpp"
#include "type_definitions.hpp"

template<Feedback U, ResultType T=std::monostate>
class steppable {

protected:

    virtual std::generator<steppable_result<U,T>>& step() = 0;
public:
    virtual ~steppable()= default;
    void step(uint64_t steps, FeedbackCollector collector) {
        uint64_t counter = 0;
        for (const auto& result : step()) {
            if (result.has_feedback())
                collector.push_back(result.feedback());
            if (result.has_result())
                break;
            ++counter;
            if (counter >= steps)
                break;
        }
    };
};


#endif //ELOQUENTLOGIC_STEPPABLE_HPP
