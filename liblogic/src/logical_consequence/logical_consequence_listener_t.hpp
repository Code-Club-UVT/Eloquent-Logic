//
// Created by Bogdan Petru on 27/08/2026.
//

#ifndef ELOQUENTLOGIC_LOGICAL_CONSEQUENCE_LISTENER_T_HPP
#define ELOQUENTLOGIC_LOGICAL_CONSEQUENCE_LISTENER_T_HPP
#include "feedback_listener.hpp"

namespace eloquent {
namespace logic {
class logical_consequence_listener_t : public feedback_listener {
  public:
    CB_N(not_logical_equivalence)
    CB_FULL(finalised_with_result, (bool))
};
} // namespace logic
} // namespace eloquent

#endif // ELOQUENTLOGIC_LOGICAL_CONSEQUENCE_LISTENER_T_HPP
