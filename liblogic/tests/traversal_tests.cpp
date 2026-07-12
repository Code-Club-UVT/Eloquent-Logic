#include <gtest/gtest.h>
#include <liblogic/private/tree_walker.h>
#include <liblogic/private/tree_strategies/depth_first_traversal.h>
#include <liblogic/private/tree_strategies/breath_first_traversal.h>
#include <liblogic/private/cursor.h>


namespace eloquent::logic {
    TEST(traversal_tests, dfs_parses_correctly) {
        CursorPrivate c;

        c.make_root();
        c.set_node(NodeBuilder::makeNewAndNode());
        c.spawn_new_child_node();
        // c.spawn_new_child_node();
        c.set_node(NodeBuilder::makeNewOrNode());
        c.spawn_new_child_node();
        // c.spawn_new_child_node();
        c.set_node(NodeBuilder::makeNewAtomicNode("P"));
        c.up();
        c.spawn_new_child_node();
        //c.spawn_new_child_node();
        c.set_node(NodeBuilder::makeNewAtomicNode("Q"));
        c.up();
        c.up();
        c.spawn_new_child_node();
        //c.spawn_new_child_node();
        c.set_node(NodeBuilder::makeNewAtomicNode("R"));


        std::vector<std::string> results;
        auto node_callback = [&results] (NodePtr node, size_t index) {
            results.emplace_back(node->text);
            return true;
        };

        luke_tree_walker dfs_walker(make_depth_first_traversal(c.root,node_callback));
        dfs_walker.walk();
        ASSERT_EQ(results, std::vector<std::string>({R"(\\wedge)", R"(\\vee)","P", "Q","R"}));
    }
    TEST(traversal_tests, bfs_parses_correctly) {
        CursorPrivate c;

        c.make_root();
        c.set_node(NodeBuilder::makeNewAndNode());

        c.spawn_new_child_node();
        c.set_node(NodeBuilder::makeNewOrNode());
        c.spawn_new_child_node();
        //c.spawn_new_child_node();
        c.set_node(NodeBuilder::makeNewAtomicNode("P"));
        c.up();
        c.spawn_new_child_node();
        //c.spawn_new_child_node();
        c.set_node(NodeBuilder::makeNewAtomicNode("Q"));
        c.up();
        c.up();
        c.spawn_new_child_node();
        //c.spawn_new_child_node();
        c.set_node(NodeBuilder::makeNewAtomicNode("R"));


        std::vector<std::string> results;
        auto node_callback = [&results] (NodePtr node, size_t index) {
            results.emplace_back(node->text);
            return true;
        };

        luke_tree_walker bfs_walker(make_breath_first_traversal(c.root,node_callback));
        bfs_walker.walk();
        ASSERT_EQ(results, std::vector<std::string>({R"(\\wedge)", R"(\\vee)","R","P", "Q"}));
    }
}
