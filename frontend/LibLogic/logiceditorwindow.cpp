#include "LogicEditorWindow.h"
#include "logiclineedit.h"
#include "agentconnection.h"
#include "syntactictreeviewer.h"
#include "truthtableviewer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QKeySequence>
#include <QRegularExpression>
#include <QJsonArray>
#include <QJsonObject>

LogicEditorWindow::LogicEditorWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    formulaInput = new LogicLineEdit(this);
    formulaInput->setPlaceholderText("Introdu formula aici...");
    QFont font = formulaInput->font();
    font.setPointSize(14);
    formulaInput->setFont(font);
    m_agent = new AgentConnection(this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *btnAnd = new QPushButton("∧", this);
    QPushButton *btnOr = new QPushButton("∨", this);
    QPushButton *btnImplies = new QPushButton("→", this);
    QPushButton *btnEquiv = new QPushButton("↔", this);
    QPushButton *btnNot = new QPushButton("¬", this);
    QPushButton *btnEntailment  = new QPushButton("⊨", this);
    QPushButton *btnTautology = new QPushButton("⊤", this);
    QPushButton *btnContradiction = new QPushButton("⊥", this);

    btnAnd->setShortcut(QKeySequence("Alt+A"));
    btnOr->setShortcut(QKeySequence("Alt+O"));
    btnImplies->setShortcut(QKeySequence("Alt+I"));
    btnEquiv->setShortcut(QKeySequence("Alt+E"));
    btnNot->setShortcut(QKeySequence("Alt+N"));
    btnEntailment->setShortcut(QKeySequence("Alt+L"));
    btnTautology->setShortcut(QKeySequence("Alt+T"));
    btnContradiction->setShortcut(QKeySequence("Alt+C"));

    buttonLayout->addWidget(btnAnd);
    buttonLayout->addWidget(btnOr);
    buttonLayout->addWidget(btnImplies);
    buttonLayout->addWidget(btnEquiv);
    buttonLayout->addWidget(btnNot);
    buttonLayout->addWidget(btnEntailment);
    buttonLayout->addWidget(btnTautology);
    buttonLayout->addWidget(btnContradiction);

    QHBoxLayout *actionLayout = new QHBoxLayout();

    QPushButton *btnFNN = new QPushButton("FNN", this);
    QPushButton *btnFNC = new QPushButton("FNC", this);
    QPushButton *btnFND = new QPushButton("FND", this);
    QPushButton *btnTruthTable = new QPushButton("Tabel Adevăr", this);
    QPushButton *btnEval = new QPushButton("See Result (AST)", this);
    QPushButton *btnSat = new QPushButton("SAT", this);

    satAlgoSelector = new QComboBox(this);
    satAlgoSelector->setFont(font);
    satAlgoSelector->addItem("DPLL MCL", "dpll_mcl");
    satAlgoSelector->addItem("DPLL RAND", "dpll_rand");
    satAlgoSelector->addItem("DP", "dp");
    satAlgoSelector->addItem("Resolution", "resolution");

    btnFNN->setFont(font);
    btnFNC->setFont(font);
    btnFND->setFont(font);
    btnTruthTable->setFont(font);
    btnEval->setFont(font);
    btnSat->setFont(font);

    actionLayout->addWidget(satAlgoSelector);
    actionLayout->addWidget(btnSat);
    actionLayout->addWidget(btnFNN);
    actionLayout->addWidget(btnFNC);
    actionLayout->addWidget(btnFND);
    actionLayout->addWidget(btnTruthTable);
    actionLayout->addStretch();
    actionLayout->addWidget(btnEval);

    resultOutput = new QLineEdit(this);
    resultOutput->setPlaceholderText("The result will appear here");
    resultOutput->setFont(font);
    resultOutput->setReadOnly(true);

    logOutput = new QTextEdit(this);
    logOutput->setReadOnly(true);
    logOutput->setMaximumHeight(100);
    logOutput->setPlaceholderText("Notificările live (lexer/parser) vor apărea aici...");


    mainLayout->addWidget(formulaInput);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(actionLayout);
    mainLayout->addWidget(resultOutput);

    connect(btnAnd, &QPushButton::clicked, [this]() { formulaInput->insert(" ∧ "); formulaInput->setFocus(); });
    connect(btnOr, &QPushButton::clicked, [this]() { formulaInput->insert(" ∨ "); formulaInput->setFocus(); });
    connect(btnImplies, &QPushButton::clicked, [this]() { formulaInput->insert(" → "); formulaInput->setFocus(); });
    connect(btnEquiv, &QPushButton::clicked, [this]() { formulaInput->insert(" ↔ "); formulaInput->setFocus(); });
    connect(btnNot, &QPushButton::clicked, [this]() { formulaInput->insert(" ¬ "); formulaInput->setFocus(); });
    connect(btnEntailment, &QPushButton::clicked, [this]() { formulaInput->insert(" ⊨ "); formulaInput->setFocus(); });
    connect(btnTautology, &QPushButton::clicked, [this]() { formulaInput->insert(" ⊤ "); formulaInput->setFocus(); });
    connect(btnContradiction, &QPushButton::clicked, [this]() { formulaInput->insert(" ⊥ "); formulaInput->setFocus(); });
    connect(btnTruthTable, &QPushButton::clicked, [this]() {
        QString formula = getFormulaWithSymbols();

        QJsonObject params;
        params["formula"] = formula;

        resultOutput->setText("Se generează tabelul de adevăr...");
        m_agent->sendRequest("truth_table", params, 2);
    });

    connect(btnSat, &QPushButton::clicked, [this]() {
        QJsonObject params;
        params["formula"] = getFormulaWithSymbols();

        params["algorithm"] = satAlgoSelector->currentData().toString();

        resultOutput->setText("Se verifică satisfiabilitatea (" + satAlgoSelector->currentText() + ")...");
        logOutput->clear();

        m_agent->sendRequest("sat", params, 6);
    });

    connect(btnFNN, &QPushButton::clicked, [this]() {
        QJsonObject params;
        params["formula"] = getFormulaWithSymbols();
        params["target"] = "nnf";

        resultOutput->setText("Se calculează FNN...");
        m_agent->sendRequest("transform", params, 3);
    });

    connect(btnFNC, &QPushButton::clicked, [this]() {
        QJsonObject params;
        params["formula"] = getFormulaWithSymbols();
        params["target"] = "cnf";

        resultOutput->setText("Se calculează FNC...");
        m_agent->sendRequest("transform", params, 4);
    });

    connect(btnFND, &QPushButton::clicked, [this]() {
        QJsonObject params;
        params["formula"] = getFormulaWithSymbols();
        params["target"] = "dnf";

        resultOutput->setText("Se calculează FND...");
        m_agent->sendRequest("transform", params, 5);
    });

    connect(m_agent, &AgentConnection::responseReceived, this, &LogicEditorWindow::onAgentResponse);
    connect(m_agent, &AgentConnection::errorReceived, this, &LogicEditorWindow::onAgentError);
    connect(m_agent, &AgentConnection::notificationReceived, this, &LogicEditorWindow::onAgentNotification);

#ifdef Q_OS_WIN
    m_agent->startAgent("C:/Users/gvend/OneDrive/Documente/GitHub/Eloquent-Logic/liblogic/cmake-build-debug/logic_agent/logic_agent.exe");
#else
    m_agent->startAgent("./logic_agent");
#endif

    connect(btnEval, &QPushButton::clicked, [this]() {
        QString formula = getFormulaWithSymbols();

        QJsonObject params;
        params["formula"] = formula;

        resultOutput->setText("Se generează arborele...");
        logOutput->clear();

        m_agent->sendRequest("parse", params, 1);
    });
    mainLayout->addWidget(logOutput);
}

LogicEditorWindow::~LogicEditorWindow() {}

QString LogicEditorWindow::getFormulaWithSymbols() const {
    QString formula = formulaInput->text();
    using namespace eloquent::liblogic::symbols;

    formula.replace("∧", SYMB_AND);
    formula.replace("∨", SYMB_OR);
    formula.replace("→", SYMB_IMPL);
    formula.replace("↔", SYMB_IFF);
    formula.replace("¬", SYMB_NOT);
    formula.replace("⊨", SYMB_LEQUI);
    formula.replace("⊤", SYMB_TAUTOLOGY);
    formula.replace("⊥", SYMB_CONTRADICTION);

    return formula;
}

QString LogicEditorWindow::getFormula() const {
    return formulaInput->text();
}

void LogicEditorWindow::onAgentResponse(int id, const QJsonObject &result) {
    if (id == 1) {
        if (result.contains("tree")) {
            QJsonObject tree = result["tree"].toObject();
            QString rootId = tree["id"].isString() ? tree["id"].toString() : tree["id"].toObject()["value"].toString();
            m_astNodes[rootId] = tree;
            qDebug().noquote() << QJsonDocument(tree).toJson(QJsonDocument::Indented);
            resultOutput->setText("Succes! Arborele a fost generat.");

            SyntacticTreeViewer *treeWindow = new SyntacticTreeViewer();
            treeWindow->setWindowTitle("Syntactic Tree - Rezultat");
            treeWindow->resize(600, 500);
            treeWindow->setAttribute(Qt::WA_DeleteOnClose);

            treeWindow->loadHistory(m_parserEvents);

            treeWindow->show();
        } else {
            resultOutput->setText("Eroare: Răspunsul nu conține un arbore valid.");
        }
    } else if (id == 2) {
        resultOutput->setText("Succes! Tabelul a fost generat.");

        QStringList headers;
        if (result.contains("headers")) {
            QJsonArray headersArr = result["headers"].toArray();
            for (const QJsonValue &h : headersArr) {
                QString headerText = h.toString();
                headerText.replace("\\wedge", "∧").replace("\\vee", "∨")
                    .replace("\\implies", "→").replace("\\iff", "↔")
                    .replace("\\neg", "¬");
                headers.append(headerText);
            }
        }

        TruthTableViewer *tableWindow = new TruthTableViewer(headers);
        tableWindow->setWindowTitle("Tabel de Adevăr");
        tableWindow->resize(500, 400);
        tableWindow->setAttribute(Qt::WA_DeleteOnClose);

        if (result.contains("rows")) {
            QJsonArray rowsArr = result["rows"].toArray();
            for (const QJsonValue &rowVal : rowsArr) {
                QJsonArray rowArr = rowVal.toArray();
                QList<bool> rowData;
                for (const QJsonValue &cell : rowArr) {
                    rowData.append(cell.toBool());
                }
                tableWindow->addRow(rowData);
            }
        }

        tableWindow->show();
    } else if (id >= 3 && id <= 5) {
        if (result.contains("tree")) {
            QJsonObject tree = result["tree"].toObject();
            QString rootId = tree["id"].toObject()["value"].toString();

            m_astNodes[rootId] = tree;

            QString newFormula = astToFormula(rootId);

            resultOutput->setText("Succes: " + newFormula);

            formulaInput->setText(newFormula);

        } else {
            resultOutput->setText("Răspuns primit, dar nu conține un arbore.");
            qDebug() << "Răspuns transform necunoscut:" << result;
        }
    } else if (id == 6) {
        bool isSat = false;

        if (result.contains("satisfiable")) {
            isSat = result["satisfiable"].toBool();
        } else if (result.contains("result")) {
            isSat = result["result"].toBool();
        }

        if (isSat) {
            resultOutput->setText("Rezultat SAT: Formula este SATISFIABILĂ.");
            resultOutput->setStyleSheet("color: green; font-weight: bold;");
        } else {
            resultOutput->setText("Rezultat SAT: Formula este NESATISFIABILĂ (Contradicție).");
            resultOutput->setStyleSheet("color: red; font-weight: bold;");
        }

        connect(formulaInput, &QLineEdit::textEdited, [this]() {
            resultOutput->setStyleSheet("");
        });
    }
}

void LogicEditorWindow::onAgentError(int id, const QJsonObject &error) {
    QString errorMsg = error["message"].toString();
    resultOutput->setText("Eroare: " + errorMsg);
}

void LogicEditorWindow::onAgentNotification(const QString &method, const QJsonObject &params) {
    QJsonDocument doc(params);
    QString paramsStr = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));

    if (logOutput) {
        logOutput->append(QString("<b>%1</b>: %2").arg(method, paramsStr));
    }

    if (method == "parser/didStart") {
        m_astNodes.clear();
        m_parserEvents = QJsonArray();
    }

    auto extractNode = [this](const QJsonObject &obj) {
        if (obj.contains("id")) {
            QString idStr = obj["id"].isString() ? obj["id"].toString() : obj["id"].toObject()["value"].toString();
            m_astNodes[idStr] = obj;
        }
    };

    if (params.contains("node")) extractNode(params["node"].toObject());
    if (params.contains("target")) extractNode(params["target"].toObject());
    if (params.contains("source")) extractNode(params["source"].toObject());

    if (method == "parser/didMakeNewSubtree" || method == "parser/didJoin") {
        QJsonObject eventObj;
        eventObj["method"] = method;
        eventObj["params"] = params;
        m_parserEvents.append(eventObj);
    }
}

QString LogicEditorWindow::astToFormula(const QString &nodeId) {
    if (!m_astNodes.contains(nodeId)) return "[?]";

    QJsonObject node = m_astNodes[nodeId];
    QString token = node["lexeme"].toObject()["token"].toString();

    if (token == "\\wedge") token = "∧";
    else if (token == "\\vee") token = "∨";
    else if (token == "\\implies") token = "→";
    else if (token == "\\iff") token = "↔";
    else if (token == "\\neg") token = "¬";
    else if (token == "\\top") token = "⊤";
    else if (token == "\\bot") token = "⊥";

    QJsonArray children = node["children"].toArray();

    if (children.isEmpty()) {
        return token;
    } else if (children.size() == 1) {
        QString childId = children[0].isString() ? children[0].toString() : children[0].toObject()["value"].toString();
        return token + astToFormula(childId);
    } else {
        QStringList childFormulas;
        for (const QJsonValue &childVal : children) {
            QString childId = childVal.isString() ? childVal.toString() : childVal.toObject()["value"].toString();
            childFormulas.append(astToFormula(childId));
        }
        return "(" + childFormulas.join(" " + token + " ") + ")";
    }
}