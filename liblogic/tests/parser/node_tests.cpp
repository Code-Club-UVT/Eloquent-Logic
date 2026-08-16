//
// Tests for Node::condense() ("n-isation": flattening a nested chain of the
// same AndOp/OrOp operator into a single n-ary node), independent of any
// listener.
//
#include <gtest/gtest.h>
#include <string>
#include <vector>

#include <node.h>
#include <dictionary.h>
#include <lexeme.hpp>

namespace eloquent::logic::testing
{
namespace
{
    NodePtr atom(const std::string& name)
    {
        return Node::make_node(lexeme::make(lexeme_type::Atom, name, 0, 0));
    }

    template <typename... Children>
    NodePtr op(lexeme_type type, const std::string& sym, Children&&... children)
    {
        NodePtr n = Node::make_node(lexeme::make(type, sym, 0, 0));
        (n->adopt(std::forward<Children>(children)), ...);
        return n;
    }

    std::vector<std::string> child_tokens(const NodeObsPtr& node)
    {
        std::vector<std::string> tokens;
        node->traverse_children([&](const NodeObsPtr& child) { tokens.push_back(child->getLexeme().token()); });
        return tokens;
    }
} // namespace

TEST(NodeCondenseTest, IsANoOpForNonAndOrOrNodes)
{
    NodePtr tree = op(lexeme_type::NotOp, symbols::SYMB_NOT, atom("P"));
    NodeObsPtr root = tree.get();

    int merge_calls = 0;
    root->condense([&](NodeObsPtr, NodeObsPtr) { ++merge_calls; });

    EXPECT_EQ(root->getType(), NodeType::NotOp);
    ASSERT_EQ(root->num_children(), 1u);
    EXPECT_EQ(root->childAt(0)->getLexeme().token(), "P");
    EXPECT_EQ(merge_calls, 0);
}

TEST(NodeCondenseTest, IsANoOpWhenNoChildSharesItsType)
{
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"), atom("Q"));
    NodeObsPtr root = tree.get();

    int merge_calls = 0;
    root->condense([&](NodeObsPtr, NodeObsPtr) { ++merge_calls; });

    EXPECT_EQ(root->getType(), NodeType::AndOp);
    EXPECT_EQ(child_tokens(root), (std::vector<std::string>{"P", "Q"}));
    EXPECT_EQ(merge_calls, 0);
}

TEST(NodeCondenseTest, FlattensASingleNestedChildOfTheSameType)
{
    // AndOp(AndOp(A,B), C) -> AndOp(C, A, B)
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND,
        op(lexeme_type::AndOp, symbols::SYMB_AND, atom("A"), atom("B")), atom("C"));
    NodeObsPtr root = tree.get();

    std::vector<NodeObsPtr> merged;
    root->condense([&](NodeObsPtr, NodeObsPtr n) { merged.push_back(n); });

    EXPECT_EQ(root->getType(), NodeType::AndOp);
    EXPECT_EQ(child_tokens(root), (std::vector<std::string>{"C", "A", "B"}));
    ASSERT_EQ(merged.size(), 1u);
}

TEST(NodeCondenseTest, ReportsTheImmediateParentOfEachMerge)
{
    // The merge here happens directly on `root`, so parent should be root
    // itself (not e.g. null or some other node) — pinned down explicitly so
    // the deeper-nesting test below has a clear baseline to contrast with.
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND,
        op(lexeme_type::AndOp, symbols::SYMB_AND, atom("A"), atom("B")), atom("C"));
    NodeObsPtr root = tree.get();

    NodeObsPtr seen_parent = nullptr;
    root->condense([&](NodeObsPtr parent, NodeObsPtr) { seen_parent = parent; });

    EXPECT_EQ(seen_parent, root);
}

TEST(NodeCondenseTest, MergedChildIsStillIntactAtCallbackTime)
{
    // The callback fires before the merged child's own children are moved
    // out, so it should still observe the pre-merge AndOp(A,B) shape.
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND,
        op(lexeme_type::AndOp, symbols::SYMB_AND, atom("A"), atom("B")), atom("C"));
    NodeObsPtr root = tree.get();

    bool saw_intact_merge = false;
    root->condense([&](NodeObsPtr, NodeObsPtr n) {
        saw_intact_merge = n->getType() == NodeType::AndOp && n->num_children() == 2 &&
            n->childAt(0)->getLexeme().token() == "A" && n->childAt(1)->getLexeme().token() == "B";
    });

    EXPECT_TRUE(saw_intact_merge);
}

TEST(NodeCondenseTest, FlattensMultipleNestedChildrenAcrossPasses)
{
    // AndOp(AndOp(A,B), C, AndOp(D,E)) -> AndOp(C, A, B, D, E), with one
    // didCondenseChild-style callback per merged nested node (2 total).
    // This tree is also large enough to exercise the vector-reallocation
    // hazard: adopt() appends to the very vector condense() is iterating,
    // so a naive implementation can invalidate its own iterator mid-merge.
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND,
        op(lexeme_type::AndOp, symbols::SYMB_AND, atom("A"), atom("B")), atom("C"),
        op(lexeme_type::AndOp, symbols::SYMB_AND, atom("D"), atom("E")));
    NodeObsPtr root = tree.get();

    int merge_calls = 0;
    root->condense([&](NodeObsPtr, NodeObsPtr) { ++merge_calls; });

    EXPECT_EQ(root->getType(), NodeType::AndOp);
    EXPECT_EQ(child_tokens(root), (std::vector<std::string>{"C", "A", "B", "D", "E"}));
    EXPECT_EQ(merge_calls, 2);
}

TEST(NodeCondenseTest, FlattensNestedOrOpTheSameWayAsAndOp)
{
    // OrOp(OrOp(A,B), C) -> OrOp(C, A, B)
    NodePtr tree = op(lexeme_type::OrOp, symbols::SYMB_OR,
        op(lexeme_type::OrOp, symbols::SYMB_OR, atom("A"), atom("B")), atom("C"));
    NodeObsPtr root = tree.get();

    int merge_calls = 0;
    root->condense([&](NodeObsPtr, NodeObsPtr) { ++merge_calls; });

    EXPECT_EQ(root->getType(), NodeType::OrOp);
    EXPECT_EQ(child_tokens(root), (std::vector<std::string>{"C", "A", "B"}));
    EXPECT_EQ(merge_calls, 1);
}

TEST(NodeCondenseTest, FlattensANestedChainBuriedBelowADifferentlyTypedRoot)
{
    // OrOp(AndOp(AndOp(A,B), C), D) -> OrOp(AndOp(C,A,B), D): the OrOp root
    // itself has no same-type child (so it's untouched), but condense()
    // called on it must still recurse down and flatten the AndOp-of-AndOp
    // buried one level below — this is the whole point of the fix: a single
    // call anywhere in the tree flattens same-type chains everywhere in it,
    // not just among the called-on node's own direct children.
    NodePtr tree = op(lexeme_type::OrOp, symbols::SYMB_OR,
        op(lexeme_type::AndOp, symbols::SYMB_AND,
            op(lexeme_type::AndOp, symbols::SYMB_AND, atom("A"), atom("B")), atom("C")),
        atom("D"));
    NodeObsPtr root = tree.get();

    int merge_calls = 0;
    root->condense([&](NodeObsPtr, NodeObsPtr) { ++merge_calls; });

    EXPECT_EQ(root->getType(), NodeType::OrOp);
    ASSERT_EQ(root->num_children(), 2u);
    EXPECT_EQ(merge_calls, 1);

    NodeObsPtr and_child = root->childAt(0);
    EXPECT_EQ(and_child->getType(), NodeType::AndOp);
    EXPECT_EQ(child_tokens(and_child), (std::vector<std::string>{"C", "A", "B"}));
    EXPECT_EQ(root->childAt(1)->getLexeme().token(), "D");
}

TEST(NodeCondenseTest, ReportsTheTrueParentForAMergeBelowTheCalledOnNode)
{
    // Mirror of the tree above, but asserting on *which node* the merge
    // callback reports as `parent`: it must be the inner AndOp the merge
    // actually happened on, not the outer OrOp condense() was called on.
    NodePtr tree = op(lexeme_type::OrOp, symbols::SYMB_OR,
        op(lexeme_type::AndOp, symbols::SYMB_AND,
            op(lexeme_type::AndOp, symbols::SYMB_AND, atom("A"), atom("B")), atom("C")),
        atom("D"));
    NodeObsPtr root = tree.get();
    NodeObsPtr expected_parent = root->childAt(0);

    NodeObsPtr seen_parent = nullptr;
    root->condense([&](NodeObsPtr parent, NodeObsPtr) { seen_parent = parent; });

    EXPECT_EQ(seen_parent, expected_parent);
    EXPECT_NE(seen_parent, root);
}

TEST(NodeCondenseTest, WorksWithoutACallback)
{
    // The callback parameter is optional; condense() must still work when
    // called with none (the pre-existing call convention).
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND,
        op(lexeme_type::AndOp, symbols::SYMB_AND, atom("A"), atom("B")), atom("C"));
    NodeObsPtr root = tree.get();

    EXPECT_NO_THROW(root->condense());

    EXPECT_EQ(child_tokens(root), (std::vector<std::string>{"C", "A", "B"}));
}

} // namespace eloquent::logic::testing
