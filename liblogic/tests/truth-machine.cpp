#include <gtest/gtest.h>
#include <../parser/util/cursor.h>
#include </IdeaProjects/liblogic/liblogic/src/tree_utils/tree_builder.h>
#include <../src/parser/util/execution_stack.h>
#include <../src/operation_private.h>
namespace eloquent::logic {
    TEST(TruthMachineTest, BasicStackTest) {
        CursorPrivate cursor;

        cursor.make_root();
        cursor.set_node(NodeBuilder::makeNewAndNode());
        cursor.spawn_new_child_node();
        cursor.set_node(NodeBuilder::makeNewAtomicNode("P"));
        cursor.up();
        cursor.spawn_new_child_node();
        cursor.set_node(NodeBuilder::makeNewAtomicNode("Q"));
        cursor.up();

        auto interpretare = std::make_shared<interpretation>();
        interpretare->emplace("P",true);
        interpretare->emplace("Q",true);
        auto result = cursor.evaluate(interpretare);
        EXPECT_EQ(result.value(),true);

        (*interpretare)["P"] = false;
        cursor.estack->reset();
        auto result2 = cursor.evaluate(interpretare);
        EXPECT_EQ(result2.value(),false);

    }
    TEST(TruthMachineTest, BasicStackTest2) {
        CursorPrivate cursor;

        cursor.make_root();
        cursor.set_node(NodeBuilder::makeNewOrNode());
        cursor.spawn_new_child_node();
        cursor.set_node(NodeBuilder::makeNewAtomicNode("P"));
        cursor.up();
        cursor.spawn_new_child_node();
        cursor.set_node(NodeBuilder::makeNewAtomicNode("Q"));
        cursor.up();

        auto interpretare = std::make_shared<interpretation>();
        interpretare->emplace("P",true);
        interpretare->emplace("Q",true);
        auto result = cursor.evaluate(interpretare);
        EXPECT_EQ(result.value(),true);

        (*interpretare)["P"] = false;
        (*interpretare)["Q"] = false;
        cursor.estack->reset();
        auto result2 = cursor.evaluate(interpretare);
        EXPECT_EQ(result2.value(),false);
    }

}