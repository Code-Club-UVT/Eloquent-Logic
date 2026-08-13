//
// Created by Bogdan Petru on 13/08/2026.
//

#ifndef ELOQUENTLOGIC_MAPPING_LISTENER_HPP
#define ELOQUENTLOGIC_MAPPING_LISTENER_HPP
#include <map>

#include "common.h"
#include "feedback_listener.hpp"
#include "node_transformation_listener_t.hpp"

namespace eloquent
{
    namespace logic
    {
        class mapper_listener : public feedback_listener
        {
        public:
            CB_FULL(did_map_literals,(const std::map<std::string,Literal>&))
            CB_FULL(did_map_clauses,(const ClauseSet&))
        };
    } // logic
} // eloquent

#endif //ELOQUENTLOGIC_MAPPING_LISTENER_HPP
