//
// Tests for eloquent::logic::logical_consequence::solve_logical_consequence:
// Premises \models Conclusion holds iff Premises & ~Conclusion is
// unsatisfiable (logical_implication rewrites LEquiOp -> AndOp(l, NotOp(r)),
// then transformation_runner::to_cnf + mapping_generator::gen_map + dpll_mcl
// decide satisfiability of the result). Formulas are built via the real
// relaxed_parser (like tests/truth_table/truth_table_tests.cpp) rather than
// hand-built trees, since \models's precedence relative to the other
// operators is itself part of what's under test.
//
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <dictionary.h>
#include <lexeme.hpp>
#include <relaxed_parser.hpp>
#include <relaxed_parser_listener.hpp>

#include "incompatible_top_level_operator.hpp"
#include "logical_consequence.hpp"
#include "logical_consequence_listener_t.hpp"
#include "mapper_listener.hpp"
#include "node_transformation_listener_t.hpp"
#include "sat_listener.hpp"
#include "token_builders.hpp"

using namespace eloquent::logic;
using namespace eloquent::logic::testing;

namespace {
// Records every callback logical_consequence_listener_t fires, in call
// order, so tests can assert both sequencing and the exact reported value.
class recording_listener : public logical_consequence_listener_t {
  public:
    std::vector<std::string> events;
    std::optional<bool> reported_result;

    void didStart() override { events.emplace_back("didStart"); }
    void didFinish() override { events.emplace_back("didFinish"); }

    void not_logical_equivalence() override {
        events.emplace_back("not_logical_equivalence");
    }

    void finalised_with_result(bool result) override {
        events.emplace_back("finalised_with_result");
        reported_result = result;
    }
};

lexeme Models() { return Sym(lexeme_type::LEquiOp, symbols::SYMB_LEQUI); }
lexeme And() { return Sym(lexeme_type::AndOp, symbols::SYMB_AND); }
lexeme Or() { return Sym(lexeme_type::OrOp, symbols::SYMB_OR); }
lexeme Not() { return Sym(lexeme_type::NotOp, symbols::SYMB_NOT); }
lexeme Implies() { return Sym(lexeme_type::ImpliesOp, symbols::SYMB_IMPL); }
lexeme LParen() { return Sym(lexeme_type::LParen, "("); }
lexeme RParen() { return Sym(lexeme_type::RParen, ")"); }
} // namespace

class LogicalConsequenceTest : public ::testing::Test {
  protected:
    std::shared_ptr<relaxed_parser_listener_t> parser_listener;

    void SetUp() override {
        parser_listener = std::make_shared<relaxed_parser_listener_t>();
    }

    std::shared_ptr<syntax_tree> build(const std::vector<lexeme> &tokens) {
        return relaxed_parser::parse(make_stream(tokens), parser_listener);
    }

    // Runs solve_logical_consequence with `listener` for the
    // logical_consequence-level callbacks and plain, default-constructed
    // (no-op) listeners for the sub-algorithms these tests aren't
    // exercising directly -- mirrors silent_listener() in
    // tests/transformations/transformation_runner_tests.cpp.
    static bool solve(std::shared_ptr<syntax_tree> &tree,
                       const std::shared_ptr<recording_listener> &listener) {
        return logical_consequence::solve_logical_consequence(
            tree, listener, std::make_shared<node_transformation_listener_t>(),
            std::make_shared<sat_listener>(),
            std::make_shared<mapper_listener>());
    }
};

TEST_F(LogicalConsequenceTest, ReflexivePremiseEntailsItself) {
    // P \models P
    auto tree = build({Atom("P"), Models(), Atom("P")});
    auto listener = std::make_shared<recording_listener>();

    EXPECT_TRUE(solve(tree, listener));
}

TEST_F(LogicalConsequenceTest, ModusPonensHolds) {
    // P & (P -> Q) \models Q -- \models has the lowest precedence, so the
    // inner parens (needed since & binds tighter than ->) are the only
    // ones required.
    auto tree = build({Atom("P"), And(), LParen(), Atom("P"), Implies(),
                       Atom("Q"), RParen(), Models(), Atom("Q")});
    auto listener = std::make_shared<recording_listener>();

    EXPECT_TRUE(solve(tree, listener));
}

TEST_F(LogicalConsequenceTest, ModusTollensHolds) {
    // (P -> Q) & ~Q \models ~P -- parens around "P -> Q" are required,
    // since without them -> (which binds looser than &) would swallow the
    // rest of the conjunction as its consequent.
    auto tree = build({LParen(), Atom("P"), Implies(), Atom("Q"), RParen(),
                       And(), Not(), Atom("Q"), Models(), Not(), Atom("P")});
    auto listener = std::make_shared<recording_listener>();

    EXPECT_TRUE(solve(tree, listener));
}

TEST_F(LogicalConsequenceTest, WeakeningHolds) {
    // P \models (P | Q) -- no parens needed: | binds tighter than \models.
    auto tree = build({Atom("P"), Models(), Atom("P"), Or(), Atom("Q")});
    auto listener = std::make_shared<recording_listener>();

    EXPECT_TRUE(solve(tree, listener));
}

TEST_F(LogicalConsequenceTest, UnrelatedAtomsDoNotEntail) {
    // P \models Q -- P & ~Q is satisfiable (P=true, Q=false), so this does
    // not hold.
    auto tree = build({Atom("P"), Models(), Atom("Q")});
    auto listener = std::make_shared<recording_listener>();

    EXPECT_FALSE(solve(tree, listener));
}

TEST_F(LogicalConsequenceTest, DisjunctionDoesNotEntailADisjunct) {
    // (A | B) \models A -- (A|B) & ~A is satisfiable (A=false, B=true).
    auto tree = build({Atom("A"), Or(), Atom("B"), Models(), Atom("A")});
    auto listener = std::make_shared<recording_listener>();

    EXPECT_FALSE(solve(tree, listener));
}

TEST_F(LogicalConsequenceTest,
       ListenerObservesStartFinaliseWithResultThenFinishInOrder) {
    auto tree = build({Atom("P"), Models(), Atom("P")});
    auto listener = std::make_shared<recording_listener>();

    solve(tree, listener);

    EXPECT_THAT(listener->events,
                ::testing::ElementsAre("didStart", "finalised_with_result",
                                       "didFinish"));
    ASSERT_TRUE(listener->reported_result.has_value());
    EXPECT_TRUE(*listener->reported_result);
}

TEST_F(LogicalConsequenceTest, ListenerReportsFalseForAnUnrelatedEntailment) {
    auto tree = build({Atom("P"), Models(), Atom("Q")});
    auto listener = std::make_shared<recording_listener>();

    solve(tree, listener);

    ASSERT_TRUE(listener->reported_result.has_value());
    EXPECT_FALSE(*listener->reported_result);
}

TEST_F(LogicalConsequenceTest,
       ThrowsIncompatibleTopLevelOperatorWhenRootIsNotLEquiOp) {
    // Plain P & Q, no top-level \models.
    auto tree = build({Atom("P"), And(), Atom("Q")});
    auto listener = std::make_shared<recording_listener>();

    EXPECT_THROW(solve(tree, listener), incompatible_top_level_operator);
}

TEST_F(LogicalConsequenceTest,
       IncompatibleTopLevelOperatorFiresNotLogicalEquivalenceButNotFinish) {
    auto tree = build({Atom("P"), And(), Atom("Q")});
    auto listener = std::make_shared<recording_listener>();

    EXPECT_THROW(solve(tree, listener), incompatible_top_level_operator);

    // didStart still fires (it's unconditional), but the throw happens
    // before finalised_with_result/didFinish ever run.
    EXPECT_THAT(listener->events,
                ::testing::ElementsAre("didStart", "not_logical_equivalence"));
    EXPECT_FALSE(listener->reported_result.has_value());
}
