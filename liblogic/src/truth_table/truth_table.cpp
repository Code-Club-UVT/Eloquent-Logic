//
// Created by vscode on 7/19/26.
//

#include "truth_table.hpp"
/// Aici se implementeaza algoritmul cu cele 2 stive

#include <stack>

#include "unknown_variable_error.hpp"
#include "unsupported_operator_error.hpp"

namespace eloquent::logic {
    bool truth_table::get_value(table_row& cache, NodeObsPtr nid)
    {
        using enum lexeme_type;
        switch (nid->getLexeme().type())
        {
        case Atom:
            return this->m_interpretation[nid->getLexeme().token()];
        case Tautology:
            return true;
        case Contradiction:
            return false;
        default:
            return cache[nid->getUUID()];
        }
    }
    bool combine(NodeType nt, bool lhs, bool rhs)
    {
        switch (nt)
        {
            using enum NodeType;
        case AndOp:
            {
                return lhs & rhs;
            }
        case OrOp:
            {
                return lhs | rhs;
            }
        case IffOp:
            {
                return lhs == rhs;
            }
        case ImpliesOp:
            {
                return !lhs || rhs;
            }
        default:
            throw std::runtime_error("Should not have reached here");
        }
    }
    void truth_table::emplace_or_update(table_row& cache, CppCommon::UUID id, bool value)
    {
        if (cache.contains(id))
        {
            cache[id] = value;
        }
        else
            cache.try_emplace(id, value);
    }

    void truth_table::evaluate_impl(const interpretation& i, table_row& cache, const std::shared_ptr<truth_table_listener_t>& listener)
    {
        for (const auto& pair : i)
        {
            if (!this->m_interpretation.contains(pair.first))
            {
                listener->didFindUnknownVariable(pair.first);
                throw unknown_variable_error(pair.first);
            }
            this->m_interpretation[pair.first] = pair.second;
            listener->didSetVariable(pair.first, pair.second);
        }

        this->m_tree->walk([&](auto node)
        {
            if (is_operator(node->getLexeme().type()))
            {
                std::optional<bool> result;
                if (is_nary_operator(node->getLexeme().type()))
                {
                    node->traverse_children([&](NodeObsPtr cnode)
                    {
                        if (result == std::nullopt)
                            result = get_value(cache, cnode);
                        else
                            result = combine(node->getType(), result.value(), get_value(cache, cnode));
                    });
                    emplace_or_update(cache, node->getUUID(), result.value());
                    listener->didComputeSubexpression(node->getUUID(), result.value());
                }
                else
                {
                    node->traverse_children([&](NodeObsPtr cnode)
                    {
                        result = get_value(cache, cnode);
                    });
                    emplace_or_update(cache, node->getUUID(), !result.value());
                    listener->didComputeSubexpression(node->getUUID(), result.value());
                }
            }
        });
    }

    std::vector<table_row> truth_table::evaluate_all(const std::shared_ptr<truth_table_listener_t>& listener)
    {
        std::vector<table_row> trs;
        listener->didStart();
        auto num_vars = this->get_variables().size();
        if (num_vars > std::numeric_limits<std::size_t>::digits)
            throw std::runtime_error("too many variables");
        for (size_t i = 0; i < (static_cast<size_t>(1)<<num_vars); ++i)
        { //TODO: this is horribly inefficient
            interpretation it;
            for (size_t j = 0; j< num_vars; ++j)
            {
                it.try_emplace(get_variables()[i], i & (1<<j));
            }
            table_row tr;
            evaluate_impl(it, tr, listener);
            trs.emplace_back(tr);
        }
        listener->didFinish();
        return trs;
    }

    table_row truth_table::evaluate(const interpretation& i, const std::shared_ptr<truth_table_listener_t>& listener)
    {
        table_row tr;
        listener->didStart();
        evaluate_impl(i,tr, listener);
        listener->didFinish();
        return tr;
    }

    truth_table truth_table::make_from_tree(const std::shared_ptr<syntax_tree> &tree) {
       truth_table t(tree);
        // add atoms
       tree->walk([&t](auto node)
       {
           using enum lexeme_type;
           if (node->getLexeme().type() == Atom && !t.m_interpretation.contains(node->getLexeme().token()))
           {
               t.m_interpretation.try_emplace(node->getLexeme().token(), false);
           }
       });
       return t;
    }

    std::vector<std::string> truth_table::get_variables() const noexcept
    {
        std::vector<std::string> result;
        for (const auto& c: m_interpretation)
        {
            result.emplace_back(c.first);
        }
        return result;
    }
};