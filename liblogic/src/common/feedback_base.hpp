//
// Created by vscode on 7/19/26.
//

#ifndef ELOQUENTLOGIC_FEEDBACK_BASE_HPP
#define ELOQUENTLOGIC_FEEDBACK_BASE_HPP
#include <type_traits>

class feedback_base {

};
template<typename T>
concept Feedback = std::is_base_of_v<feedback_base, T>;

#endif //ELOQUENTLOGIC_FEEDBACK_BASE_HPP
