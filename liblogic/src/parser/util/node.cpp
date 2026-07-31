//
// Created by vscode on 7/31/26.
//
#include "node.h"

namespace eloquent::logic
{
    bool Node::isRoot() const noexcept
    {
        return parent.expired();
    }

    std::vector<NodePtr>& Node::getChildren() noexcept
    {
        return children;
    }

    NodeType Node::getType() const noexcept
    {
        return type;
    }

    std::string Node::getText() const noexcept
    {
        return text;
    }

    NodeObsPtr Node::getParent() const noexcept
    {
        return parent;
    }

    lexeme Node::getLexeme() const noexcept
    {
        return m_lexeme;
    }

    CppCommon::UUID Node::getUUID() const noexcept
    {
        return uuid;
    }

    void Node::spawn_new_child(const lexeme& l)
    {
        this->children.emplace_back(std::make_shared<Node>(l));
    }

    Node::Node(lexeme l) : uuid(CppCommon::UUID::Random())
    {
        switch (l)
        {
        case lexeme_type::Unknown:
                this->type = NodeType::Blank;
            break;
        case lexeme_type::Atom:
            this->type = NodeType::Atom;
            break;
        case lexeme_type::NotOp:
            this->type = NodeType::NotOp;
            break;
        case lexeme_type::AndOp:
            this->type = NodeType::AndOp;
            break;
        case lexeme_type::OrOp:
            this->type = NodeType::OrOp;
            break;
        case lexeme_type::ImpliesOp:
            this->type = NodeType::ImpliesOp;
            break;
        case lexeme_type::IffOp:
            this->type = NodeType::IffOp;
            break;
        case lexeme_type::LEquiOp:
            this->type = NodeType::LEquiOp;
            break;
        default:
            throw std::logic_error("Unknown lexeme type");
        }
        m_lexeme = l;
    }


    Node::Node(const Node& n)
    {
        this->children = n.children;
        this->type = n.type;
        this->m_lexeme = n.m_lexeme;
        this->uuid = n.uuid;
    }
    constexpr bool Node::isAtom() const noexcept
    {
        return type == NodeType::Atom;
    }

    constexpr bool Node::isBlank() const noexcept
    {
        return type == NodeType::Blank;
    }

    std::string Node::to_string()
    {
        std::ostringstream os;
        to_string_impl(os);
        return os.str();
    }

    std::shared_ptr<Node> Node::make_node(NodeType _type, const std::string& _text)
    {
        return std::make_shared<Node>(_type, _text);
    }

    std::ostringstream& Node::to_string_impl(std::ostringstream& ss)
    {
        if (this->isAtom())
        {
            ss<<this->text;
            return ss;
        }
        ss << '(';

        if (this->type == NodeType::NotOp)
        {
            ss<<symbols::SYMB_NOT;
            if (!this->children.empty())
                this->children.back()->to_string_impl(ss);
            ss << ')';
            return ss;
        }
        for (size_t i = 0; i < children.size()-1; ++i)
        {
            children[i]->to_string_impl(ss);
            ss << this->text;
        }
        ss<<this->text;
        children.back()->to_string_impl(ss);
        ss<<')';
        return ss;
    }

    bool operator==(const Node& lhs, const Node& rhs)
    {
        if (lhs.children.size() != rhs.children.size()) {
            return false;
        }
        bool response =  lhs.type == rhs.type
               && lhs.text == rhs.text;
        return response;
    }

    bool operator!=(const Node& lhs, const Node& rhs)
    {
        return !(lhs == rhs);
    }
}
