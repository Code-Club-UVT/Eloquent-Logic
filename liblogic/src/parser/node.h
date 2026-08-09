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
        [[nodiscard]] constexpr bool isAtom() const noexcept {
            return type == NodeType::Atom;
        }
        [[nodiscard]] constexpr bool isBlank() const noexcept{
            return type == NodeType::Blank;
        }
        [[nodiscard]] NodeType getType() const noexcept;
        [[nodiscard]] NodeObsPtr getParent() const noexcept;
        [[nodiscard]] lexeme getLexeme() const noexcept;
        [[nodiscard]] CppCommon::UUID getUUID() const noexcept;
        [[nodiscard]] size_t num_children() const noexcept;
        [[nodiscard]] size_t arity() const noexcept;
        [[nodiscard]] NodeObsPtr childAt(size_t idx) const;
        [[nodiscard]] bool allChildrenAreWritten() const noexcept;
        [[nodiscard]] size_t find_by_uuid(CppCommon::UUID id) const noexcept;
        [[nodiscard]] NodePtr disconnect(size_t idx);
        void spawn_new_child(const lexeme& l);
        void set_node_type(const lexeme& l);
        void walk(const std::function<void(NodeObsPtr)>& cb);
        void traverse_children(const std::function<void(const NodeObsPtr&)>& cb) const;
        void condense(); //applies n-isation
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
        void adopt(NodePtr node);
        void replace_child(size_t idx, NodePtr new_child);
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