//
// Created by thonkdifferent on 11.12.24.
//
#include <liblogic/private/tree_builder.h>

namespace eloquent::logic {

    NodePtr NodeBuilder::makeNewAtomicNode(const std::string& text) {
        return std::make_shared<Node>(NodeType::Atom,text);
    }
    NodePtr NodeBuilder::makeNewNotNode(){
        return std::make_shared<Node>(NodeType::NotOp,ELOQUENT_LIBLOGIC_SYMB_NOT);
    }
    NodePtr NodeBuilder::makeNewAndNode() {
        return std::make_shared<Node>(NodeType::AndOp, ELOQUENT_LIBLOGIC_SYMB_AND);
    }
    NodePtr NodeBuilder::makeNewOrNode() {
        return std::make_shared<Node>(NodeType::OrOp,ELOQUENT_LIBLOGIC_SYMB_OR);
    }
    NodePtr NodeBuilder::makeNewImpliesNode() {
        return std::make_shared<Node>(NodeType::ImpliesOp,ELOQUENT_LIBLOGIC_SYMB_IMPL);
    }
    NodePtr NodeBuilder::makeNewIffNode() {
        return std::make_shared<Node>(NodeType::IffOp,ELOQUENT_LIBLOGIC_SYMB_IFF);
    }
    //------------------------
    NodePtr NodeBuilder::makeNewTautologyNode() {
        return std::make_shared<Node>(NodeType::Atom, ELOQUENT_LIBLOGIC_SYMB_TAUTOLOGY);
    }
    NodePtr NodeBuilder::makeNewContradictionNode() {
        return std::make_shared<Node>(NodeType::Atom, ELOQUENT_LIBLOGIC_SYMB_CONTRADICTION);
    }
    //------------------------
    NodePtr NodeBuilder::makeNewBlankNode() {
        return std::make_shared<Node>(NodeType::Blank);
    }

    void Node::spawn_new_child() {
        children.push_back(NodeBuilder::makeNewBlankNode());
        children[children.size()-1]->index = children.size()-1;
    }

};