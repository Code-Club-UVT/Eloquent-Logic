#pragma once
#include <memory>
#include <string>
#include <vector>
#include <system/uuid.h>
#include "dictionary.h"
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
        std::string text;
        size_t index = 0;
        std::vector<NodePtr> children;
        NodeObsPtr parent;
        CppCommon::UUID uuid;
    public:
        [[nodiscard]] bool isRoot() const noexcept;
        [[nodiscard]] constexpr bool isAtom() const noexcept;
        [[nodiscard]] constexpr bool isBlank() const noexcept;
        [[nodiscard]] std::vector<NodePtr>& getChildren() noexcept;
        void spawn_new_child();

        /**
         * Creates a new node with given parameters
         * @param _type Type of node
         * @param _text Label of node
         */
        explicit Node(NodeType _type, std::string _text="");
        Node(const Node& n);
        friend bool operator==(const Node &lhs, const Node &rhs) ;
        friend bool operator!=(const Node &lhs, const Node &rhs) ;
        std::string to_string();

    private:
        std::ostringstream& to_string_impl(std::ostringstream& ss);
    };



};