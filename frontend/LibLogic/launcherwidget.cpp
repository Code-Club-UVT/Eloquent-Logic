#include "LauncherWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QKeySequence>

#include "TruthTableViewer.h"
#include "syntactictreeviewer.h"

LauncherWidget::LauncherWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *btnOpenTable = new QPushButton("Truth Table", this);
    QPushButton *btnOpenTree = new QPushButton("Syntactic Tree", this);

    QFont btnFont = btnOpenTable->font();
    btnFont.setPointSize(11);
    btnOpenTable->setFont(btnFont);
    btnOpenTree->setFont(btnFont);

    buttonLayout->addWidget(btnOpenTable);
    buttonLayout->addWidget(btnOpenTree);

    mainLayout->addLayout(buttonLayout);

    connect(btnOpenTable, &QPushButton::clicked, [this]() {
        TruthTableViewer *tableWindow = new TruthTableViewer({"A", "B", "A ∧ B"});

        tableWindow->setWindowTitle("Truth Table");
        tableWindow->resize(450, 300);

        tableWindow->setAttribute(Qt::WA_DeleteOnClose);

        tableWindow->addRow({true, true, true});
        tableWindow->addRow({true, false, false});
        tableWindow->addRow({false, true, false});
        tableWindow->addRow({false, false, false});

        tableWindow->show();
    });

    connect(btnOpenTree, &QPushButton::clicked, [this]() {
        SyntacticTreeViewer *treeWindow = new SyntacticTreeViewer();
        treeWindow->setWindowTitle("Syntactic Tree");
        treeWindow->resize(600, 500);

        treeWindow->setAttribute(Qt::WA_DeleteOnClose);

        treeWindow->buildDummyTree();

        treeWindow->show();
    });
}