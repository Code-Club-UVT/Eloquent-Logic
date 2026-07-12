//
// Created by xcell on 24.02.2025.
//

#pragma once
#include <../../include/liblogic/tree_sanitizer.h>

namespace eloquent::logic::tests{
    CursorPrivate build_tree(const std::string& exp, bool n_izer_switch = false);

    std::vector<CursorPrivate> build_tree_array(const std::vector<std::string>& battery, bool n_izer_switch = false);

    bool are_tree_arrays_equal(const std::vector<CursorPrivate>& battery, const std::vector<CursorPrivate>& answers);
}