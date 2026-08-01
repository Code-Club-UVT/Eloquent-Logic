//
// Created by vscode on 7/31/26.
//
#include "node.h"

namespace eloquent::logic
{
    bool Node::isRoot() const noexcept
    {
        return parent != nullptr;
    }


    NodeType Node::getType() const noexcept
    {
        return type;
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
        this->children.emplace_back(make_node(l));
        this->children.back()->set_parent(this);
    }

    void Node::set_node_type(const lexeme& l)
    {
        switch (l.type())
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
    }

    size_t Node::num_children() const noexcept
    {
        return children.size();
    }
    NodeObsPtr Node::childAt(size_t idx) const
    {
        return children[idx].get();
    }

    bool Node::allChildrenAreWritten() const noexcept
    {
        for (const auto & i : children)
        {
            if (i->getType() == NodeType::Blank)
                return false;
        }
        return true;
    }
    size_t Node::find_by_uuid(CppCommon::UUID id) const noexcept
    {
        for (size_t i = 0; i < children.size(); ++i)
        {
            if (children[i]->uuid == id)
                return i;
        }
        return children.size();
    }
    size_t Node::arity() const noexcept
    {
        return children.size();
    }

    Node::Node(const lexeme& l): m_lexeme(l)
    {
        this->uuid = CppCommon::UUID::Random();
        set_node_type(l);
    }

    void Node::set_lexeme(const lexeme& l)
    {
        this->m_lexeme = l;
        set_node_type(l);
    }


    constexpr bool Node::isAtom() const noexcept
    {
        return type == NodeType::Atom;
    }

    constexpr bool Node::isBlank() const noexcept
    {
        return type == NodeType::Blank;
    }
    void Node::adopt(NodePtr node)
    {
        children.emplace_back(std::move(node));
    }

    void Node::replace_child(size_t idx, NodePtr new_child)
    {
        NodePtr old = std::move(children[idx]);
        new_child->set_parent(this);
        new_child->children[0] = std::move(old);
        children[idx] = std::move(new_child);
    }

    std::string Node::to_string() const
    {
        std::ostringstream os;
        to_string_impl(os);
        return os.str();
    }


    std::unique_ptr<Node> Node::make_node(const lexeme& l)
    {
        return std::make_unique<Node>(l);
    }


    std::ostringstream& Node::to_string_impl(std::ostringstream& ss) const
    {
        if (this->isAtom())
        {
            ss<<this->m_lexeme.token();
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
            ss << this->m_lexeme.token();
        }
        ss<<this->m_lexeme.token();
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
               && lhs.m_lexeme.token() == rhs.m_lexeme.token();
        return response;
    }

    bool operator!=(const Node& lhs, const Node& rhs)
    {
        return !(lhs == rhs);
    }
}
