//
// Created by Bogdan Petru on 13/08/2026.
//

#ifndef ELOQUENTLOGIC_MAPPING_GENERATOR_HPP
#define ELOQUENTLOGIC_MAPPING_GENERATOR_HPP

#include "common.h"
#include <map>
#include <syntax_tree.hpp>

#include "mapper_listener.hpp"
#include "sat_listener.hpp"

namespace eloquent::logic {
struct conversion_result_t {
    std::map<std::string, Literal> mapping;
    ClauseSet clauses;
};
class mapping_generator {
  public:
    static conversion_result_t
    gen_map(const std::shared_ptr<syntax_tree> &tree,
            const std::shared_ptr<mapper_listener> &listener);
};
} // namespace eloquent::logic

#endif // ELOQUENTLOGIC_MAPPING_GENERATOR_HPP
