//
// Created by Bogdan Petru on 05/08/2026.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <dictionary.h>
#include <lexeme.hpp>
#include <relaxed_parser.hpp>
#include <relaxed_parser_listener.hpp>

#include "token_builders.hpp"
#include "truth_table.hpp"
#include "unbound_variable_error.hpp"
#include "unsupported_operator_error.hpp"

using namespace eloquent::logic;
using namespace eloquent::logic::testing;

namespace
{
    // Records every callback truth_table fires, so tests can assert on both
    // the sequencing and the exact values reported.
    class recording_listener : public truth_table_listener_t
    {
    public:
        bool started = false;
        bool finished = false;
        std::vector<std::pair<std::string, bool>> set_variables;
        std::vector<std::string> unknown_variables;
        std::vector<std::pair<CppCommon::UUID, bool>> computed;

        void didStart() override { started = true; }
        void didFinish() override { finished = true; }

        void didFindUnknownVariable(const std::string_view& name) override
        {
            unknown_variables.emplace_back(name);
        }

        void didSetVariable(const std::string_view& name, bool value) override
        {
            set_variables.emplace_back(name, value);
        }

        void didComputeSubexpression(const CppCommon::UUID& id, bool value) override
        {
            computed.emplace_back(id, value);
        }

        [[nodiscard]] std::optional<bool> valueFor(const CppCommon::UUID& id) const
        {
            for (const auto& [computed_id, value] : computed)
            {
                if (computed_id == id)
                    return value;
            }
            return std::nullopt;
        }
    };
}

class TruthTableTest : public ::testing::Test
{
protected:
    std::shared_ptr<relaxed_parser_listener_t> parser_listener;
    std::shared_ptr<truth_table_listener_t> null_listener;

    void SetUp() override
    {
        parser_listener = std::make_shared<relaxed_parser_listener_t>();
        null_listener = std::make_shared<truth_table_listener_t>();
    }

    std::shared_ptr<syntax_tree> build(const std::vector<lexeme>& tokens)
    {
        return relaxed_parser::parse(make_stream(tokens), parser_listener);
    }
};

TEST_F(TruthTableTest, CollectsEachDistinctAtomOnceAsAVariable)
{
    // P & Q | P
    auto tree = build({
        Atom("P"), Sym(lexeme_type::AndOp, symbols::SYMB_AND), Atom("Q"),
        Sym(lexeme_type::OrOp, symbols::SYMB_OR), Atom("P")
    });
    auto tt = truth_table::make_from_tree(tree);

    EXPECT_THAT(tt.get_variables(), ::testing::UnorderedElementsAre("P", "Q"));
}

TEST_F(TruthTableTest, ExcludesTautologyAndContradictionFromVariables)
{
    // top & P & bot
    auto tree = build({
        Sym(lexeme_type::Tautology, symbols::SYMB_TAUTOLOGY), Sym(lexeme_type::AndOp, symbols::SYMB_AND),
        Atom("P"), Sym(lexeme_type::AndOp, symbols::SYMB_AND),
        Sym(lexeme_type::Contradiction, symbols::SYMB_CONTRADICTION)
    });
    auto tt = truth_table::make_from_tree(tree);

    EXPECT_THAT(tt.get_variables(), ::testing::UnorderedElementsAre("P"));
}

TEST_F(TruthTableTest, EvaluatesAnd)
{
    auto tree = build({ Atom("P"), Sym(lexeme_type::AndOp, symbols::SYMB_AND), Atom("Q") });
    auto tt = truth_table::make_from_tree(tree);
    auto root_id = tree->rootRef()->getUUID();

    auto eval = [&](bool p, bool q) {
        return tt.evaluate({{"P", p}, {"Q", q}}, null_listener).at(root_id);
    };

    EXPECT_TRUE(eval(true, true));
    EXPECT_FALSE(eval(true, false));
    EXPECT_FALSE(eval(false, true));
    EXPECT_FALSE(eval(false, false));
}

TEST_F(TruthTableTest, EvaluatesOr)
{
    auto tree = build({ Atom("P"), Sym(lexeme_type::OrOp, symbols::SYMB_OR), Atom("Q") });
    auto tt = truth_table::make_from_tree(tree);
    auto root_id = tree->rootRef()->getUUID();

    auto eval = [&](bool p, bool q) {
        return tt.evaluate({{"P", p}, {"Q", q}}, null_listener).at(root_id);
    };

    EXPECT_TRUE(eval(true, true));
    EXPECT_TRUE(eval(true, false));
    EXPECT_TRUE(eval(false, true));
    EXPECT_FALSE(eval(false, false));
}

TEST_F(TruthTableTest, EvaluatesImplies)
{
    auto tree = build({ Atom("P"), Sym(lexeme_type::ImpliesOp, symbols::SYMB_IMPL), Atom("Q") });
    auto tt = truth_table::make_from_tree(tree);
    auto root_id = tree->rootRef()->getUUID();

    auto eval = [&](bool p, bool q) {
        return tt.evaluate({{"P", p}, {"Q", q}}, null_listener).at(root_id);
    };

    EXPECT_TRUE(eval(false, false));
    EXPECT_TRUE(eval(false, true));
    EXPECT_FALSE(eval(true, false));
    EXPECT_TRUE(eval(true, true));
}

TEST_F(TruthTableTest, EvaluatesIff)
{
    auto tree = build({ Atom("P"), Sym(lexeme_type::IffOp, symbols::SYMB_IFF), Atom("Q") });
    auto tt = truth_table::make_from_tree(tree);
    auto root_id = tree->rootRef()->getUUID();

    auto eval = [&](bool p, bool q) {
        return tt.evaluate({{"P", p}, {"Q", q}}, null_listener).at(root_id);
    };

    EXPECT_TRUE(eval(true, true));
    EXPECT_TRUE(eval(false, false));
    EXPECT_FALSE(eval(true, false));
    EXPECT_FALSE(eval(false, true));
}

TEST_F(TruthTableTest, EvaluatesNot)
{
    auto tree = build({ Sym(lexeme_type::NotOp, symbols::SYMB_NOT), Atom("P") });
    auto tt = truth_table::make_from_tree(tree);
    auto root_id = tree->rootRef()->getUUID();

    EXPECT_FALSE(tt.evaluate({{"P", true}}, null_listener).at(root_id));
    EXPECT_TRUE(tt.evaluate({{"P", false}}, null_listener).at(root_id));
}

TEST_F(TruthTableTest, EvaluatesDoubleNegation)
{
    auto tree = build({
        Sym(lexeme_type::NotOp, symbols::SYMB_NOT), Sym(lexeme_type::NotOp, symbols::SYMB_NOT), Atom("P")
    });
    auto tt = truth_table::make_from_tree(tree);
    auto root_id = tree->rootRef()->getUUID();

    EXPECT_TRUE(tt.evaluate({{"P", true}}, null_listener).at(root_id));
    EXPECT_FALSE(tt.evaluate({{"P", false}}, null_listener).at(root_id));
}

TEST_F(TruthTableTest, EvaluatesCompositeExpressionRespectingPrecedence)
{
    // P & Q | R  =>  (P & Q) | R, since & binds tighter than |.
    auto tree = build({
        Atom("P"), Sym(lexeme_type::AndOp, symbols::SYMB_AND), Atom("Q"),
        Sym(lexeme_type::OrOp, symbols::SYMB_OR), Atom("R")
    });
    auto tt = truth_table::make_from_tree(tree);
    auto root_id = tree->rootRef()->getUUID();

    auto eval = [&](bool p, bool q, bool r) {
        return tt.evaluate({{"P", p}, {"Q", q}, {"R", r}}, null_listener).at(root_id);
    };

    EXPECT_TRUE(eval(true, true, false));
    EXPECT_FALSE(eval(true, false, false));
    EXPECT_TRUE(eval(false, false, true));
    EXPECT_FALSE(eval(false, false, false));
}

TEST_F(TruthTableTest, UnsetVariablesDefaultToFalse)
{
    auto tree = build({ Atom("P"), Sym(lexeme_type::AndOp, symbols::SYMB_AND), Atom("Q") });
    auto tt = truth_table::make_from_tree(tree);
    auto root_id = tree->rootRef()->getUUID();

    // Q is never mentioned in the interpretation, so it keeps the default
    // (false) value make_from_tree seeded it with.
    EXPECT_FALSE(tt.evaluate({{"P", true}}, null_listener).at(root_id));
}

TEST_F(TruthTableTest, ThrowsUnboundVariableErrorForUnrecognizedVariable)
{
    auto tree = build({ Atom("P") });
    auto tt = truth_table::make_from_tree(tree);
    auto listener = std::make_shared<recording_listener>();

    EXPECT_THROW(tt.evaluate({{"Z", true}}, listener), unbound_variable_error);
    ASSERT_EQ(listener->unknown_variables.size(), 1u);
    EXPECT_EQ(listener->unknown_variables[0], "Z");
}

TEST_F(TruthTableTest, UnboundVariableErrorMessageMentionsTheVariable)
{
    auto tree = build({ Atom("P") });
    auto tt = truth_table::make_from_tree(tree);

    try
    {
        tt.evaluate({{"Z", true}}, null_listener);
        FAIL() << "expected unbound_variable_error to be thrown";
    }
    catch (const unbound_variable_error& e)
    {
        EXPECT_NE(std::string(e.what()).find('Z'), std::string::npos);
    }
}

TEST_F(TruthTableTest, ListenerObservesStartFinishAndSetVariables)
{
    auto tree = build({ Atom("P"), Sym(lexeme_type::AndOp, symbols::SYMB_AND), Atom("Q") });
    auto tt = truth_table::make_from_tree(tree);
    auto listener = std::make_shared<recording_listener>();

    tt.evaluate({{"P", true}, {"Q", false}}, listener);

    EXPECT_TRUE(listener->started);
    EXPECT_TRUE(listener->finished);
    EXPECT_THAT(listener->set_variables, ::testing::UnorderedElementsAre(
        std::make_pair(std::string("P"), true),
        std::make_pair(std::string("Q"), false)));
}

TEST_F(TruthTableTest, ListenerReportsThePostNegationValueForNotNodes)
{
    // ~P & Q  =>  AndOp(NotOp(P), Q)
    auto tree = build({
        Sym(lexeme_type::NotOp, symbols::SYMB_NOT), Atom("P"),
        Sym(lexeme_type::AndOp, symbols::SYMB_AND), Atom("Q")
    });
    auto tt = truth_table::make_from_tree(tree);
    auto listener = std::make_shared<recording_listener>();

    auto root = tree->rootRef();
    auto not_node = root->childAt(0);

    auto tr = tt.evaluate({{"P", false}, {"Q", true}}, listener);

    // P is false, so ~P must be reported (and cached) as true -- not the
    // raw pre-negation child value.
    auto not_reported = listener->valueFor(not_node->getUUID());
    ASSERT_TRUE(not_reported.has_value());
    EXPECT_TRUE(*not_reported);
    EXPECT_EQ(tr.at(not_node->getUUID()), *not_reported);

    // ~P (true) & Q (true) => true
    auto root_reported = listener->valueFor(root->getUUID());
    ASSERT_TRUE(root_reported.has_value());
    EXPECT_TRUE(*root_reported);
    EXPECT_EQ(tr.at(root->getUUID()), *root_reported);
}

TEST_F(TruthTableTest, EvaluateAllEnumeratesEveryRowForAnd)
{
    auto tree = build({ Atom("P"), Sym(lexeme_type::AndOp, symbols::SYMB_AND), Atom("Q") });
    auto tt = truth_table::make_from_tree(tree);
    auto root_id = tree->rootRef()->getUUID();

    auto rows = tt.evaluate_all(null_listener);
    ASSERT_EQ(rows.size(), 4u);

    size_t true_count = 0;
    for (const auto& row : rows)
        if (row.at(root_id))
            ++true_count;

    // Exactly one of the four rows (P=Q=true) satisfies AND.
    EXPECT_EQ(true_count, 1u);
}

TEST_F(TruthTableTest, EvaluateAllEnumeratesEveryRowForOr)
{
    auto tree = build({ Atom("P"), Sym(lexeme_type::OrOp, symbols::SYMB_OR), Atom("Q") });
    auto tt = truth_table::make_from_tree(tree);
    auto root_id = tree->rootRef()->getUUID();

    auto rows = tt.evaluate_all(null_listener);
    ASSERT_EQ(rows.size(), 4u);

    size_t true_count = 0;
    for (const auto& row : rows)
        if (row.at(root_id))
            ++true_count;

    // Only P=Q=false fails OR, so three of the four rows are true.
    EXPECT_EQ(true_count, 3u);
}

TEST_F(TruthTableTest, EvaluateAllMatchesTheExpectedAssignmentForEveryRow)
{
    // P -> (Q & R); implication binds looser than and, so this needs no parens.
    auto tree = build({
        Atom("P"), Sym(lexeme_type::ImpliesOp, symbols::SYMB_IMPL),
        Atom("Q"), Sym(lexeme_type::AndOp, symbols::SYMB_AND), Atom("R")
    });
    auto tt = truth_table::make_from_tree(tree);
    auto root_id = tree->rootRef()->getUUID();

    auto vars = tt.get_variables();
    ASSERT_EQ(vars.size(), 3u);

    auto rows = tt.evaluate_all(null_listener);
    ASSERT_EQ(rows.size(), 8u);

    for (size_t i = 0; i < rows.size(); ++i)
    {
        std::unordered_map<std::string, bool> assignment;
        for (size_t j = 0; j < vars.size(); ++j)
            assignment[vars[j]] = static_cast<bool>(i & (static_cast<size_t>(1) << j));

        bool expected = !assignment.at("P") || (assignment.at("Q") && assignment.at("R"));
        EXPECT_EQ(rows[i].at(root_id), expected) << "mismatched row " << i;
    }
}

TEST_F(TruthTableTest, EvaluateAllHandlesAFormulaWithNoVariables)
{
    auto tree = build({ Sym(lexeme_type::Tautology, symbols::SYMB_TAUTOLOGY) });
    auto tt = truth_table::make_from_tree(tree);

    EXPECT_TRUE(tt.get_variables().empty());

    auto rows = tt.evaluate_all(null_listener);
    EXPECT_EQ(rows.size(), 1u);
}

TEST_F(TruthTableTest, ThrowsUnsupportedOperatorErrorForModelsOperator)
{
    // P models Q -- semantic entailment is not a truth-functional connective.
    auto tree = build({ Atom("P"), Sym(lexeme_type::LEquiOp, symbols::SYMB_LEQUI), Atom("Q") });
    auto tt = truth_table::make_from_tree(tree);

    EXPECT_THROW(tt.evaluate({{"P", true}, {"Q", true}}, null_listener), unsupported_operator_error);
}

TEST_F(TruthTableTest, UnsupportedOperatorErrorMessageMentionsTheOperatorToken)
{
    auto tree = build({ Atom("P"), Sym(lexeme_type::LEquiOp, symbols::SYMB_LEQUI), Atom("Q") });
    auto tt = truth_table::make_from_tree(tree);

    try
    {
        tt.evaluate({{"P", true}, {"Q", true}}, null_listener);
        FAIL() << "expected unsupported_operator_error to be thrown";
    }
    catch (const unsupported_operator_error& e)
    {
        EXPECT_NE(std::string(e.what()).find(symbols::SYMB_LEQUI), std::string::npos);
    }
}
