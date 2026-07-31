//
// Created by vscode on 7/20/26.
//

#ifndef ELOQUENTLOGIC_LEXEME_HPP
#define ELOQUENTLOGIC_LEXEME_HPP

#include "node.h"

#include <utility>
namespace eloquent::logic {

    enum class lexeme_type
    {
        Atom,
        LParen,
        RParen,
        NotOp,
        AndOp,
        OrOp,
        ImpliesOp,
        IffOp,
        LEquiOp,
        Eof,
        Unknown // blank
    };
    lexeme_type make_from_node(NodeType nt);



    class lexeme {
        lexeme_type m_node_type;
        std::string m_token;
        size_t m_start;
        size_t m_end;
        lexeme(lexeme_type node_type, std::string token, size_t start, size_t end) : m_node_type(node_type), m_token(std::move(token)), m_start(start), m_end(end){}
        public:
        static lexeme make(lexeme_type node_type, std::string token, size_t start, size_t end);
        [[nodiscard]] lexeme_type type() const ;
        [[nodiscard]] std::string token() const;
        [[nodiscard]] size_t position() const;
    };
}


#endif //ELOQUENTLOGIC_LEXEME_HPP
