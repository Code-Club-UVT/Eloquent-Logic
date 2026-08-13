#include "SyntacticTreeViewer.h"
#include "node.h"
#include "edge.h"
#include <QUuid>

SyntacticTreeViewer::SyntacticTreeViewer(QWidget *parent)
    : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    scene->setSceneRect(-400, -400, 800, 800);

    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
}

void SyntacticTreeViewer::buildDummyTree()
{
    QUuid idAnd = QUuid::createUuid();
    QUuid idP = QUuid::createUuid();
    QUuid idQ = QUuid::createUuid();
    Node *nodeAnd = new Node(nullptr, idAnd, "∧", NodeType::Operator, QUuid(), {idP, idQ});
    Node *nodeP = new Node(nullptr, idP, "A", NodeType::Variable, idAnd, {});
    Node *nodeQ = new Node(nullptr, idQ, "B", NodeType::Variable, idAnd, {});

    scene->addItem(nodeAnd);
    scene->addItem(nodeP);
    scene->addItem(nodeQ);

    nodeAnd->setPos(0, -100);
    nodeP->setPos(-100, 50);
    nodeQ->setPos(100, 50);

    scene->addItem(new Edge(nodeAnd, nodeP));
    scene->addItem(new Edge(nodeAnd, nodeQ));
}