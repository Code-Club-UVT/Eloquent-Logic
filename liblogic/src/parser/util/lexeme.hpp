//
// Created by vscode on 7/20/26.
//

#ifndef ELOQUENTLOGIC_LEXEME_HPP
#define ELOQUENTLOGIC_LEXEME_HPP

#include <tree_builder.h>
namespace eloquent::logic {
    class lexeme {
        NodeType m_node_type;
        std::string m_token;
        lexeme(NodeType node_type, std::string token) : m_node_type(node_type), m_token(token) {}
        public:
        static lexeme build_from_node(NodeObsPtr ptr);
        NodeType node_type() const ;
        std::string token() const;
    };
}


#endif //ELOQUENTLOGIC_LEXEME_HPP
