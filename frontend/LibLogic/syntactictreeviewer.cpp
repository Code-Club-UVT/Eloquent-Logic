#include "SyntacticTreeViewer.h"
#include "node.h"
#include "edge.h"
#include <QUuid>
#include <QVBoxLayout>
#include <QHBoxLayout>

SyntacticTreeViewer::SyntacticTreeViewer(QWidget *parent)
    : QWidget(parent), currentStepIndex(-1)
{
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-400, -400, 800, 800);

    view = new QGraphicsView(scene);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setRenderHint(QPainter::Antialiasing);

    btnPrev = new QPushButton("BACK", this);
    btnNext = new QPushButton("NEXT", this);
    lblStatus = new QLabel("STATE: ...", this);
    lblStatus->setAlignment(Qt::AlignCenter);

    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->addWidget(btnPrev);
    controlLayout->addWidget(lblStatus, 1);
    controlLayout->addWidget(btnNext);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(view);
    mainLayout->addLayout(controlLayout);

    connect(btnPrev, &QPushButton::clicked, this, &SyntacticTreeViewer::prevStep);
    connect(btnNext, &QPushButton::clicked, this, &SyntacticTreeViewer::nextStep);

    updateUI();
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

    nodeMap.insert(idAnd, nodeAnd);
    nodeMap.insert(idP, nodeP);
    nodeMap.insert(idQ, nodeQ);

    nodeAnd->setPos(0, -100);
    nodeP->setPos(-100, 50);
    nodeQ->setPos(100, 50);

    scene->addItem(new Edge(nodeAnd, nodeP));
    scene->addItem(new Edge(nodeAnd, nodeQ));

    recordStep(idP, QColor(255, 223, 0), "Evaluăm variabila A...");
    recordStep(idQ, QColor(255, 223, 0), "Evaluăm variabila B...");
    recordStep(idAnd, QColor(144, 238, 144), "Rezultatul este T!");
}

void SyntacticTreeViewer::recordStep(const QUuid& nodeId, const QColor& newColor, const QString& description)
{
    if (!nodeMap.contains(nodeId)) return;

    TreeStep step;
    step.nodeId = nodeId;
    step.oldColor = nodeMap[nodeId]->highlightColor();
    step.newColor = newColor;
    step.description = description;

    history.append(step);
    updateUI();
}

void SyntacticTreeViewer::nextStep()
{
    if (currentStepIndex < history.size() - 1) {
        currentStepIndex++;
        const TreeStep& step = history[currentStepIndex];

        if (nodeMap.contains(step.nodeId)) {
            nodeMap[step.nodeId]->animateHighlight(step.newColor);
        }

        lblStatus->setText(step.description);
        updateUI();
    }
}

void SyntacticTreeViewer::prevStep()
{
    if (currentStepIndex >= 0) {
        const TreeStep& step = history[currentStepIndex];

        if (nodeMap.contains(step.nodeId)) {
            nodeMap[step.nodeId]->animateHighlight(step.oldColor);
        }

        currentStepIndex--;

        if (currentStepIndex >= 0) {
            lblStatus->setText(history[currentStepIndex].description);
        } else {
            lblStatus->setText("STATE: ...");
        }
        updateUI();
    }
}

void SyntacticTreeViewer::updateUI()
{
    btnPrev->setEnabled(currentStepIndex >= 0);
    btnNext->setEnabled(currentStepIndex < history.size() - 1);
}