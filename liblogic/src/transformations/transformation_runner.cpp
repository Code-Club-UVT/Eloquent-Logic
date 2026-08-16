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
            listener->didStart();
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
            listener->didFinish();
        }

        namespace
        {
            // Same-precedence operator chains never come out of the parser
            // (or out of and_distribution/or_distribution's freshly-built
            // wrapper nodes) as a single flat n-ary AndOp/OrOp — they're
            // strictly binary/nested (see relaxed_parser.cpp). cnf_recogniser
            // and dnf_recogniser only accept a flat top level, so without
            // this, a tree that's already logically in normal form but
            // structurally nested would make the while loops below spin
            // forever: nothing else ever matches an AndOp-of-AndOp (or
            // OrOp-of-OrOp), so the tree would never change and match()
            // would keep returning false on every iteration.
            void flatten(std::shared_ptr<syntax_tree>& tree,
                const std::shared_ptr<node_transformation_listener_t>& listener)
            {
                tree->rootRef()->condense([&](NodeObsPtr parent, NodeObsPtr merged)
                {
                    listener->didCondenseChild(parent, merged);
                });
            }
        }

        void transformation_runner::to_dnf(std::shared_ptr<syntax_tree>& tree,
            const std::shared_ptr<node_transformation_listener_t>& listener)
        {
            listener->didStart();
            to_nnf(tree, listener);
            flatten(tree, listener);
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
                flatten(tree, listener);
            }
            listener->didFinish();
        }

        void transformation_runner::to_cnf(std::shared_ptr<syntax_tree>& tree,
            const std::shared_ptr<node_transformation_listener_t>& listener)
        {
            listener->didStart();
            to_nnf(tree, listener);
            flatten(tree, listener);
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
                flatten(tree, listener);
            }
            listener->didFinish();
        }
    } // logic
} // eloquent