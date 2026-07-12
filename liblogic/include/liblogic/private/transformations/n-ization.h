//
// Created by xcell on 17.12.2024.
//

#pragma once
#include "tree_recognizer_base.h"

namespace eloquent::logic {
    class N_izer : public TreeRecognizerBase {
        std::vector<NodePtr> get_descendants(const NodeObsPtr& subtree);
    public:
        bool match(NodeObsPtr subtree) override;
        void replace(NodeObsPtr target) override;
    };
}