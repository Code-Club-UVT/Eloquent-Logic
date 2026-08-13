#ifndef SYNTACTICTREEVIEWER_H
#define SYNTACTICTREEVIEWER_H

#include <QGraphicsView>
#include <QGraphicsScene>

class SyntacticTreeViewer : public QGraphicsView
{
    Q_OBJECT
    QGraphicsScene *scene;

public:
    SyntacticTreeViewer(QWidget *parent = nullptr);
    void buildDummyTree();
};

#endif