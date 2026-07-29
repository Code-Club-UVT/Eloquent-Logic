//
// Created by vscode on 7/29/26.
//

#ifndef ELOQUENTLOGIC_FEEDBACK_STREAM_HPP
#define ELOQUENTLOGIC_FEEDBACK_STREAM_HPP
#include <coroutine>
#include <exception>
#include <vector>
#include <variant>
#include <optional>
#include <concepts>

#include "feedback_base.hpp"

namespace eloquent::logic
{
    template<typename T>
    concept feedback_type = std::derived_from<T, feedback_base>;
    // Wrapper for either an intermediate feedback item or the final result
    template <feedback_type F, typename ResultT>
    class feedback_stream {
    public:
        using Event = std::variant<F, ResultT>;

        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;

        struct promise_type {
            std::optional<Event> current_value;
            std::exception_ptr exception;

            feedback_stream get_return_object() {
                return feedback_stream{handle_type::from_promise(*this)};
            }

            std::suspend_always initial_suspend() noexcept { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }

            // Triggered by co_yield
            std::suspend_always yield_value(F value) noexcept {
                current_value = Event{std::move(value)};
                return {};
            }

            // Triggered by co_return
            void return_value(ResultT value) noexcept {
                current_value = Event{std::move(value)};
            }

            void unhandled_exception() {
                exception = std::current_exception();
            }
        };

    private:
        handle_type handle_;

    public:
        explicit feedback_stream(handle_type h) : handle_(h) {}
        ~feedback_stream() {
            if (handle_) handle_.destroy();
        }

        feedback_stream(const feedback_stream&) = delete;
        feedback_stream& operator=(const feedback_stream&) = delete;

        feedback_stream(feedback_stream&& other) noexcept : handle_(other.handle_) {
            other.handle_ = nullptr;
        }

        // Advances execution until the next co_yield or co_return
        bool next() {
            if (!handle_ || handle_.done()) return false;
            handle_.resume();
            if (handle_.promise().exception) {
                std::rethrow_exception(handle_.promise().exception);
            }
            return !handle_.done();
        }

        // Retrieve current yield/return payload
        const Event& current() const {
            return *handle_.promise().current_value;
        }

        [[nodiscard]] bool is_done() const {
            return handle_.done();
        }
    };
} // eloquent

#endif //ELOQUENTLOGIC_FEEDBACK_STREAM_HPP
