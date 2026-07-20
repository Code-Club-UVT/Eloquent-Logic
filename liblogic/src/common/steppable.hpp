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

/**
 * Base class for all operations that can be performed step by step.
 * @tparam U Feedback superclass of all types of feedback that can be generated from the object
 * @tparam T Type of the result the completed operation returns. We recommend it being a std::expected<T,E> for easier serialisation and deserialisation
 */
template<Feedback U, ResultType T=std::monostate> //CONSIDER: state type (?)
class steppable {
protected:

    virtual std::generator<steppable_result<U,T>> step_impl() = 0;
public:
    virtual ~steppable()= default;

    steppable_result<U,T> step(uint64_t steps, FeedbackCollector collector) {
        uint64_t counter = 0;
        for (const auto& result : step()) {
            if (result.has_feedback())
                collector.push_back(result.feedback());
            if (result.has_result())
                return result;
            ++counter;
            if (counter >= steps)
                return result;
        }
        throw std::runtime_error("");
    };
};


#endif //ELOQUENTLOGIC_STEPPABLE_HPP
