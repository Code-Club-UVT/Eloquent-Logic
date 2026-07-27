//
// Created by vscode on 7/19/26.
//

#include "feedback_base.hpp"

namespace eloquent::logic {
    void feedback_base::to_json(nlohmann::json &j) const {
        j.emplace("type", this->get_type());
        j.emplace("is_result", this->is_result());
    }

    void feedback_base::from_json(const nlohmann::json &j) {
        if (j["type"].get<std::string>() != this->get_type()) {
            throw std::logic_error("feedback_base::from_json: type mismatch");
        }
        if (j["is_result"].get<bool>() != this->is_result()) {
            throw std::logic_error("feedback_base::from_json: result mismatch");
        }
    }
}
