//
// Created by vscode on 7/20/26.
//

#ifndef ELOQUENTLOGIC_LEXEME_HPP
#define ELOQUENTLOGIC_LEXEME_HPP

#include <tree_builder.h>

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
        Unknown
    };
    lexeme_type make_from_node(NodeType nt);



    class lexeme {
        lexeme_type m_node_type;
        std::string m_token;
        lexeme(lexeme_type node_type, std::string token) : m_node_type(node_type), m_token(std::move(token)) {}
        public:
        static lexeme build_from_node(NodeObsPtr ptr);
        static lexeme make(lexeme_type node_type, std::string token);
        [[nodiscard]] lexeme_type type() const ;
        [[nodiscard]] std::string token() const;
    };
}


#endif //ELOQUENTLOGIC_LEXEME_HPP
