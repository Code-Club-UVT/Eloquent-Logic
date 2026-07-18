#include "LogicEditorWindow.h"
#include "logiclineedit.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QKeySequence>

LogicEditorWindow::LogicEditorWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    formulaInput = new LogicLineEdit(this);
    formulaInput->setPlaceholderText("Introdu formula aici...");
    QFont font = formulaInput->font();
    font.setPointSize(14);
    formulaInput->setFont(font);

    QPushButton *btnAnd = new QPushButton("∧", this);
    QPushButton *btnOr = new QPushButton("∨", this);
    QPushButton *btnImplies = new QPushButton("→", this);
    QPushButton *btnEquiv = new QPushButton("↔", this);
    QPushButton *btnNot = new QPushButton("¬", this);
    QPushButton *btnEntailment  = new QPushButton("⊨", this);
    QPushButton *btnNor = new QPushButton("|", this);
    QPushButton *btnNand = new QPushButton("∇", this);

    btnAnd->setShortcut(QKeySequence("Alt+A"));
    btnOr->setShortcut(QKeySequence("Alt+O"));
    btnImplies->setShortcut(QKeySequence("Alt+I"));
    btnEquiv->setShortcut(QKeySequence("Alt+E"));
    btnNot->setShortcut(QKeySequence("Alt+N"));
    btnEntailment->setShortcut(QKeySequence("Alt+T"));
    btnNor->setShortcut(QKeySequence("Alt+R"));
    btnNand->setShortcut(QKeySequence("Alt+D"));

    buttonLayout->addWidget(btnAnd);
    buttonLayout->addWidget(btnOr);
    buttonLayout->addWidget(btnImplies);
    buttonLayout->addWidget(btnEquiv);
    buttonLayout->addWidget(btnNot);
    buttonLayout->addWidget(btnEntailment);
    buttonLayout->addWidget(btnNor);
    buttonLayout->addWidget(btnNand);

    mainLayout->addWidget(formulaInput);
    mainLayout->addLayout(buttonLayout);

    connect(btnAnd, &QPushButton::clicked, [this]() {
        formulaInput->insert("and");
        formulaInput->setFocus();
    });
    connect(btnOr, &QPushButton::clicked, [this]() {
        formulaInput->insert("or");
        formulaInput->setFocus();
    });
    connect(btnImplies, &QPushButton::clicked, [this]() {
        formulaInput->insert("impl");
        formulaInput->setFocus();
    });
    connect(btnEquiv, &QPushButton::clicked, [this]() {
        formulaInput->insert("eq");
        formulaInput->setFocus();
    });
    connect(btnNot, &QPushButton::clicked, [this]() {
        formulaInput->insert("not");
        formulaInput->setFocus();
    });
    connect(btnEntailment, &QPushButton::clicked, [this]() {
        formulaInput->insert("ent");
        formulaInput->setFocus();
    });
    connect(btnNor, &QPushButton::clicked, [this]() {
        formulaInput->insert("nor");
        formulaInput->setFocus();
    });
    connect(btnNand, &QPushButton::clicked, [this]() {
        formulaInput->insert("nand");
        formulaInput->setFocus();
    });
}

QString LogicEditorWindow::getFormula() const {
    return formulaInput->text();
}