//
// Created by xcell on 15.12.2024.
//

#include <string>
#include <stack>
#include <memory>

#include <gtest/gtest.h>

#include <../src/transformations.h>
#include <test_wrapper_functions.h>

namespace eloquent::logic {
    TEST(Transformations, wrapper_test) {
        const std::string exp = R"(F \\vee \\top)";
        const auto cleanedtree = tests::build_tree(exp); // the root of the cleaned tree

        if (NeutralElements fixer; fixer.match(cleanedtree.root)) { fixer.replace(cleanedtree.root); }

        // create expected tree to compare with finished tree
        const std::string rightexp = R"(\\top)";
        const auto right = tests::build_tree(rightexp);

        // assertion
        ASSERT_TRUE(cleanedtree.are_trees_equal(right));
    }

    TEST(Transformations, DoubleNegation) {
        // The relaxed syntax is not done right. Without these parentheses, the tree doesn't form properly.
        const std::string exp = R"(P \\wedge (\\neg (\\neg Q)))";
        const auto cleanedtree = tests::build_tree(exp); // the root of the cleaned tree

        // clean double negations with the help of DFS
        DoubleNegation double_negation_fixer;
        std::stack<NodeObsPtr> s;
        s.push(cleanedtree.root);

        while (!s.empty()) {
            auto c = s.top();
            s.pop();

            // processing
            // the fixer has a quirk: it gives children to Q when it shouldn't have any
            NodePtr c_shared = c.lock();
            for (const auto & i : c_shared->children) {
                if (double_negation_fixer.match(i)) {
                    double_negation_fixer.replace(i);
                }
            }

            // push the children to the stack in reverse order to respect DFS
            if (!c_shared->children.empty()) {
                for (int i = c_shared->children.size() - 1; i >= 0; i--) {
                    s.push(c_shared->children[i]);
                }
            }

        }

        // Create expected tree to compare with the finished tree
        const std::string rightexp = R"(P \\wedge Q)";
        const auto right = tests::build_tree(rightexp);

        // assertion
        ASSERT_TRUE(cleanedtree.are_trees_equal(right));
    }

    TEST(Transformations, ImplicationReduction) {
        // THIS TEST IS BROKEN - the \\neg operator doesn't parse properly

        // input expression
        const std::string exp = R"(P \\implies Q)";
        const auto cleanedtree = tests::build_tree(exp); // the root of the cleaned tree

        // reduce implications
        ImplicationReduction implication_reducer;
        std::stack<NodeObsPtr> s;
        s.push(cleanedtree.root);

        while (!s.empty()) {
            auto c = s.top();
            s.pop();

            NodePtr c_shared = c.lock();
            if (implication_reducer.match(c_shared)) {
                implication_reducer.replace(c_shared);
            }

            if (!c_shared->children.empty()) {
                for (int i = c_shared->children.size() - 1; i >= 0; i--) {
                    s.push(c_shared->children[i]);
                }
            }
        }

        // Create expected tree to compare with the finished tree
        const std::string rightexp = R"((\\neg P) \\vee Q)";
        const auto right = tests::build_tree(rightexp);

        ASSERT_TRUE(cleanedtree.are_trees_equal(right));
    }

    TEST(Transformations, EquivalenceReduction) {
        // THIS TEST IS BROKEN - the \\neg operator doesn't parse properly

        // input expression
        const std::string exp = R"(P \\iff Q)";
        auto cleanedtree = tests::build_tree(exp); // the root of the cleaned tree

        // Process
        EquivalenceReduction equivalence_reducer;
        auto node = cleanedtree.root;
        if (equivalence_reducer.match(node)) {
            equivalence_reducer.replace(node);
        }

        // Create expected tree to compare with the finished tree
        const std::string rightexp = R"((P \\wedge Q) \\vee ((\\neg P) \\wedge (\\neg Q)))";
        const auto right = tests::build_tree(rightexp);

        ASSERT_TRUE(cleanedtree.are_trees_equal(right));
    }

    TEST(Transformations, DeMorganDisjunction) {
        // THIS TEST IS BROKEN

        // input expression
        const std::string exp = R"(\\neg (P \\vee Q \\vee R))";
        const auto cleanedtree = tests::build_tree(exp); // the root of the cleaned tree

        // Process
        if (DeMorganDisjunction de_morgan_fixer; de_morgan_fixer.match(cleanedtree.root)) {
            de_morgan_fixer.replace(cleanedtree.root);
        }

        // Create expected tree to compare with the finished tree
        const std::string rightexp = R"((\\neg P)\\wedge(\\neg Q)\\wedge(\\neg R))";
        const auto right = tests::build_tree(rightexp);

        ASSERT_TRUE(cleanedtree.are_trees_equal(right));
    }

    TEST(Transformations, DeMorganConjunction) {
        // THIS TEST IS BROKEN

        const std::string exp = R"(\\neg (P \\wedge Q \\wedge R))";
        const auto cleanedtree = tests::build_tree(exp);

        // Process
        if (DeMorganConjunction de_morgan_fixer; de_morgan_fixer.match(cleanedtree.root)) {
            de_morgan_fixer.replace(cleanedtree.root);
        }

        // Create expected tree to compare with the finished tree
        const std::string rightexp = R"((\\neg P)\\vee(\\neg Q)\\vee(\\neg R))";
        const auto right = tests::build_tree(rightexp);

        ASSERT_TRUE(cleanedtree.are_trees_equal(right));
    }

    TEST(Transformations, N_ization1) {
        const std::string exp = R"(P \\wedge Q \\wedge R)";
        const auto cleanedtree = tests::build_tree(exp); // the root of the cleaned tree

        // Process
        if (N_izer n_izer; n_izer.match(cleanedtree.root)) {
            n_izer.replace(cleanedtree.root);
        }

        // Create expected tree to compare with the finished tree

        // This test cannot be automated by tests::build_tree(),
        // because this transformation affects organization of nodes in the AST,
        // and not the syntactic values of operation nodes.
        CursorPrivate right;
        right.make_root();
        right.set_node(NodeBuilder::makeNewAndNode());
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("Q"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("R"));

        ASSERT_TRUE(cleanedtree.are_trees_equal(right));
    }

    TEST(Transformations, N_ization2) {
        //const std::string exp = R"(P \\vee Q \\vee R)";
        const std::string exp = R"(P_1 \\vee P_2 \\vee P_3 \\vee P_4 \\vee P_5 \\vee P_6 \\vee P_7 \\vee P_8 \\vee P_9)";
        const auto cleanedtree = tests::build_tree(exp); // the root of the cleaned tree

        // Process
        if (N_izer n_izer; n_izer.match(cleanedtree.root)) {
            n_izer.replace(cleanedtree.root);
        }

        // Create expected tree to compare with the finished tree

        // This test cannot be automated by tests::build_tree(),
        // because this transformation affects organization of nodes in the AST,
        // and not the syntactic values of operation nodes.
        CursorPrivate right;
        right.make_root();
        right.set_node(NodeBuilder::makeNewOrNode());
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P_1"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P_2"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P_3"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P_4"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P_5"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P_6"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P_7"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P_8"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P_9"));

        ASSERT_TRUE(cleanedtree.are_trees_equal(right));
    }

    // xcell: I did NOT test inversions extensively! The class functions may still blow up.

    TEST(Transformations, Inversion1) {
        // This tree also doesn't seem to form properly, unless the extra parentheses are added.
        // Something wrong with the "\neg" operator?
        const std::string exp = R"((\\neg \\bot))";
        const auto cleanedtree = tests::build_tree(exp); // the root of the cleaned tree

        // Process
        if (Inverter inverter; inverter.match(cleanedtree.root)) {
            inverter.replace(cleanedtree.root);
        }

        // Create expected tree to compare with the finished tree
        const std::string rightexp = R"(\\top)";
        const auto right = tests::build_tree(rightexp);

        ASSERT_TRUE(cleanedtree.are_trees_equal(right));
    }

    TEST(Transformations, Inversion2) {
        const std::string exp = R"((\\neg \\top))";
        const auto cleanedtree = tests::build_tree(exp); // the root of the cleaned tree

        // Process
        if (Inverter inverter; inverter.match(cleanedtree.root)) {
            inverter.replace(cleanedtree.root);
        }

        // Create expected tree to compare with the finished tree
        const std::string rightexp = R"(\\bot)";
        const auto right = tests::build_tree(rightexp);

        ASSERT_TRUE(cleanedtree.are_trees_equal(right));
    }

    TEST(Transformations, NeutralElements_Binary) {
        // Creating a battery of expressions to test.
        const std::vector<std::string> battery = {
            R"(F \\vee \\top)",
            R"(F \\vee \\bot)",
            R"(F \\wedge \\top)",
            R"(F \\wedge \\bot)"
        };

        // Creating the array of trees of the corresponding expressions.
        const auto battery_trees = tests::build_tree_array(battery);

        // Actually fixing each and every expression.
        NeutralElements fixer;

        for (const auto& exp : battery_trees) {
            if (fixer.match(exp.root)) fixer.replace(exp.root);
        }

        // Creating a battery of answers for each expression and its corresponding tree.
        const std::vector<std::string> answers = {
            R"(\\top)",
            R"(F)",
            R"(F)",
            R"(\\bot)"
        };

        const auto answers_trees = tests::build_tree_array(answers);

        // Assertion.
        ASSERT_TRUE(tests::are_tree_arrays_equal(battery_trees, answers_trees));
    }

    TEST(Transformations, NeutralElements_Ternary) {
        // Creating a battery of expressions to test.
        const std::vector<std::string> battery = {
            R"(F \\vee G \\vee \\top)",
            R"(F \\vee G \\vee \\bot)",
            R"(F \\wedge G \\wedge \\top)",
            R"(F \\wedge G \\wedge \\bot)"
        };

        // Creating the array of trees of the corresponding expressions.
        const auto battery_trees = tests::build_tree_array(battery, true);

        // Actually fixing each and every expression.
        NeutralElements fixer;

        for (const auto& exp : battery_trees) {
            if (fixer.match(exp.root)) fixer.replace(exp.root);
        }

        // Creating a battery of answers for each expression and its corresponding tree.
        const std::vector<std::string> answers = {
            R"(\\top)",
            R"(F \\vee G)",
            R"(F \\wedge G)",
            R"(\\bot)"
        };

        const auto answers_trees = tests::build_tree_array(answers);

        // Assertion.
        ASSERT_TRUE(tests::are_tree_arrays_equal(battery_trees, answers_trees));
    }

    TEST(Transformations, NeutralElements_Quaternary) {
        // Creating a battery of expressions to test.
        const std::vector<std::string> battery = {
            R"(F \\vee G \\vee H \\vee \\top)",
            R"(F \\vee G \\vee H \\vee \\bot)",
            R"(F \\wedge G \\wedge H \\wedge \\top)",
            R"(F \\wedge G \\wedge H \\wedge \\bot)"
        };

        // Creating the array of trees of the corresponding expressions.
        const auto battery_trees = tests::build_tree_array(battery, true);

        // Actually fixing each and every expression.
        NeutralElements fixer;

        for (const auto& exp : battery_trees) {
            if (fixer.match(exp.root)) fixer.replace(exp.root);
        }

        // Creating a battery of answers for each expression and its corresponding tree.
        const std::vector<std::string> answers = {
            R"(\\top)",
            R"(F \\vee G \\vee H)",
            R"(F \\wedge G \\wedge H)",
            R"(\\bot)"
        };

        const auto answers_trees = tests::build_tree_array(answers, true);

        // Assertion.
        ASSERT_TRUE(tests::are_tree_arrays_equal(battery_trees, answers_trees));
    }

    TEST(Transformations, NeutralElements_Mixed) {
        // --- BROKEN TEST ---
        // It seems like the parser ignores the authority of the parentheses.
        // Will raise an issue on Git.

        // I have 2 more batteries, but I don't think I'm going to implement them just yet,
        // since they look very similar to this one.

        // Creating a battery of expressions to test.
        const std::vector<std::string> battery = {
            R"( (F \\wedge G) \\vee (\\top))",
            R"( (F \\wedge G) \\vee (\\bot))",
            R"( (F \\vee G) \\wedge (\\top))",
            R"( (F \\vee G) \\wedge (\\bot))"
        };

        // Creating the array of trees of the corresponding expressions.
        const auto battery_trees = tests::build_tree_array(battery);

        // Actually fixing each and every expression.
        NeutralElements fixer;

        for (const auto& exp : battery_trees) {
            if (fixer.match(exp.root)) fixer.replace(exp.root);
        }

        // Creating a battery of answers for each expression and its corresponding tree.
        const std::vector<std::string> answers = {
            R"(\\top)",
            R"(F \\wedge G)",
            R"(F \\vee G)",
            R"(\\bot)"
        };

        const auto answers_trees = tests::build_tree_array(answers);

        // Assertion.
        ASSERT_TRUE(tests::are_tree_arrays_equal(battery_trees, answers_trees));
    }
}