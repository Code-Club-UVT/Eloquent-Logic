//
// Created by Bogdan Petru on 13/08/2026.
//

#include "mapping_generator.hpp"
#include <cnf_recogniser.hpp>
namespace eloquent::logic
{
    namespace
    {
        // A literal is a bare atom (positive) or its negation (NotOp wrapping
        // one Atom child, negative, DIMACS-style) — to_cnf's NNF pass
        // guarantees negation never appears any deeper than this.
        Literal literal_for(NodeObsPtr node, const std::map<std::string, Literal>& mapping)
        {
            if (node->getType() == NodeType::NotOp)
            {
                return -mapping.at(node->childAt(0)->getLexeme().token());
            }
            return mapping.at(node->getLexeme().token());
        }
    }

    conversion_result_t mapping_generator::gen_map(const std::shared_ptr<syntax_tree>& tree, const std::shared_ptr<mapper_listener>& listener)
    {
        conversion_result_t result;
        cnf_recogniser recogniser;
        auto nlistener = std::make_shared<node_transformation_listener_t>();
        if (!recogniser.match(tree->rootRef(), nlistener))
        {
            throw std::runtime_error("Cannot map non-cnf form formula");
        }

        Literal start = 1;
        tree->rootRef()->walk([&](auto node)
        {
            lexeme l = node->getLexeme();
            if (l.type() == lexeme_type::Atom && !result.mapping.contains(l.token()))
            {
                result.mapping.try_emplace(l.token(), start);
                ++start;
            }
        });
        listener->did_map_literals(result.mapping);

        auto clause_from = [&](NodeObsPtr clause_node)
        {
            if (clause_node->getType() != NodeType::OrOp)
            {
                // A clause of exactly one literal has no OrOp wrapper.
                return Clause{literal_for(clause_node, result.mapping)};
            }
            Clause c;
            clause_node->traverse_children([&](NodeObsPtr literal_node)
            {
                c.emplace(literal_for(literal_node, result.mapping));
            });
            return c;
        };

        auto* root = tree->rootRef();
        if (root->getType() == NodeType::AndOp)
        {
            root->traverse_children([&](NodeObsPtr clause_node)
            {
                result.clauses.emplace(clause_from(clause_node));
            });
        }
        else
        {
            // A formula of exactly one clause has no AndOp wrapper.
            result.clauses.emplace(clause_from(root));
        }
        listener->did_map_clauses(result.clauses);

        return result;
    }
}
