//
// Created by vscode on 7/19/26.
//

#ifndef ELOQUENTLOGIC_TRUTH_TABLE_HPP
#define ELOQUENTLOGIC_TRUTH_TABLE_HPP

#include <string>
#include <system/uuid.h>


#include <lexeme.hpp>
#include <unordered_map>
#include <syntax_tree.hpp>

#include "truth_table_listener_t.hpp"

namespace eloquent::logic {

    using interpretation = std::unordered_map<std::string, bool>;
    using table_row = std::unordered_map<CppCommon::UUID, bool>;
    class truth_table{
        interpretation m_interpretation;
        std::shared_ptr<syntax_tree> m_tree;
        table_row value_cache;
        explicit truth_table(std::shared_ptr<syntax_tree> tree): m_tree(std::move(tree)) {}
        bool get_value(NodeObsPtr nid);
        void emplace_or_update(CppCommon::UUID, bool value);
        void evaluate_impl(const interpretation& i, const std::shared_ptr<truth_table_listener_t>& listener);

    public:

        std::vector<table_row> evaluate_all (const std::shared_ptr<truth_table_listener_t>& listener);
        table_row evaluate(const interpretation& i, const std::shared_ptr<truth_table_listener_t>& listener);
        static truth_table make_from_tree(const std::shared_ptr<syntax_tree>& tree);
    };
};
#endif //ELOQUENTLOGIC_TRUTH_TABLE_HPP
