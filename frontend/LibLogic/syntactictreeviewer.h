#ifndef SYNTACTICTREEVIEWER_H
#define SYNTACTICTREEVIEWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include "TreeStep.h"

class Node;

class SyntacticTreeViewer : public QWidget
{
    Q_OBJECT
    QGraphicsScene *scene;
    QGraphicsView *view;

    QPushButton *btnPrev;
    QPushButton *btnNext;
    QLabel *lblStatus;

    QMap<QUuid, Node*> nodeMap;
    QList<TreeStep> history;
    int currentStepIndex;

private slots:
    void nextStep();
    void prevStep();
    void updateUI();

public:
    SyntacticTreeViewer(QWidget *parent = nullptr);
    void buildDummyTree();
    void recordStep(const QUuid& nodeId, const QColor& newColor, const QString& description = "");
};

#endif