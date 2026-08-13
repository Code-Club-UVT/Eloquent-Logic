#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>
#include <QString>
#include <QUuid>

class Edge;
class GraphWidget;

enum class NodeType {
    Operator,
    Variable,
    Constant
};

class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget,
         const QUuid &id,
         const QString &text,
         NodeType type,
         const QUuid &parent,
         const QList<QUuid> &children);

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    void calculateForces();
    bool advancePosition();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QUuid id() const { return m_id; }
    QString text() const { return m_text; }
    NodeType logicalType() const { return m_type; }
    QUuid parentId() const { return m_parent; }
    QList<QUuid> childrenIds() const { return m_children; }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;

    QUuid m_id;
    QString m_text;
    NodeType m_type;
    QUuid m_parent;
    QList<QUuid> m_children;
};

#endif // NODE_H