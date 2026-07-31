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

    void Node::spawn_new_child()
    {
        this->children.emplace_back(std::make_shared<Node>(NodeType::Blank, ""));
    }

    Node::Node(NodeType _type, std::string _text):
            type(_type),
            text(std::move(_text)),
            uuid(CppCommon::UUID::Random()){}

    Node::Node(const Node& n)
    {
        this->children = n.children;
        this->type = n.type;
        this->text = n.text;
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
