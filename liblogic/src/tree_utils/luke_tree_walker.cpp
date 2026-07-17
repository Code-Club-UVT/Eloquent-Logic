#include <tree_walker.h>

namespace eloquent::logic {
    void luke_tree_walker::walk() const {
        this->strategy->start();
    }

}