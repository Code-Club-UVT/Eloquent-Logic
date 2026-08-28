//
// Regression tests for transformation_runner::to_cnf/to_dnf: specifically,
// that they terminate on formulas whose top-level conjunction/disjunction
// has 3+ operands. Neither the parser nor and_distribution/or_distribution's
// freshly-built wrapper nodes ever produce a flat n-ary AndOp/OrOp for a
// chained same-precedence operator (see relaxed_parser.cpp and the comment
// on node_transformation_listener_tests.cpp's own op() helper) — before the
// fix, cnf_recogniser/dnf_recogniser only accepted a single flat level, so a
// tree that was already logically in normal form but structurally nested
// (e.g. AndOp(clause1, AndOp(clause2, clause3))) made the while loops in
// to_cnf/to_dnf spin forever: nothing else ever matches an AndOp-of-AndOp
// (or OrOp-of-OrOp), so the tree never changed and match() kept returning
// false on every iteration. The fix is Node::condense() now recursing over
// the whole tree (node.cpp) plus transformation_runner.cpp calling it after
// to_nnf and after every loop iteration.
//
#include <gtest/gtest.h>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include <common.h>
#include <dpll_mcl.h>
#include <node.h>
#include <resolution_naive_first_fit.h>
#include <syntax_tree.hpp>
#include <transformation_runner.hpp>

#include "cnf_recogniser.hpp"
#include "dnf_recogniser.hpp"
#include "node_transformation_listener_t.hpp"

namespace eloquent::logic::testing {
namespace {
NodePtr atom(const std::string &name) {
    return Node::make_node(lexeme::make(lexeme_type::Atom, name, 0, 0));
}

NodePtr negate(NodePtr child) {
    NodePtr n = Node::make_node(
        lexeme::make(lexeme_type::NotOp, symbols::SYMB_NOT, 0, 0));
    n->adopt(std::move(child));
    return n;
}

// Builds an operator node and adopts each of `children` in order, so
// tests can express exact (deliberately nested, non-flat) tree shapes
// without going through a parser — see node_transformation_listener_tests.cpp's
// identical helper for why that matters here.
template <typename... Children>
NodePtr op(lexeme_type type, const std::string &sym, Children &&...children) {
    NodePtr n = Node::make_node(lexeme::make(type, sym, 0, 0));
    (n->adopt(std::forward<Children>(children)), ...);
    return n;
}

std::shared_ptr<syntax_tree> tree_from(NodePtr root) {
    auto tree = std::make_shared<syntax_tree>();
    tree->set_root(std::move(root));
    return tree;
}

// A literal is a bare atom (positive) or a NotOp wrapping one Atom child
// (negative), DIMACS-style — mirrors logic_agent/rpc/methods/sat_method.cpp's
// own literal_for/map_to_clauses. mapping_generator::gen_map
// (src/sat/utilities) can't be reused here: it unconditionally passes a
// null node_transformation_listener_t into its own internal
// cnf_recogniser check (a guaranteed null-pointer dereference), and its
// clause-building never handles a negated literal — every clause below
// has at least one.
Literal literal_for(NodeObsPtr node,
                    const std::map<std::string, Literal> &mapping) {
    if (node->getType() == NodeType::NotOp)
        return -mapping.at(node->childAt(0)->getLexeme().token());
    return mapping.at(node->getLexeme().token());
}

ClauseSet map_to_clauses(const std::shared_ptr<syntax_tree> &tree) {
    std::map<std::string, Literal> mapping;
    Literal next = 1;
    tree->walk([&](NodeObsPtr node) {
        const auto &l = node->getLexeme();
        if (l.type() == lexeme_type::Atom && !mapping.contains(l.token()))
            mapping.try_emplace(l.token(), next++);
    });

    auto clause_from = [&](NodeObsPtr clause_node) {
        if (clause_node->getType() != NodeType::OrOp)
            return Clause{literal_for(clause_node, mapping)};
        Clause c;
        clause_node->traverse_children([&](const NodeObsPtr &literal_node) {
            c.emplace(literal_for(literal_node, mapping));
        });
        return c;
    };

    ClauseSet clauses;
    NodeObsPtr root = tree->rootRef();
    if (root->getType() == NodeType::AndOp) {
        root->traverse_children([&](const NodeObsPtr &clause_node) {
            clauses.emplace(clause_from(clause_node));
        });
    } else {
        clauses.emplace(clause_from(root));
    }
    return clauses;
}

// transformation_runner requires a listener; these tests only care about
// the resulting tree shape/algorithm outcome, so a plain no-op one
// (every callback default-implemented by feedback_listener) is enough.
std::shared_ptr<node_transformation_listener_t> silent_listener() {
    return std::make_shared<node_transformation_listener_t>();
}

// (A \vee B) \wedge (\neg A \vee C) \wedge (\neg B \vee \neg C) — the
// exact formula that hung sat/dpll_mcl before the fix. Built
// right-nested (AndOp(clause1, AndOp(clause2, clause3))), matching what
// relaxed_parser actually produces for a chained \wedge (see
// relaxed_parser.cpp's expr()) — never a flat 3-ary AndOp.
std::shared_ptr<syntax_tree> three_clause_sat_formula() {
    return tree_from(
        op(lexeme_type::AndOp, symbols::SYMB_AND,
           op(lexeme_type::OrOp, symbols::SYMB_OR, atom("A"), atom("B")),
           op(lexeme_type::AndOp, symbols::SYMB_AND,
              op(lexeme_type::OrOp, symbols::SYMB_OR, negate(atom("A")),
                 atom("C")),
              op(lexeme_type::OrOp, symbols::SYMB_OR, negate(atom("B")),
                 negate(atom("C"))))));
}
} // namespace

TEST(TransformationRunnerToCnfTest,
     TerminatesAndFlattensAThreeClauseConjunction) {
    auto tree = three_clause_sat_formula();

    transformation_runner runner;
    auto listener = silent_listener();
    runner.to_cnf(tree, listener);

    cnf_recogniser recogniser;
    EXPECT_TRUE(recogniser.match(tree->rootRef(), listener));
    EXPECT_EQ(tree->rootRef()->getType(), NodeType::AndOp);
    EXPECT_EQ(tree->rootRef()->num_children(), 3u);
}

TEST(TransformationRunnerToCnfTest, DpllMclAgreesOnTheThreeClauseConjunction) {
    // The actual reported symptom, end to end: parse (hand-built here instead
    // of via the real parser, per three_clause_sat_formula()'s comment) ->
    // to_cnf -> map to clauses -> dpll_mcl, exactly what sat_method.cpp does
    // for RPC method "sat".
    auto tree = three_clause_sat_formula();

    transformation_runner runner;
    runner.to_cnf(tree, silent_listener());

    ClauseSet clauses = map_to_clauses(tree);
    auto listener = std::make_shared<eloquent::logic::sat_listener>();

    EXPECT_EQ(dpll_mcl(clauses, listener), SatState::SAT);
}

TEST(TransformationRunnerToCnfTest,
     ResolutionAgreesOnAnUnsatFourClauseConjunction) {
    // (A \vee B) \wedge (\neg A \vee C) \wedge \neg B \wedge \neg C — unsat,
    // and the sat/resolution hang reported alongside dpll_mcl's used the
    // same shape (4 conjuncted clauses, same nested-AndOp cause).
    auto tree = tree_from(
        op(lexeme_type::AndOp, symbols::SYMB_AND,
           op(lexeme_type::OrOp, symbols::SYMB_OR, atom("A"), atom("B")),
           op(lexeme_type::AndOp, symbols::SYMB_AND,
              op(lexeme_type::OrOp, symbols::SYMB_OR, negate(atom("A")),
                 atom("C")),
              op(lexeme_type::AndOp, symbols::SYMB_AND, negate(atom("B")),
                 negate(atom("C"))))));

    transformation_runner runner;
    runner.to_cnf(tree, silent_listener());

    ClauseSet clauses = map_to_clauses(tree);
    auto listener = std::make_shared<eloquent::logic::sat_listener>();

    EXPECT_EQ(res(clauses, listener), SatState::UNSAT);
}

TEST(TransformationRunnerToDnfTest,
     TerminatesAndFlattensAThreeTermDisjunction) {
    // A \vee \neg B \vee C: mirrors the to_cnf regression above but for
    // to_dnf — a nested OrOp-of-OrOp chain of 3 literal terms that's already
    // logically in DNF once flattened, needing no actual distribution step
    // (distribution's own behavior is out of scope for this fix/these
    // tests; see the plan for the separately-reported, separately-diagnosed
    // transform/target:"dnf" hang on a formula that does need distribution).
    auto tree = tree_from(op(
        lexeme_type::OrOp, symbols::SYMB_OR, atom("A"),
        op(lexeme_type::OrOp, symbols::SYMB_OR, negate(atom("B")), atom("C"))));

    transformation_runner runner;
    auto listener = silent_listener();
    runner.to_dnf(tree, listener);

    dnf_recogniser recogniser;
    EXPECT_TRUE(recogniser.match(tree->rootRef(), listener));
    EXPECT_EQ(tree->rootRef()->getType(), NodeType::OrOp);
    EXPECT_EQ(tree->rootRef()->num_children(), 3u);
}

} // namespace eloquent::logic::testing
