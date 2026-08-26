//
// Created by Bogdan Petru on 27/08/2026.
//

#include "logical_consequence.hpp"

#include "../sat/dpll_mcl/dpll_mcl.h"
#include "incompatible_top_level_operator.hpp"
#include "logical_implication.hpp"
#include "mapping_generator.hpp"
#include "transformation_runner.hpp"

namespace eloquent::logic {

bool logical_consequence::solve_logical_consequence(
    std::shared_ptr<syntax_tree> &tree,
    const std::shared_ptr<logical_consequence_listener_t> &listener,
    const std::shared_ptr<node_transformation_listener_t> &node_listener,
    const std::shared_ptr<sat_listener> &sat_listener,
    const std::shared_ptr<mapper_listener> &mapper_listener) {
    listener->didStart();
    logical_implication transformer;
    if (!transformer.match(tree->rootRef(), node_listener)) {
        listener->not_logical_equivalence();
        throw incompatible_top_level_operator(tree->rootRef());
    }
    transformer.replace(tree->rootRef(), node_listener);
    transformation_runner runner;
    runner.to_cnf(tree, node_listener);

    auto mappings = mapping_generator::gen_map(tree, mapper_listener);
    auto state = dpll_mcl(mappings.clauses, sat_listener);

    if (state == SatState::UNSAT) {
        listener->finalised_with_result(true);
    } else {
        listener->finalised_with_result(false);
    }
    listener->didFinish();
}
} // namespace eloquent::logic
