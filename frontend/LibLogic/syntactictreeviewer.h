#ifndef SYNTACTICTREEVIEWER_H
#define SYNTACTICTREEVIEWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>
#include <QMap>
#include <QTimer>

class Node;
class QPushButton;
class QLabel;

class SyntacticTreeViewer : public QWidget
{
    Q_OBJECT
    QGraphicsView *view;
    QGraphicsScene *scene;

    QPushButton *btnBack;
    QPushButton *btnNext;
    QPushButton *btnPlay;
    QLabel *lblStep;
    QTimer *playTimer;

    QJsonArray m_history;
    int m_currentStep;

public:
    SyntacticTreeViewer(QWidget *parent = nullptr);
    void loadHistory(const QJsonArray &events);
    void clearTree();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void goToStep(int step);

private:
    void rebuildMapAndDraw();
    Node* createNodeRecursive(const QString &nodeId, const QMap<QString, QJsonObject> &nodeMap, const QUuid &parentId, int depth, int &xOffset, const QString &animatedNodeId = "");
};

#endif