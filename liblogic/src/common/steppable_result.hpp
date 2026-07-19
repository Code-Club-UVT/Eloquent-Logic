//
// Created by vscode on 7/19/26.
//

#ifndef ELOQUENTLOGIC_STEPPABLE_RESULT_HPP
#define ELOQUENTLOGIC_STEPPABLE_RESULT_HPP
#include <memory>

#include "feedback_base.hpp"
#include <utility>
#include <variant>

template <typename T>
concept ResultType = !std::is_base_of_v<feedback_base, T> && std::is_move_constructible_v<T>;


template <Feedback U, ResultType  T = std::monostate>
class steppable_result {

    T m_result;
    std::shared_ptr<U> m_feedback;
    bool m_has_result;
    bool m_has_feedback;
    public:
    steppable_result(T result, U feedback):m_result(std::move(result)), m_feedback(std::move(feedback)), m_has_result(true), m_has_feedback(true) {}
    steppable_result(U feedback):m_result(std::move(feedback)), m_has_result(false), m_has_feedback(true) {}
    steppable_result(T result):m_result(std::move(result)), m_has_result(true), m_has_feedback(false) {}

    [[nodiscard]] bool has_result() const { return m_has_result; }
    [[nodiscard]] bool has_feedback() const { return m_has_feedback; }
    [[nodiscard]] auto feedback() const { return m_feedback; }
    [[nodiscard]] auto result() const { return m_result; }

};

#endif //ELOQUENTLOGIC_STEPPABLE_RESULT_HPP
