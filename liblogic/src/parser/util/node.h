#pragma once
#include <memory>
#include <string>
#include <vector>
#include <system/uuid.h>
#include "dictionary.h"
#include "lexeme.hpp"

namespace eloquent::logic {
    struct Node;
    struct NodeBuilder;
    using NodePtr = std::shared_ptr<Node>;
    using NodeObsPtr = std::weak_ptr<Node>;
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
        NodeObsPtr parent;
        CppCommon::UUID uuid;
    public:
        [[nodiscard]] bool isRoot() const noexcept;
        [[nodiscard]] constexpr bool isAtom() const noexcept;
        [[nodiscard]] constexpr bool isBlank() const noexcept;
        [[nodiscard]] std::vector<NodePtr>& getChildren() noexcept;
        [[nodiscard]] NodeType getType() const noexcept;
        [[nodiscard]] std::string getText() const noexcept;
        [[nodiscard]] NodeObsPtr getParent() const noexcept;
        [[nodiscard]] lexeme getLexeme() const noexcept;
        [[nodiscard]] CppCommon::UUID getUUID() const noexcept;
        void spawn_new_child(const lexeme& l);

        /**
         * Creates a new node with given parameters
         * @param _type Type of node
         * @param _text Label of node
         */
        explicit Node(lexeme l);
        Node(const Node& n);
        friend bool operator==(const Node &lhs, const Node &rhs) ;
        friend bool operator!=(const Node &lhs, const Node &rhs) ;
        std::string to_string();
        static std::shared_ptr<Node> make_node(NodeType _type, const std::string& _text="");
    private:
        std::ostringstream& to_string_impl(std::ostringstream& ss);
    };



};