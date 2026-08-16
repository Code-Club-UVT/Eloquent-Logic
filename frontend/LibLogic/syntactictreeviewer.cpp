#include "SyntacticTreeViewer.h"
#include "node.h"
#include "edge.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWheelEvent>
#include <QPropertyAnimation>
#include <QEasingCurve>

SyntacticTreeViewer::SyntacticTreeViewer(QWidget *parent) : QWidget(parent), m_currentStep(0)
{
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);

    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view->setResizeAnchor(QGraphicsView::AnchorViewCenter);

    view->viewport()->installEventFilter(this);

    btnBack = new QPushButton("← Back", this);
    btnNext = new QPushButton("Next →", this);
    lblStep = new QLabel("Pas 0 / 0", this);
    lblStep->setAlignment(Qt::AlignCenter);
    btnPlay = new QPushButton("▶ Play", this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *controlsLayout = new QHBoxLayout();

    controlsLayout->addWidget(btnBack);
    controlsLayout->addWidget(btnPlay);
    controlsLayout->addWidget(lblStep);
    controlsLayout->addWidget(btnNext);

    playTimer = new QTimer(this);
    playTimer->setInterval(600);

    connect(playTimer, &QTimer::timeout, [this]() {
        if (m_currentStep < m_history.size()) {
            goToStep(m_currentStep + 1);
        } else {
            playTimer->stop();
            btnPlay->setText("▶ Play");
        }
    });

    connect(btnPlay, &QPushButton::clicked, [this]() {
        if (playTimer->isActive()) {
            playTimer->stop();
            btnPlay->setText("▶ Play");
        } else {
            if (m_currentStep == m_history.size()) goToStep(0);
            playTimer->start();
            btnPlay->setText("⏸ Pause");
        }
    });

    mainLayout->addWidget(view);
    mainLayout->addLayout(controlsLayout);

    connect(btnBack, &QPushButton::clicked, [this]() { goToStep(m_currentStep - 1); });
    connect(btnNext, &QPushButton::clicked, [this]() { goToStep(m_currentStep + 1); });
}

void SyntacticTreeViewer::loadHistory(const QJsonArray &events)
{
    m_history = events;

    m_currentStep = m_history.size();
    rebuildMapAndDraw();

    QRectF fullTreeRect = scene->itemsBoundingRect().adjusted(-50, -50, 50, 50);
    scene->setSceneRect(fullTreeRect);

    lblStep->setText(QString("Pas %1 / %2").arg(m_currentStep).arg(m_history.size()));
    btnBack->setEnabled(true);
    btnNext->setEnabled(false);
}

void SyntacticTreeViewer::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    if (scene && !scene->sceneRect().isEmpty()) {
        view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }
}

void SyntacticTreeViewer::clearTree()
{
    scene->clear();
}

Node* SyntacticTreeViewer::createNodeRecursive(const QString &nodeId, const QMap<QString, QJsonObject> &nodeMap, const QUuid &parentId, int depth, int &xOffset, const QString &animatedNodeId){
    if (!nodeMap.contains(nodeId)) return nullptr;

    QJsonObject obj = nodeMap[nodeId];

    QUuid id = QUuid(nodeId);
    QJsonObject lexeme = obj["lexeme"].toObject();
    QString token = lexeme["token"].toString();
    QString typeStr = obj["type"].toString();

    NodeType nType = NodeType::Operator;
    if (typeStr == "Atom") nType = NodeType::Variable;
    else if (typeStr == "Tautology" || typeStr == "Contradiction") nType = NodeType::Constant;

    if (token == "\\wedge") token = "∧";
    else if (token == "\\vee") token = "∨";
    else if (token == "\\implies") token = "→";
    else if (token == "\\iff") token = "↔";
    else if (token == "\\neg") token = "¬";
    else if (token == "\\top") token = "⊤";
    else if (token == "\\bot") token = "⊥";

    QJsonArray childrenArr = obj["children"].toArray();
    QList<QUuid> childrenIds;
    for (const QJsonValue &val : childrenArr) {
        QString childId = val.isString() ? val.toString() : val.toObject()["value"].toString();
        childrenIds.append(QUuid(childId));
    }

    Node *node = new Node(nullptr, id, token, nType, parentId, childrenIds);
    scene->addItem(node);

    if (childrenArr.isEmpty()) {
        node->setPos(xOffset, depth * 80 - 150);
        xOffset += 80;
    } else {
        int firstChildX = -1;
        int lastChildX = -1;

        for (const QJsonValue &val : childrenArr) {
            QString childId = val.isString() ? val.toString() : val.toObject()["value"].toString();
            Node *childNode = createNodeRecursive(childId, nodeMap, id, depth + 1, xOffset);
            if (childNode) {
                scene->addItem(new Edge(node, childNode));
                if (firstChildX == -1) firstChildX = childNode->x();
                lastChildX = childNode->x();
            }
        }

        int currentX = firstChildX != -1 ? (firstChildX + lastChildX) / 2 : xOffset;
        node->setPos(currentX, depth * 80 - 150);
    }

    if (nodeId == animatedNodeId) {
        QPropertyAnimation *anim = new QPropertyAnimation(node, "scale", node);
        anim->setDuration(500);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);

        anim->setEasingCurve(QEasingCurve::OutBack);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }

    return node;
}

void SyntacticTreeViewer::goToStep(int step)
{
    if (step < 0 || step > m_history.size()) return;

    m_currentStep = step;
    lblStep->setText(QString("Pas %1 / %2").arg(m_currentStep).arg(m_history.size()));

    btnBack->setEnabled(m_currentStep > 0);
    btnNext->setEnabled(m_currentStep < m_history.size());

    rebuildMapAndDraw();
}

void SyntacticTreeViewer::rebuildMapAndDraw()
{
    clearTree();
    QMap<QString, QJsonObject> currentNodeMap;

    QString animatedNodeId = "";
    if (m_currentStep > 0 && m_currentStep <= m_history.size()) {
        QJsonObject lastEvent = m_history[m_currentStep - 1].toObject();
        QString method = lastEvent["method"].toString();
        QJsonValue idVal;

        if (method == "parser/didMakeNewSubtree") {
            idVal = lastEvent["params"].toObject()["node"].toObject()["id"];
        } else if (method == "parser/didJoin") {
            idVal = lastEvent["params"].toObject()["target"].toObject()["id"];
        }
        animatedNodeId = idVal.isString() ? idVal.toString() : idVal.toObject()["value"].toString();
    }

    for (int i = 0; i < m_currentStep; ++i) {
        QJsonObject ev = m_history[i].toObject();
        QString method = ev["method"].toString();
        QJsonObject params = ev["params"].toObject();

        if (method == "parser/didMakeNewSubtree") {
            QJsonObject node = params["node"].toObject();
            QString idStr = node["id"].isString() ? node["id"].toString() : node["id"].toObject()["value"].toString();
            currentNodeMap[idStr] = node;
        } else if (method == "parser/didJoin") {
            QJsonObject target = params["target"].toObject();
            QString idStr = target["id"].isString() ? target["id"].toString() : target["id"].toObject()["value"].toString();
            currentNodeMap[idStr] = target;
        }
    }

    QList<QString> rootIds = currentNodeMap.keys();
    for (const QJsonObject &node : currentNodeMap) {
        QJsonArray children = node["children"].toArray();
        for (const QJsonValue &child : children) {
            QString childId = child.isString() ? child.toString() : child.toObject()["value"].toString();
            rootIds.removeAll(childId);
        }
    }

    int xOffset = 0;
    for (const QString &rootId : rootIds) {
        createNodeRecursive(rootId, currentNodeMap, QUuid(), 0, xOffset, animatedNodeId);
        xOffset += 100;
    }
}

bool SyntacticTreeViewer::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == view->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);

        if (wheelEvent->angleDelta().y() > 0) {
            view->scale(1.1, 1.1);
        } else {
            view->scale(0.9, 0.9);
        }

        return true;
    }

    return QWidget::eventFilter(obj, event);
}