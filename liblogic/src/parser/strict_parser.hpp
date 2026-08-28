//
// Created by vscode on 7/31/26.
//

#ifndef ELOQUENTLOGIC_STRICT_PARSER_HPP
#define ELOQUENTLOGIC_STRICT_PARSER_HPP

#include <memory>
#include <vector>

#include "cursor.h"
#include "lexeme.hpp"
#include "lexeme_stream.hpp"
#include "strict_parser_listener_t.hpp"
#include "syntax_tree.hpp"
namespace eloquent::logic {
class strict_parser {
  public:
    static std::shared_ptr<syntax_tree>
    parse(const std::vector<lexeme> &lexemes,
          const std::shared_ptr<strict_parser_listener_t> &listener);
};
} // namespace eloquent::logic

#endif // ELOQUENTLOGIC_STRICT_PARSER_HPP
