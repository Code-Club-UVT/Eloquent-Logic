//
// Created by Bogdan Petru on 11/08/2026.
//

#include "cnf_recogniser.hpp"

namespace eloquent {
namespace logic {
namespace {
// A literal is a bare atom or its negation (NotOp wrapping
// exactly one Atom child) — NNF guarantees negation never
// appears any deeper than this.
bool is_literal(NodeObsPtr node) {
    if (node->getType() == NodeType::Atom) {
        return true;
    }
    return node->getType() == NodeType::NotOp && node->num_children() == 1 &&
           node->childAt(0)->getType() == NodeType::Atom;
}

// A CNF clause is a disjunction of literals, or — since a
// clause of exactly one literal has no OrOp wrapper — a bare
// literal.
bool is_clause(NodeObsPtr node) {
    if (is_literal(node)) {
        return true;
    }
    if (node->getType() != NodeType::OrOp) {
        return false;
    }
    bool result = true;
    node->traverse_children([&](auto child) { result &= is_literal(child); });
    return result;
}
} // namespace

bool cnf_recogniser::match(
    NodeObsPtr subtree,
    const std::shared_ptr<node_transformation_listener_t> &listener) {
    // CNF is a conjunction of clauses, or — since a formula of
    // exactly one clause has no AndOp wrapper — a bare clause.
    bool result;
    if (subtree->getType() == NodeType::AndOp) {
        result = true;
        subtree->traverse_children(
            [&](auto node1) { result &= is_clause(node1); });
    } else {
        result = is_clause(subtree);
    }
    listener->didCheckCNF(subtree, result);
    return result;
}
} // namespace logic
} // namespace eloquent
