#ifndef LOGIC_NODE_H
#define LOGIC_NODE_H

#include <QUuid>
#include <QString>
#include <QList>

enum class NodeType {
    Operator,       // AND, OR, IMPLIES, NOT
    Variable,       // A, B, C
    Constant,       // True, False
    Root            // (Opțional) Nodul de bază
};

struct LogicNode {
    QUuid id;
    QString text;
    NodeType type;
    QUuid parent;          // Null dacă e root
    QList<QUuid> children;
};

#endif // LOGIC_NODE_H