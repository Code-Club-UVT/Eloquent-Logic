//
// Created by xcell on 15.12.2024.
//

/**
 *  @file tree_recognizer_base.h
 *  @brief Contains a base class for transformations.
 */

#pragma once
#include <node.h>

#include "tree_matcher.hpp"

namespace eloquent::logic {
    /**
     * @class tree_operation_base
     * @brief Base class for other classes responsible for propositional logic transformations.
     */
    class tree_operation_base: public tree_matcher {
    public:
        virtual ~tree_operation_base() = default;

        /**
         * @brief Base function which modifies the tree at given node according to a pattern.
         * @param target Node observer pointer, which represents the place according to which the change occurs.
         */
        virtual void replace(NodeObsPtr target) = 0;

        /**
         * @brief Called to check whether or not operation should be applied automatically. Defaults to true
         * @return True if this operation can be safely applied automatically, false otherwise
         */
        virtual bool should_apply()
        {
            return true;
        }

    };
}
