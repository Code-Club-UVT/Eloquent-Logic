//
// Created by vscode on 7/19/26.
//

#ifndef ELOQUENTLOGIC_TRUTH_TABLE_HPP
#define ELOQUENTLOGIC_TRUTH_TABLE_HPP

#include <steppable.hpp>
#include <feedback_base.hpp>

class truth_table_feedback : public feedback_base {

};
class truth_table_header_feedback : public truth_table_feedback {

};
class truth_table_operation_feedback : public truth_table_feedback {};

class truth_table : public steppable<truth_table_feedback>{

protected:
    std::generator<steppable_result<truth_table_feedback>> & step() final {

    }
};


#endif //ELOQUENTLOGIC_TRUTH_TABLE_HPP
