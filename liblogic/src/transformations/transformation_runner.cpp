//
// Created by Bogdan Petru on 10/08/2026.
//

#include "transformation_runner.hpp"

#include <utility>

#include "cnf_recogniser.hpp"
#include "dnf_recogniser.hpp"

namespace eloquent
{
    namespace logic
    {


        void transformation_runner::to_nnf(std::shared_ptr<syntax_tree>& tree,
            const std::shared_ptr<node_transformation_listener_t>& listener)
        {
            for (auto& transf: transformations)
            {
                bool is_done = true;
                do
                {
                    is_done = true;
                    NodeObsPtr tgt = nullptr;
                    tree->walk([&](auto node)
                    {
                        if (transf->match(node, listener))
                        {
                            tgt = node;
                            is_done = false;
                        }
                    });
                    if (tgt)
                        transf->replace(tgt, listener);
                }
                while (!is_done);
            }
        }

        void transformation_runner::to_dnf(std::shared_ptr<syntax_tree>& tree,
            const std::shared_ptr<node_transformation_listener_t>& listener)
        {
            to_nnf(tree, listener);
            dnf_recogniser recogniser;
            std::array<std::unique_ptr<tree_operation_base>,2> ops = {
                std::make_unique<and_distribution>(),
                std::make_unique<absorption_transformation>()
            };
            while (!recogniser.match(tree->rootRef(), listener))
            {
                for (auto& t: ops)
                {
                    NodeObsPtr tgt = nullptr;
                    tree->walk([&](auto node)
                    {
                        if (t->match(node, listener))
                        {
                            tgt = node;
                        }
                    });
                    if (tgt)
                        t->replace(tgt, listener);
                }
            }
        }

        void transformation_runner::to_cnf(std::shared_ptr<syntax_tree>& tree,
            const std::shared_ptr<node_transformation_listener_t>& listener)
        {
            to_nnf(tree, listener);
            cnf_recogniser recogniser;
            std::array<std::unique_ptr<tree_operation_base>,2> ops = {
                std::make_unique<or_distribution>(),
                std::make_unique<absorption_transformation>()
            };
            while (!recogniser.match(tree->rootRef(), listener))
            {
                for (auto& t: ops)
                {
                    NodeObsPtr tgt = nullptr;
                    tree->walk([&](auto node)
                    {
                        if (t->match(node, listener))
                        {
                            tgt = node;
                        }
                    });
                    if (tgt)
                        t->replace(tgt, listener);
                }
            }
        }
    } // logic
} // eloquent