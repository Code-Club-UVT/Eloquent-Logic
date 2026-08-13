#ifndef TREE_VIEWER_H
#define TREE_VIEWER_H

#include "visualnode.h"
#include <QGraphicsView>
#include <QGraphicsScene>

class TreeViewer : public QGraphicsView {
    Q_OBJECT
public:
    explicit TreeViewer(QWidget *parent = nullptr) : QGraphicsView(parent) {
        scene = new QGraphicsScene(this);
        setScene(scene);
        setRenderHint(QPainter::Antialiasing);
        setDragMode(QGraphicsView::ScrollHandDrag); // Permite mutarea cu mouse-ul
    }

    void addNode(VisualNode* node) {
        scene->addItem(node);
    }

    // Adaugă asta în TreeViewer.h, la public:
    void addEdge(const QPointF& start, const QPointF& end) {
        // Desenăm o linie neagră de grosime 2
        QGraphicsLineItem* line = scene->addLine(QLineF(start, end), QPen(Qt::black, 2));
        line->setZValue(-1); // O punem în spate (Z-index -1) ca să nu acopere nodurile!
    }
    QGraphicsScene* getScene() const { return scene; }

private:
    QGraphicsScene *scene;
};

#endif // TREE_VIEWER_H