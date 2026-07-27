//
// Created by vscode on 7/19/26.
//

#ifndef ELOQUENTLOGIC_FEEDBACK_BASE_HPP
#define ELOQUENTLOGIC_FEEDBACK_BASE_HPP
#include <type_traits>
#include <string>
#include <nlohmann/json.hpp>
#define FEEDBACK_TYPE(T) std::string get_type() const final {return #T;}
namespace eloquent::logic {
    class feedback_base {
        [[nodiscard]] virtual std::string get_type() const = 0;
        [[nodiscard]] virtual bool is_result() const = 0;
        virtual void to_json(nlohmann::json& j) const;
        virtual void from_json(const nlohmann::json& j);
    };
    template<typename T>
    concept Feedback = std::is_base_of_v<feedback_base, T>;
}
#endif //ELOQUENTLOGIC_FEEDBACK_BASE_HPP
