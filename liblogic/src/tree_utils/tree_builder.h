#pragma once
#include <memory>
#include <string>
#include <vector>
#include "dictionary.h"
namespace eloquent::logic {
    struct Node;
    struct NodeBuilder;
    using NodePtr = std::shared_ptr<Node>;
    using NodeObsPtr = std::weak_ptr<Node>;
    enum class NodeType {
        Atom = 0,
        NotOp,
        AndOp,
        OrOp,
        ImpliesOp,
        IffOp,
        Blank
    };

    struct NodeBuilder {
        static NodePtr makeNewAtomicNode(const std::string& text);
        static NodePtr makeNewNotNode();
        static NodePtr makeNewAndNode();
        static NodePtr makeNewOrNode();
        static NodePtr makeNewImpliesNode();
        static NodePtr makeNewIffNode();
        //------------------------
        static NodePtr makeNewTautologyNode();
        static NodePtr makeNewContradictionNode();
        //------------------------
        static NodePtr makeNewBlankNode();
    };
    struct Node {
        NodeType type = NodeType::Blank;
        std::string text;
        size_t index = 0;
        std::vector<NodePtr> children;
        NodeObsPtr parent;
        [[nodiscard]] bool isRoot() const noexcept {
            return parent.expired();
        }
        [[nodiscard]] constexpr bool isAtom() const noexcept {return type==NodeType::Atom;}
        [[nodiscard]] constexpr bool isBlank() const noexcept {return type==NodeType::Blank;}

        void spawn_new_child();

        Node(Node const& n) = delete;

        /**
         * Creates a new node with given parameters
         * @param _type Type of node
         * @param _text Label of node
         */
        explicit Node(NodeType _type, std::string _text=""):   type(_type), text(std::move(_text)){}

        /**
         * Writes the information regarding the given token in node form's type and properties to the current node
         * @param n Node to copy from
         */
        void copy_from(const NodePtr& n) {
            this->type = n->type;
            this->text = n->text;
        }

        /*
         * If node math doesn't check out during debugging or if you're attempting to optimize,
         * this function is a good place to start.
        */
        void copy_children(const NodePtr& n) {
            this->children = n->children;
        }

        friend bool operator==(const Node &lhs, const Node &rhs) {
            if (lhs.children.size() != rhs.children.size()) {
                return false;
            }
            bool response =  lhs.type == rhs.type
                   && lhs.text == rhs.text;
            for (size_t i = 0; i < lhs.children.size(); i++) {
                response &= *lhs.children[i] == *rhs.children[i];
            }
            return response;
        }

        friend bool operator!=(const Node &lhs, const Node &rhs) {
            return !(lhs == rhs);
        }

        friend struct NodeBuilder;
    };




};