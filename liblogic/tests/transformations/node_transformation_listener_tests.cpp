//
// Tests for node_transformation_listener_t and its wiring into every
// transformation's match()/replace() (and the two normal-form recognisers).
//
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <node.h>
#include <dictionary.h>
#include <lexeme.hpp>

#include "node_transformation_listener_t.hpp"
#include "logical_implication.hpp"
#include "equivalence_reduction.h"
#include "implication_reduction.h"
#include "de_morgan.h"
#include "double_negation.h"
#include "inverter.h"
#include "absorption_transformation.hpp"
#include "neutral_elements.h"
#include "distributions.hpp"
#include "cnf_recogniser.hpp"
#include "dnf_recogniser.hpp"

namespace eloquent::logic::testing
{
namespace
{
    NodePtr atom(const std::string& name)
    {
        return Node::make_node(lexeme::make(lexeme_type::Atom, name, 0, 0));
    }

    NodePtr constant(lexeme_type type, const std::string& sym)
    {
        return Node::make_node(lexeme::make(type, sym, 0, 0));
    }

    // Builds an operator node and adopts each of `children` in order, so
    // tests can express exact tree shapes without going through a parser
    // (same-precedence chaining in relaxed_parser is right-associative and
    // never auto-flattens into n-ary nodes, which would make the shapes
    // these transformations pattern-match on hard to pin down reliably).
    template <typename... Children>
    NodePtr op(lexeme_type type, const std::string& sym, Children&&... children)
    {
        NodePtr n = Node::make_node(lexeme::make(type, sym, 0, 0));
        (n->adopt(std::forward<Children>(children)), ...);
        return n;
    }

    // Records every callback node_transformation_listener_t fires, so tests
    // can assert on both which rule fired and which node it fired for.
    class recording_listener : public node_transformation_listener_t
    {
    public:
        bool started = false;
        bool finished = false;

        std::vector<NodeObsPtr> matched_logical_implication, reduced_logical_implication;
        std::vector<std::pair<NodeObsPtr, NodeObsPtr>> condensed_children;
        std::vector<NodeObsPtr> matched_equivalence, reduced_equivalence;
        std::vector<NodeObsPtr> matched_implication, reduced_implication;
        std::vector<NodeObsPtr> matched_de_morgan_conjunction, applied_de_morgan_conjunction;
        std::vector<NodeObsPtr> matched_de_morgan_disjunction, applied_de_morgan_disjunction;
        std::vector<NodeObsPtr> matched_double_negation, eliminated_double_negation;
        std::vector<NodeObsPtr> matched_inverter, inverted_constant;
        std::vector<NodeObsPtr> matched_neutral_element, collapsed_to_contradiction, collapsed_to_tautology,
            dropped_neutral_element;
        std::vector<NodeObsPtr> matched_absorption, applied_absorption;
        std::vector<NodeObsPtr> matched_and_distribution, distributed_and_over_or;
        std::vector<NodeObsPtr> matched_or_distribution, distributed_or_over_and;
        std::vector<std::pair<NodeObsPtr, bool>> checked_cnf, checked_dnf;

        void didStart() override { started = true; }
        void didFinish() override { finished = true; }

        void didMatchLogicalImplication(NodeObsPtr n) override { matched_logical_implication.push_back(n); }
        void didReduceLogicalImplication(NodeObsPtr n) override { reduced_logical_implication.push_back(n); }
        void didCondenseChild(NodeObsPtr parent, NodeObsPtr merged) override { condensed_children.emplace_back(parent, merged); }
        void didMatchEquivalence(NodeObsPtr n) override { matched_equivalence.push_back(n); }
        void didReduceEquivalence(NodeObsPtr n) override { reduced_equivalence.push_back(n); }
        void didMatchImplication(NodeObsPtr n) override { matched_implication.push_back(n); }
        void didReduceImplication(NodeObsPtr n) override { reduced_implication.push_back(n); }
        void didMatchDeMorganConjunction(NodeObsPtr n) override { matched_de_morgan_conjunction.push_back(n); }
        void didApplyDeMorganConjunction(NodeObsPtr n) override { applied_de_morgan_conjunction.push_back(n); }
        void didMatchDeMorganDisjunction(NodeObsPtr n) override { matched_de_morgan_disjunction.push_back(n); }
        void didApplyDeMorganDisjunction(NodeObsPtr n) override { applied_de_morgan_disjunction.push_back(n); }
        void didMatchDoubleNegation(NodeObsPtr n) override { matched_double_negation.push_back(n); }
        void didEliminateDoubleNegation(NodeObsPtr n) override { eliminated_double_negation.push_back(n); }
        void didMatchInverter(NodeObsPtr n) override { matched_inverter.push_back(n); }
        void didInvertConstant(NodeObsPtr n) override { inverted_constant.push_back(n); }
        void didMatchNeutralElement(NodeObsPtr n) override { matched_neutral_element.push_back(n); }
        void didCollapseToContradiction(NodeObsPtr n) override { collapsed_to_contradiction.push_back(n); }
        void didCollapseToTautology(NodeObsPtr n) override { collapsed_to_tautology.push_back(n); }
        void didDropNeutralElement(NodeObsPtr n) override { dropped_neutral_element.push_back(n); }
        void didMatchAbsorption(NodeObsPtr n) override { matched_absorption.push_back(n); }
        void didApplyAbsorption(NodeObsPtr n) override { applied_absorption.push_back(n); }
        void didMatchAndDistribution(NodeObsPtr n) override { matched_and_distribution.push_back(n); }
        void didDistributeAndOverOr(NodeObsPtr n) override { distributed_and_over_or.push_back(n); }
        void didMatchOrDistribution(NodeObsPtr n) override { matched_or_distribution.push_back(n); }
        void didDistributeOrOverAnd(NodeObsPtr n) override { distributed_or_over_and.push_back(n); }
        void didCheckCNF(NodeObsPtr n, bool is_cnf) override { checked_cnf.emplace_back(n, is_cnf); }
        void didCheckDNF(NodeObsPtr n, bool is_dnf) override { checked_dnf.emplace_back(n, is_dnf); }
    };
} // namespace

class NodeTransformationListenerTest : public ::testing::Test
{
protected:
    std::shared_ptr<recording_listener> listener;

    void SetUp() override
    {
        listener = std::make_shared<recording_listener>();
    }
};

// --- logical_implication: LEquiOp -> AndOp(l, NotOp(r)) ---

TEST_F(NodeTransformationListenerTest, LogicalImplicationMatchFiresOnLEquiOp)
{
    NodePtr tree = op(lexeme_type::LEquiOp, symbols::SYMB_LEQUI, atom("P"), atom("Q"));
    logical_implication transf;

    EXPECT_TRUE(transf.match(tree.get(), listener));
    ASSERT_EQ(listener->matched_logical_implication.size(), 1u);
    EXPECT_EQ(listener->matched_logical_implication[0], tree.get());
}

TEST_F(NodeTransformationListenerTest, LogicalImplicationMatchDoesNotFireOnOtherOps)
{
    NodePtr tree = atom("P");
    logical_implication transf;

    EXPECT_FALSE(transf.match(tree.get(), listener));
    EXPECT_TRUE(listener->matched_logical_implication.empty());
}

TEST_F(NodeTransformationListenerTest, LogicalImplicationReplaceRewritesAndFiresCallback)
{
    NodePtr tree = op(lexeme_type::LEquiOp, symbols::SYMB_LEQUI, atom("P"), atom("Q"));
    NodeObsPtr root = tree.get();
    logical_implication transf;

    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::AndOp);
    ASSERT_EQ(listener->reduced_logical_implication.size(), 1u);
    EXPECT_EQ(listener->reduced_logical_implication[0], root);
    // Neither operand is itself an AndOp, so condense() has nothing to merge.
    EXPECT_TRUE(listener->condensed_children.empty());
}

TEST_F(NodeTransformationListenerTest, LogicalImplicationReplaceFiresCondenseCallbackWhenItFlattens)
{
    // LEquiOp(AndOp(P,Q), R): once relabeled AndOp, the left operand is
    // itself an AndOp, so condense() must flatten it into a 3-ary AndOp,
    // firing didCondenseChild once for the merged AndOp(P,Q) node.
    NodePtr tree = op(lexeme_type::LEquiOp, symbols::SYMB_LEQUI,
        op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"), atom("Q")), atom("R"));
    NodeObsPtr root = tree.get();
    logical_implication transf;

    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::AndOp);
    ASSERT_EQ(root->num_children(), 3u);
    EXPECT_EQ(root->childAt(0)->getType(), NodeType::NotOp);
    EXPECT_EQ(root->childAt(0)->childAt(0)->getLexeme().token(), "R");
    EXPECT_EQ(root->childAt(1)->getLexeme().token(), "P");
    EXPECT_EQ(root->childAt(2)->getLexeme().token(), "Q");

    ASSERT_EQ(listener->condensed_children.size(), 1u);
    EXPECT_EQ(listener->condensed_children[0].first, root);
}

// --- equivalence_reduction: IffOp -> OrOp(AndOp(l,r), AndOp(NotOp(l),NotOp(r))) ---

TEST_F(NodeTransformationListenerTest, EquivalenceReductionMatchFiresOnIffOp)
{
    NodePtr tree = op(lexeme_type::IffOp, symbols::SYMB_IFF, atom("P"), atom("Q"));
    EquivalenceReduction transf;

    EXPECT_TRUE(transf.match(tree.get(), listener));
    ASSERT_EQ(listener->matched_equivalence.size(), 1u);
    EXPECT_EQ(listener->matched_equivalence[0], tree.get());
}

TEST_F(NodeTransformationListenerTest, EquivalenceReductionMatchDoesNotFireOnOtherOps)
{
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"), atom("Q"));
    EquivalenceReduction transf;

    EXPECT_FALSE(transf.match(tree.get(), listener));
    EXPECT_TRUE(listener->matched_equivalence.empty());
}

TEST_F(NodeTransformationListenerTest, EquivalenceReductionReplaceRewritesAndFiresCallback)
{
    NodePtr tree = op(lexeme_type::IffOp, symbols::SYMB_IFF, atom("P"), atom("Q"));
    NodeObsPtr root = tree.get();
    EquivalenceReduction transf;

    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::OrOp);
    ASSERT_EQ(root->num_children(), 2u);
    EXPECT_EQ(root->childAt(0)->getType(), NodeType::AndOp);
    EXPECT_EQ(root->childAt(1)->getType(), NodeType::AndOp);
    ASSERT_EQ(listener->reduced_equivalence.size(), 1u);
    EXPECT_EQ(listener->reduced_equivalence[0], root);
}

// --- implication_reduction: ImpliesOp -> OrOp(NotOp(l), r) ---

TEST_F(NodeTransformationListenerTest, ImplicationReductionMatchFiresOnImpliesOp)
{
    NodePtr tree = op(lexeme_type::ImpliesOp, symbols::SYMB_IMPL, atom("P"), atom("Q"));
    ImplicationReduction transf;

    EXPECT_TRUE(transf.match(tree.get(), listener));
    ASSERT_EQ(listener->matched_implication.size(), 1u);
    EXPECT_EQ(listener->matched_implication[0], tree.get());
}

TEST_F(NodeTransformationListenerTest, ImplicationReductionMatchDoesNotFireOnOtherOps)
{
    NodePtr tree = op(lexeme_type::OrOp, symbols::SYMB_OR, atom("P"), atom("Q"));
    ImplicationReduction transf;

    EXPECT_FALSE(transf.match(tree.get(), listener));
    EXPECT_TRUE(listener->matched_implication.empty());
}

TEST_F(NodeTransformationListenerTest, ImplicationReductionReplaceRewritesAndFiresCallback)
{
    NodePtr tree = op(lexeme_type::ImpliesOp, symbols::SYMB_IMPL, atom("P"), atom("Q"));
    NodeObsPtr root = tree.get();
    ImplicationReduction transf;

    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::OrOp);
    ASSERT_EQ(root->num_children(), 2u);
    EXPECT_EQ(root->childAt(0)->getType(), NodeType::NotOp);
    EXPECT_EQ(root->childAt(0)->childAt(0)->getLexeme().token(), "P");
    EXPECT_EQ(root->childAt(1)->getLexeme().token(), "Q");
    ASSERT_EQ(listener->reduced_implication.size(), 1u);
    EXPECT_EQ(listener->reduced_implication[0], root);
}

// --- de_morgan: NotOp(OrOp(l,r)) -> AndOp(NotOp(l),NotOp(r)) and its mirror ---

TEST_F(NodeTransformationListenerTest, DeMorganDisjunctionMatchAndReplace)
{
    NodePtr tree = op(lexeme_type::NotOp, symbols::SYMB_NOT,
        op(lexeme_type::OrOp, symbols::SYMB_OR, atom("P"), atom("Q")));
    NodeObsPtr root = tree.get();
    DeMorganDisjunction transf;

    EXPECT_TRUE(transf.match(root, listener));
    ASSERT_EQ(listener->matched_de_morgan_disjunction.size(), 1u);
    EXPECT_EQ(listener->matched_de_morgan_disjunction[0], root);

    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::AndOp);
    ASSERT_EQ(root->num_children(), 2u);
    EXPECT_EQ(root->childAt(0)->getType(), NodeType::NotOp);
    EXPECT_EQ(root->childAt(0)->childAt(0)->getLexeme().token(), "P");
    EXPECT_EQ(root->childAt(1)->getType(), NodeType::NotOp);
    EXPECT_EQ(root->childAt(1)->childAt(0)->getLexeme().token(), "Q");
    ASSERT_EQ(listener->applied_de_morgan_disjunction.size(), 1u);
    EXPECT_EQ(listener->applied_de_morgan_disjunction[0], root);
}

TEST_F(NodeTransformationListenerTest, DeMorganConjunctionMatchAndReplace)
{
    NodePtr tree = op(lexeme_type::NotOp, symbols::SYMB_NOT,
        op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"), atom("Q")));
    NodeObsPtr root = tree.get();
    DeMorganConjunction transf;

    EXPECT_TRUE(transf.match(root, listener));
    ASSERT_EQ(listener->matched_de_morgan_conjunction.size(), 1u);
    EXPECT_EQ(listener->matched_de_morgan_conjunction[0], root);

    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::OrOp);
    ASSERT_EQ(root->num_children(), 2u);
    EXPECT_EQ(root->childAt(0)->getType(), NodeType::NotOp);
    EXPECT_EQ(root->childAt(0)->childAt(0)->getLexeme().token(), "P");
    EXPECT_EQ(root->childAt(1)->getType(), NodeType::NotOp);
    EXPECT_EQ(root->childAt(1)->childAt(0)->getLexeme().token(), "Q");
    ASSERT_EQ(listener->applied_de_morgan_conjunction.size(), 1u);
    EXPECT_EQ(listener->applied_de_morgan_conjunction[0], root);
}

TEST_F(NodeTransformationListenerTest, DeMorganDoesNotFireWhenPatternAbsent)
{
    NodePtr tree = op(lexeme_type::NotOp, symbols::SYMB_NOT, atom("P"));
    DeMorganDisjunction disj;
    DeMorganConjunction conj;

    EXPECT_FALSE(disj.match(tree.get(), listener));
    EXPECT_FALSE(conj.match(tree.get(), listener));
    EXPECT_TRUE(listener->matched_de_morgan_disjunction.empty());
    EXPECT_TRUE(listener->matched_de_morgan_conjunction.empty());
}

// --- double_negation: NotOp(NotOp(x)) -> x ---

TEST_F(NodeTransformationListenerTest, DoubleNegationMatchAndReplace)
{
    NodePtr tree = op(lexeme_type::NotOp, symbols::SYMB_NOT,
        op(lexeme_type::NotOp, symbols::SYMB_NOT, atom("P")));
    NodeObsPtr root = tree.get();
    DoubleNegation transf;

    EXPECT_TRUE(transf.match(root, listener));
    ASSERT_EQ(listener->matched_double_negation.size(), 1u);
    EXPECT_EQ(listener->matched_double_negation[0], root);

    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::Atom);
    EXPECT_EQ(root->getLexeme().token(), "P");
    ASSERT_EQ(listener->eliminated_double_negation.size(), 1u);
    EXPECT_EQ(listener->eliminated_double_negation[0], root);
}

TEST_F(NodeTransformationListenerTest, DoubleNegationDoesNotFireOnSingleNegation)
{
    NodePtr tree = op(lexeme_type::NotOp, symbols::SYMB_NOT, atom("P"));
    DoubleNegation transf;

    EXPECT_FALSE(transf.match(tree.get(), listener));
    EXPECT_TRUE(listener->matched_double_negation.empty());
}

// --- inverter: NotOp(Tautology|Contradiction) -> the opposite constant ---

TEST_F(NodeTransformationListenerTest, InverterFlipsTautologyToContradiction)
{
    NodePtr tree = op(lexeme_type::NotOp, symbols::SYMB_NOT,
        constant(lexeme_type::Tautology, symbols::SYMB_TAUTOLOGY));
    NodeObsPtr root = tree.get();
    Inverter transf;

    EXPECT_TRUE(transf.match(root, listener));
    ASSERT_EQ(listener->matched_inverter.size(), 1u);

    transf.replace(root, listener);

    EXPECT_EQ(root->getLexeme().type(), lexeme_type::Contradiction);
    EXPECT_EQ(root->num_children(), 0u);
    ASSERT_EQ(listener->inverted_constant.size(), 1u);
    EXPECT_EQ(listener->inverted_constant[0], root);
}

TEST_F(NodeTransformationListenerTest, InverterFlipsContradictionToTautology)
{
    NodePtr tree = op(lexeme_type::NotOp, symbols::SYMB_NOT,
        constant(lexeme_type::Contradiction, symbols::SYMB_CONTRADICTION));
    NodeObsPtr root = tree.get();
    Inverter transf;

    EXPECT_TRUE(transf.match(root, listener));
    transf.replace(root, listener);

    EXPECT_EQ(root->getLexeme().type(), lexeme_type::Tautology);
    ASSERT_EQ(listener->inverted_constant.size(), 1u);
    EXPECT_EQ(listener->inverted_constant[0], root);
}

TEST_F(NodeTransformationListenerTest, InverterDoesNotFireOnPlainNegation)
{
    NodePtr tree = op(lexeme_type::NotOp, symbols::SYMB_NOT, atom("P"));
    Inverter transf;

    EXPECT_FALSE(transf.match(tree.get(), listener));
    EXPECT_TRUE(listener->matched_inverter.empty());
}

// --- absorption_transformation: x AND (x OR y) -> x, and the OrOp mirror ---

TEST_F(NodeTransformationListenerTest, AbsorptionCollapsesConjunctionOverDisjunction)
{
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"),
        op(lexeme_type::OrOp, symbols::SYMB_OR, atom("P"), atom("Q")));
    NodeObsPtr root = tree.get();
    absorption_transformation transf;

    EXPECT_TRUE(transf.match(root, listener));
    ASSERT_EQ(listener->matched_absorption.size(), 1u);
    EXPECT_EQ(listener->matched_absorption[0], root);

    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::Atom);
    EXPECT_EQ(root->getLexeme().token(), "P");
    ASSERT_EQ(listener->applied_absorption.size(), 1u);
    EXPECT_EQ(listener->applied_absorption[0], root);
}

TEST_F(NodeTransformationListenerTest, AbsorptionCollapsesDisjunctionOverConjunction)
{
    NodePtr tree = op(lexeme_type::OrOp, symbols::SYMB_OR, atom("P"),
        op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"), atom("Q")));
    NodeObsPtr root = tree.get();
    absorption_transformation transf;

    EXPECT_TRUE(transf.match(root, listener));
    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::Atom);
    EXPECT_EQ(root->getLexeme().token(), "P");
    ASSERT_EQ(listener->applied_absorption.size(), 1u);
    EXPECT_EQ(listener->applied_absorption[0], root);
}

TEST_F(NodeTransformationListenerTest, AbsorptionDoesNotFireWithoutARedundantChild)
{
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"),
        op(lexeme_type::OrOp, symbols::SYMB_OR, atom("Q"), atom("R")));
    absorption_transformation transf;

    EXPECT_FALSE(transf.match(tree.get(), listener));
    EXPECT_TRUE(listener->matched_absorption.empty());
}

// --- neutral_elements: constants short-circuit or get dropped from n-ary nodes ---

TEST_F(NodeTransformationListenerTest, NeutralElementsCollapsesConjunctionContradiction)
{
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"),
        constant(lexeme_type::Contradiction, symbols::SYMB_CONTRADICTION));
    NodeObsPtr root = tree.get();
    NeutralElements transf;

    EXPECT_TRUE(transf.match(root, listener));
    ASSERT_EQ(listener->matched_neutral_element.size(), 1u);

    transf.replace(root, listener);

    EXPECT_EQ(root->getLexeme().type(), lexeme_type::Contradiction);
    EXPECT_EQ(root->num_children(), 0u);
    ASSERT_EQ(listener->collapsed_to_contradiction.size(), 1u);
    EXPECT_EQ(listener->collapsed_to_contradiction[0], root);
    EXPECT_TRUE(listener->dropped_neutral_element.empty());
}

TEST_F(NodeTransformationListenerTest, NeutralElementsCollapsesDisjunctionTautology)
{
    NodePtr tree = op(lexeme_type::OrOp, symbols::SYMB_OR, atom("P"),
        constant(lexeme_type::Tautology, symbols::SYMB_TAUTOLOGY));
    NodeObsPtr root = tree.get();
    NeutralElements transf;

    EXPECT_TRUE(transf.match(root, listener));
    transf.replace(root, listener);

    EXPECT_EQ(root->getLexeme().type(), lexeme_type::Tautology);
    EXPECT_EQ(root->num_children(), 0u);
    ASSERT_EQ(listener->collapsed_to_tautology.size(), 1u);
    EXPECT_EQ(listener->collapsed_to_tautology[0], root);
}

TEST_F(NodeTransformationListenerTest, NeutralElementsDropsEachRedundantTautologyInAConjunction)
{
    // AndOp(P, top, top): both tautologies are direct children, so a single
    // replace() call must cascade through two separate drop steps.
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"),
        constant(lexeme_type::Tautology, symbols::SYMB_TAUTOLOGY),
        constant(lexeme_type::Tautology, symbols::SYMB_TAUTOLOGY));
    NodeObsPtr root = tree.get();
    NeutralElements transf;

    EXPECT_TRUE(transf.match(root, listener));
    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::AndOp);
    ASSERT_EQ(root->num_children(), 1u);
    EXPECT_EQ(root->childAt(0)->getLexeme().token(), "P");
    EXPECT_EQ(listener->dropped_neutral_element.size(), 2u);
    EXPECT_EQ(listener->dropped_neutral_element[0], root);
    EXPECT_EQ(listener->dropped_neutral_element[1], root);
    EXPECT_TRUE(listener->collapsed_to_contradiction.empty());
    EXPECT_TRUE(listener->collapsed_to_tautology.empty());
}

TEST_F(NodeTransformationListenerTest, NeutralElementsDropsAContradictionFromADisjunction)
{
    // OrOp(P, bot): the dual of CONJUNCTION_TAUTOLOGY -- the contradiction is
    // dropped, not short-circuited (that's DISJUNCTION_TAUTOLOGY's job).
    NodePtr tree = op(lexeme_type::OrOp, symbols::SYMB_OR, atom("P"),
        constant(lexeme_type::Contradiction, symbols::SYMB_CONTRADICTION));
    NodeObsPtr root = tree.get();
    NeutralElements transf;

    EXPECT_TRUE(transf.match(root, listener));
    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::OrOp);
    ASSERT_EQ(root->num_children(), 1u);
    EXPECT_EQ(root->childAt(0)->getLexeme().token(), "P");
    ASSERT_EQ(listener->dropped_neutral_element.size(), 1u);
    EXPECT_EQ(listener->dropped_neutral_element[0], root);
    EXPECT_TRUE(listener->collapsed_to_contradiction.empty());
    EXPECT_TRUE(listener->collapsed_to_tautology.empty());
}

TEST_F(NodeTransformationListenerTest, NeutralElementsDoesNotFireWithoutConstants)
{
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"), atom("Q"));
    NeutralElements transf;

    EXPECT_FALSE(transf.match(tree.get(), listener));
    EXPECT_TRUE(listener->matched_neutral_element.empty());
}

// --- distributions: N-ary AND/OR distribution, used while converting to DNF/CNF ---

TEST_F(NodeTransformationListenerTest, AndDistributionMatchAndReplace)
{
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND, atom("O1"),
        op(lexeme_type::OrOp, symbols::SYMB_OR, atom("D1"), atom("D2")));
    NodeObsPtr root = tree.get();
    and_distribution transf;

    EXPECT_TRUE(transf.match(root, listener));
    ASSERT_EQ(listener->matched_and_distribution.size(), 1u);
    EXPECT_EQ(listener->matched_and_distribution[0], root);

    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::OrOp);
    ASSERT_EQ(root->num_children(), 2u);
    EXPECT_EQ(root->childAt(0)->getType(), NodeType::AndOp);
    EXPECT_EQ(root->childAt(1)->getType(), NodeType::AndOp);
    ASSERT_EQ(listener->distributed_and_over_or.size(), 1u);
    EXPECT_EQ(listener->distributed_and_over_or[0], root);
}

TEST_F(NodeTransformationListenerTest, OrDistributionMatchAndReplace)
{
    NodePtr tree = op(lexeme_type::OrOp, symbols::SYMB_OR, atom("O1"),
        op(lexeme_type::AndOp, symbols::SYMB_AND, atom("C1"), atom("C2")));
    NodeObsPtr root = tree.get();
    or_distribution transf;

    EXPECT_TRUE(transf.match(root, listener));
    ASSERT_EQ(listener->matched_or_distribution.size(), 1u);
    EXPECT_EQ(listener->matched_or_distribution[0], root);

    transf.replace(root, listener);

    EXPECT_EQ(root->getType(), NodeType::AndOp);
    ASSERT_EQ(root->num_children(), 2u);
    EXPECT_EQ(root->childAt(0)->getType(), NodeType::OrOp);
    EXPECT_EQ(root->childAt(1)->getType(), NodeType::OrOp);
    ASSERT_EQ(listener->distributed_or_over_and.size(), 1u);
    EXPECT_EQ(listener->distributed_or_over_and[0], root);
}

TEST_F(NodeTransformationListenerTest, DistributionsDoNotFireWithoutAMixedChild)
{
    NodePtr and_tree = op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"), atom("Q"));
    NodePtr or_tree = op(lexeme_type::OrOp, symbols::SYMB_OR, atom("P"), atom("Q"));
    and_distribution and_transf;
    or_distribution or_transf;

    EXPECT_FALSE(and_transf.match(and_tree.get(), listener));
    EXPECT_FALSE(or_transf.match(or_tree.get(), listener));
    EXPECT_TRUE(listener->matched_and_distribution.empty());
    EXPECT_TRUE(listener->matched_or_distribution.empty());
}

// --- cnf_recogniser / dnf_recogniser: report every check, not just matches ---

TEST_F(NodeTransformationListenerTest, CnfRecogniserReportsTrueForACnfTree)
{
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND,
        op(lexeme_type::OrOp, symbols::SYMB_OR, atom("P"), atom("Q")), atom("R"));
    cnf_recogniser recogniser;

    EXPECT_TRUE(recogniser.match(tree.get(), listener));
    ASSERT_EQ(listener->checked_cnf.size(), 1u);
    EXPECT_EQ(listener->checked_cnf[0].first, tree.get());
    EXPECT_TRUE(listener->checked_cnf[0].second);
}

TEST_F(NodeTransformationListenerTest, CnfRecogniserReportsFalseForANonCnfTree)
{
    NodePtr tree = op(lexeme_type::OrOp, symbols::SYMB_OR, atom("P"), atom("Q"));
    cnf_recogniser recogniser;

    EXPECT_FALSE(recogniser.match(tree.get(), listener));
    ASSERT_EQ(listener->checked_cnf.size(), 1u);
    EXPECT_FALSE(listener->checked_cnf[0].second);
}

TEST_F(NodeTransformationListenerTest, DnfRecogniserReportsTrueForADnfTree)
{
    NodePtr tree = op(lexeme_type::OrOp, symbols::SYMB_OR,
        op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"), atom("Q")), atom("R"));
    dnf_recogniser recogniser;

    EXPECT_TRUE(recogniser.match(tree.get(), listener));
    ASSERT_EQ(listener->checked_dnf.size(), 1u);
    EXPECT_EQ(listener->checked_dnf[0].first, tree.get());
    EXPECT_TRUE(listener->checked_dnf[0].second);
}

TEST_F(NodeTransformationListenerTest, DnfRecogniserReportsFalseForANonDnfTree)
{
    NodePtr tree = op(lexeme_type::AndOp, symbols::SYMB_AND, atom("P"), atom("Q"));
    dnf_recogniser recogniser;

    EXPECT_FALSE(recogniser.match(tree.get(), listener));
    ASSERT_EQ(listener->checked_dnf.size(), 1u);
    EXPECT_FALSE(listener->checked_dnf[0].second);
}

} // namespace eloquent::logic::testing
