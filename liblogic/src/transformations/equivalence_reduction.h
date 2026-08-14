//
// Created by xcell on 16.12.2024.
//

#pragma once
#include <tree_recognizer_base.h>

namespace eloquent::logic {
    class EquivalenceReduction : public TreeRecognizerBase {
    public:
        bool match(NodeObsPtr subtree) override;
        void replace(NodeObsPtr target) override;
    };
}