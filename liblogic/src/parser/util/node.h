#pragma once
#include <memory>
#include <string>
#include <vector>
#include <system/uuid.h>
#include "dictionary.h"
#include "lexeme.hpp"

namespace eloquent::logic {
    class Node;
    using NodePtr = std::unique_ptr<Node>;
    using NodeObsPtr = Node*;
    enum class NodeType {
        Atom,
        NotOp,
        AndOp,
        OrOp,
        ImpliesOp,
        IffOp,
        LEquiOp,
        Blank
    };
    class Node {
        NodeType type = NodeType::Blank;
        lexeme m_lexeme;
        std::vector<NodePtr> children;
        Node* parent = nullptr;
        CppCommon::UUID uuid;
    public:
        [[nodiscard]] bool isRoot() const noexcept;
        [[nodiscard]] constexpr bool isAtom() const noexcept;
        [[nodiscard]] constexpr bool isBlank() const noexcept;
        [[nodiscard]] std::vector<NodePtr>& getChildren() noexcept;
        [[nodiscard]] NodeType getType() const noexcept;
        [[nodiscard]] NodeObsPtr getParent() const noexcept;
        [[nodiscard]] lexeme getLexeme() const noexcept;
        [[nodiscard]] CppCommon::UUID getUUID() const noexcept;
        void spawn_new_child(const lexeme& l);
        void set_node_type(const lexeme& l);

        /**
         * Creates a new node with given parameters
         * @param _type Type of node
         * @param _text Label of node
         */
        explicit Node(const lexeme& l);
        Node(const Node& n) = delete;
        void set_lexeme(const lexeme& l);
        friend bool operator==(const Node &lhs, const Node &rhs) ;
        friend bool operator!=(const Node &lhs, const Node &rhs) ;
        void adopt(const NodePtr& node);
        std::string to_string() const;

        void set_parent(const NodeObsPtr& parent)
        {
            this->parent = parent;
        }

        static std::unique_ptr<Node> make_node(const lexeme& l);
    private:
        std::ostringstream& to_string_impl(std::ostringstream& ss) const;
    };



};