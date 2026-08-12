//
// Created by xcell on 07.02.2025.
//

#pragma once
#include <tree_recognizer_base.h>

namespace eloquent::logic {
    class NeutralElements : public TreeRecognizerBase {
        enum class Signal {
            SIG_CLEAR = 0,
            SIG_CONJUNCTION_CONTRADICTION,          // high priority
            SIG_DISJUNCTION_TAUTOLOGY,              // high priority
            SIG_CONJUNCTION_TAUTOLOGY_BINARY,
            SIG_CONJUNCTION_TAUTOLOGY_MULTIPLE,
            SIG_DISJUNCTION_CONTRADICTION_BINARY,
            SIG_DISJUNCTION_CONTRADICTION_MULTIPLE
        };
    public:
        static Signal get_case(const NodeObsPtr& node);
        bool match(NodeObsPtr subtree) override;
        void replace(NodeObsPtr target) override;
    };
}
