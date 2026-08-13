//
// Created by Bogdan Petru on 13/08/2026.
//

#include "mapping_generator.hpp"
#include <cnf_recogniser.hpp>
namespace eloquent::logic
{
    conversion_result_t mapping_generator::gen_map(const std::shared_ptr<syntax_tree>& tree, const std::shared_ptr<mapper_listener>& listener)
    {
        conversion_result_t result;
        cnf_recogniser recogniser;
        std::shared_ptr<node_transformation_listener_t> nlistener;
        if (!recogniser.match(tree->rootRef(), nlistener))
        {
            throw std::runtime_error("Cannot map non-cnf form formula");
        }
        Literal start = 1;
        tree->rootRef()->walk([&](auto node)
        {
            lexeme l = node->getLexeme();
            if (l.type() == lexeme_type::Atom &&
                !result.mapping.contains(l.token()))
            {
                result.mapping.try_emplace(l.token(), start);
                ++start;
            }
        });
        listener->did_map_literals(result.mapping);
        tree->rootRef()->traverse_children([&](auto node)
        {
            lexeme l = node->getLexeme();
            if (l.type() == lexeme_type::Atom)
            {
                result.clauses.emplace(Clause({result.mapping.at(l.token())}));
            }
            else
            {
                Clause c;
                node->traverse_children([&](auto n)
                {
                    c.emplace(result.mapping.at(n->getLexeme().token()));
                });
                result.clauses.emplace(c);
            }
        });
        listener->did_map_clauses(result.clauses);
        return result;
    }
}