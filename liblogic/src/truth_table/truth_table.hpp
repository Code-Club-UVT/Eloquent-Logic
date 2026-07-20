//
// Created by vscode on 7/19/26.
//

#ifndef ELOQUENTLOGIC_TRUTH_TABLE_HPP
#define ELOQUENTLOGIC_TRUTH_TABLE_HPP

#include <steppable.hpp>
#include <feedback_base.hpp>
#include <string>

#include "interpretation.h"
#include "lexeme.hpp"
#include <syntax_tree.hpp>
namespace eloquent::logic {
    class truth_table_feedback : public feedback_base {
        interpretation m_interpretation;
        public:
[[nodiscard]] explicit truth_table_feedback(interpretation m_interpretation)
            : m_interpretation(std::move(m_interpretation)) {}
    };

    class truth_table : public steppable<truth_table_feedback>{
        std::vector<lexeme> m_lexemes;
        explicit truth_table(std::vector<lexeme> lexemes): m_lexemes(std::move(lexemes)) {}
    protected:
        std::generator<steppable_result<truth_table_feedback>> step_impl() final;
    public:
        static truth_table make_from_tree(const syntax_tree& tree);

    };
};
#endif //ELOQUENTLOGIC_TRUTH_TABLE_HPP
