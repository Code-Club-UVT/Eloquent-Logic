//
// Created by Bogdan Petru on 10/08/2026.
//

#ifndef ELOQUENTLOGIC_TRANSFORMATION_RUNNER_HPP
#define ELOQUENTLOGIC_TRANSFORMATION_RUNNER_HPP
#include "syntax_tree.hpp"
#include <vector>

#include "absorption_transformation.hpp"
#include "de_morgan.h"
#include "distributions.hpp"
#include "double_negation.h"
#include "equivalence_reduction.h"
#include "implication_reduction.h"
#include "inverter.h"
#include "logical_implication.hpp"
#include "neutral_elements.h"
#include "node_transformation_listener_t.hpp"
#include "tree_operation_base.h"

namespace eloquent {
namespace logic {
class transformation_runner {
    std::array<std::unique_ptr<tree_operation_base>, 7> transformations = {
        std::make_unique<EquivalenceReduction>(),
        std::make_unique<ImplicationReduction>(),
        std::make_unique<DeMorganConjunction>(),
        std::make_unique<DeMorganDisjunction>(),
        std::make_unique<DoubleNegation>(),
        std::make_unique<absorption_transformation>(),
        std::make_unique<NeutralElements>()};

  public:
    void
    to_nnf(std::shared_ptr<syntax_tree> &tree,
           const std::shared_ptr<node_transformation_listener_t> &listener);
    void
    to_dnf(std::shared_ptr<syntax_tree> &tree,
           const std::shared_ptr<node_transformation_listener_t> &listener);
    void
    to_cnf(std::shared_ptr<syntax_tree> &tree,
           const std::shared_ptr<node_transformation_listener_t> &listener);
};
} // namespace logic
} // namespace eloquent

#endif // ELOQUENTLOGIC_TRANSFORMATION_RUNNER_HPP
