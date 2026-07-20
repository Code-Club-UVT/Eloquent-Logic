//
// Created by vscode on 7/19/26.
//

#include "truth_table.hpp"
/// Aici se implementeaza algoritmul cu cele 2 stive

#include <stack>
namespace eloquent::logic {
    std::generator<steppable_result<truth_table_feedback>>  truth_table::step_impl() {

    }

    truth_table truth_table::make_from_tree(const syntax_tree &tree) {
        std::vector<lexeme> lexemes;
        for (const auto& node: tree.post_order()){
            if (node->isBlank()) {
                throw std::invalid_argument("blank node");
            }
            if (node->isAtom())
                lexemes.push_back(lexeme::build_from_node(node));
            else if (node->type == NodeType::NotOp) {

            }
        }
        return truth_table(lexemes);
    }
};