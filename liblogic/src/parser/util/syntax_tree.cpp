//
// Created by vscode on 7/20/26.
//

#include "syntax_tree.hpp"
namespace eloquent::logic {
   NodePtr syntax_tree::root() {
       return this->m_root;
    }

    void syntax_tree::set_root(NodePtr root) {
       this->m_root.swap(root);
    }

    std::generator<NodePtr> syntax_tree::post_order() const {
       return post_order_impl(m_root);
    }

    std::generator<NodePtr> syntax_tree::post_order_impl(NodePtr node) const {
      for (const auto& child : node->children) {
          co_yield std::ranges::elements_of(post_order_impl(child));
      }
      co_yield node;
      co_return;
    }

    bool operator==(const syntax_tree &lhs, const syntax_tree &rhs)  {
      auto lhs_gen =  lhs.post_order();
      auto rhs_gen = rhs.post_order();

      auto t1_it = lhs_gen.begin();
      auto t2_it = rhs_gen.begin();
      while (t1_it != std::default_sentinel && t2_it != std::default_sentinel) {
         if (*t1_it != *t2_it) {
            return false;
         }
         ++t1_it;
         ++t2_it;
      }
      return t1_it == std::default_sentinel && t2_it == std::default_sentinel;
    }
};