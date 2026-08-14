#ifndef VISUAL_NODE_H
#define VISUAL_NODE_H

#include <QGraphicsObject>
#include <QPainter>
#include <QColor>
#include "LogicNode.h"

class VisualNode : public QGraphicsObject {
    Q_OBJECT
    Q_PROPERTY(QColor color READ getColor WRITE setColor)

public:
    VisualNode(const LogicNode& data, QGraphicsItem *parent = nullptr);

    QColor getColor() const { return m_color; }
    void setColor(const QColor& color) {
        m_color = color;
        update();
    }

    void highlight(const QColor& highlightColor, int durationMs = 500);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    LogicNode m_data;
    QColor m_color;
    int m_radius = 25;
};

#endif