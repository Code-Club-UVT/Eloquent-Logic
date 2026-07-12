//
// Created by xcell on 15.12.2024.
//

/**
 *  @file tree_recognizer_base.h
 *  @brief Contains a base class for transformations.
 */

#pragma once
#include <liblogic/private/tree_builder.h>

namespace eloquent::logic {
    /**
     * @class TreeRecognizerBase
     * @brief Base class for other classes responsible for propositional logic transformations.
     */
    class TreeRecognizerBase {
    public:
        virtual ~TreeRecognizerBase() = default;

    private:
        /**
         * @brief Base function which seeks a user-defined pattern, such as a double negation.
         * @param subtree Node observer pointer, which represents the place where the function begins to seek the pattern.
         * @return bool Whether the function found the sought pattern or not.
         */
        virtual bool match(NodeObsPtr subtree) = 0;

        /**
         * @brief Base function which modifies the tree at given node according to a pattern.
         * @param target Node observer pointer, which represents the place according to which the change occurs.
         */
        virtual void replace(NodeObsPtr target) = 0;
    };
}
