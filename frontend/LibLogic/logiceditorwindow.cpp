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
#include <QJsonDocument>
#include <QDialog>
#include <QTextBrowser>
#include <QSet>

LogicEditorWindow::LogicEditorWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    formulaInput = new LogicLineEdit(this);
    formulaInput->setPlaceholderText("Write a formula here...");
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
    buttonLayout->addStretch();

    QPushButton *btnInfo = new QPushButton("Info", this);
    btnInfo->setToolTip("User Guide and Shortcuts");
    buttonLayout->addWidget(btnInfo);

    connect(btnInfo, &QPushButton::clicked, [this]() {
        QDialog *infoDialog = new QDialog(this);
        infoDialog->setWindowTitle("User Guide");
        infoDialog->resize(650, 700);
        infoDialog->setAttribute(Qt::WA_DeleteOnClose);

        QVBoxLayout *layout = new QVBoxLayout(infoDialog);
        QTextBrowser *browser = new QTextBrowser(infoDialog);

        QString html = R"(
            <div style="font-family: 'Segoe UI', Arial, sans-serif; padding: 10px;">
                <!-- SEȚIUNEA ÎN ROMÂNĂ -->
                <h2 style="color: #005A9E; border-bottom: 2px solid #005A9E; padding-bottom: 5px;">🇷🇴 Ghid de Utilizare - Eloquent Logic</h2>

                <h3 style="color: #2b579a;">Scurtături de Tastatură (Simboluri Logice)</h3>
                <p>Poți introduce simbolurile logice rapid folosind combinațiile de taste sau scriind direct următoarele cuvinte:</p>
                <ul>
                    <li><b>ȘI ( ∧ )</b> : <code>Alt + A</code> sau scrie <code>and</code></li>
                    <li><b>SAU ( ∨ )</b> : <code>Alt + O</code> sau scrie <code>or</code></li>
                    <li><b>IMPLICAȚIE ( → )</b> : <code>Alt + I</code> sau scrie <code>im</code></li>
                    <li><b>ECHIVALENȚĂ ( ↔ )</b> : <code>Alt + E</code> sau scrie <code>eq</code></li>
                    <li><b>NEGAȚIE ( ¬ )</b> : <code>Alt + N</code> sau scrie <code>not</code></li>
                    <li><b>CONSECINȚĂ / ENTAILMENT ( ⊨ )</b> : <code>Alt + L</code> sau scrie <code>ent</code></li>
                    <li><b>TAUTOLOGIE ( ⊤ )</b> : <code>Alt + T</code> sau scrie <code>ta</code></li>
                    <li><b>CONTRADICȚIE ( ⊥ )</b> : <code>Alt + C</code> sau scrie <code>co</code></li>
                </ul>

                <h3 style="color: #2b579a;">Butoane și Funcționalități</h3>
                <ul>
                    <li><b>SAT:</b> Verifică satisfiabilitatea formulei (dacă poate fi adevărată) folosind algoritmul selectat din meniul derulant (ex: DPLL, DP, Rezoluție). Rezultatul va fi afișat în bara de jos.</li>
                    <li><b>FNN / FNC / FND:</b> Transformă formula curentă în Forma Normală Negativă, Conjunctivă sau Disjunctivă. <i>Atenție: Rezultatul va înlocui automat textul din caseta de input pentru a putea continua să analizezi noua formă!</i></li>
                    <li><b>Tabel Adevăr:</b> Deschide o fereastră care generează automat tabelul de adevăr, calculând valoarea formulei pentru absolut toate combinațiile de valori (True/False) ale variabilelor.</li>
                    <li><b>See Result (AST):</b> Analizează formula curentă și deschide Arborele Sintactic.</li>
                </ul>

                <h3 style="color: #2b579a;">Arborele Sintactic (Syntactic Tree)</h3>
                <p>Oferă o reprezentare vizuală a modului în care parser-ul și algoritmii de transformare structurează formula.</p>
                <ul>
                    <li><b>Navigare:</b> Dă <i>Click-Stânga</i> oriunde pe fundalul gri și trage de mouse (Drag) pentru a te plimba pe hartă.</li>
                    <li><b>Zoom:</b> Folosește <i>Rotița mouse-ului</i> (Scroll) pentru a apropia sau depărta vederea.</li>
                    <li><b>Animație (Istoric):</b> Folosește butoanele <b>Play / Pause</b>, <b>Next</b> și <b>Back</b> din partea de jos pentru a urmări pas cu pas cum s-a construit arborele original sau cum au tăiat și unit algoritmii de transformare ramurile arborelui.</li>
                </ul>

                <br><hr style="border: 1px solid #ccc; margin: 20px 0;"><br>

                <!-- SEȚIUNEA ÎN ENGLEZĂ -->
                <h2 style="color: #005A9E; border-bottom: 2px solid #005A9E; padding-bottom: 5px;">🇬🇧 User Guide - Eloquent Logic</h2>

                <h3 style="color: #2b579a;">Keyboard Shortcuts (Logical Symbols)</h3>
                <p>You can quickly insert logical symbols using key combinations or by typing the following keywords:</p>
                <ul>
                    <li><b>AND ( ∧ )</b> : <code>Alt + A</code> or type <code>and</code></li>
                    <li><b>OR ( ∨ )</b> : <code>Alt + O</code> or type <code>or</code></li>
                    <li><b>IMPLICATION ( → )</b> : <code>Alt + I</code> or type <code>im</code></li>
                    <li><b>EQUIVALENCE ( ↔ )</b> : <code>Alt + E</code> or type <code>eq</code></li>
                    <li><b>NEGATION ( ¬ )</b> : <code>Alt + N</code> or type <code>not</code></li>
                    <li><b>ENTAILMENT ( ⊨ )</b> : <code>Alt + L</code> or type <code>ent</code></li>
                    <li><b>TAUTOLOGY ( ⊤ )</b> : <code>Alt + T</code> or type <code>ta</code></li>
                    <li><b>CONTRADICTION ( ⊥ )</b> : <code>Alt + C</code> or type <code>co</code></li>
                </ul>

                <h3 style="color: #2b579a;">Buttons and Features</h3>
                <ul>
                    <li><b>SAT:</b> Checks the satisfiability of the formula (whether it can evaluate to true) using the selected algorithm from the dropdown menu (e.g., DPLL, DP, Resolution). The result will be displayed in the bottom bar.</li>
                    <li><b>NNF / CNF / DNF:</b> Transforms the current formula into Negation, Conjunctive, or Disjunctive Normal Form. <i>Note: The result will automatically replace the text in the input box so you can seamlessly continue analyzing the new form!</i></li>
                    <li><b>Truth Table:</b> Opens a window that automatically generates the truth table, calculating the formula's value for absolutely all combinations of truth values (True/False) of the variables.</li>
                    <li><b>See Result (AST):</b> Analyzes the current formula and opens the Syntactic Tree.</li>
                </ul>

                <h3 style="color: #2b579a;">Syntactic Tree (AST)</h3>
                <p>Provides a visual representation of how the parser and transformation algorithms structure the formula.</p>
                <ul>
                    <li><b>Navigation:</b> <i>Left-Click</i> anywhere on the gray background and drag your mouse to move around the canvas.</li>
                    <li><b>Zoom:</b> Use the <i>Mouse Wheel</i> (Scroll) to zoom in or out.</li>
                    <li><b>Animation (History):</b> Use the <b>Play / Pause</b>, <b>Next</b>, and <b>Back</b> buttons at the bottom to watch step-by-step how the original tree was built or how the transformation algorithms cut and merged the branches.</li>
                </ul>
            </div>
        )";

        browser->setHtml(html);
        layout->addWidget(browser);
        infoDialog->show();
    });

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

    QPushButton *btnFNN = new QPushButton("NNF", this);
    QPushButton *btnFNC = new QPushButton("CNF", this);
    QPushButton *btnFND = new QPushButton("DNF", this);
    QPushButton *btnTruthTable = new QPushButton("Truth Table", this);
    QPushButton *btnEval = new QPushButton("Syntactic Tree", this);
    QPushButton *btnSat = new QPushButton("SAT", this);

    btnFNC->setObjectName("btnFNC");

    satAlgoSelector = new QComboBox(this);
    satAlgoSelector->setObjectName("satAlgoSelector");
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
    resultOutput->setObjectName("resultOutput");
    resultOutput->setPlaceholderText("The result will appear here");
    resultOutput->setFont(font);
    resultOutput->setReadOnly(true);

    logOutput = new QTextEdit(this);
    logOutput->setReadOnly(true);
    logOutput->setMaximumHeight(100);
    logOutput->setPlaceholderText("Live notifications (lexer/parser) will appear here...");

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
        resultOutput->setText("The Truth Table is generating...");
        m_agent->sendRequest("truth_table", params, 2);
    });

    connect(btnSat, &QPushButton::clicked, [this]() {
        QJsonObject params;
        params["formula"] = getFormulaWithSymbols();
        params["algorithm"] = satAlgoSelector->currentData().toString();
        resultOutput->setText("Checking satisfiability (" + satAlgoSelector->currentText() + ")...");
        logOutput->clear();
        m_agent->sendRequest("sat", params, 6);
    });

    connect(btnFNN, &QPushButton::clicked, [this]() {
        QJsonObject params;
        params["formula"] = getFormulaWithSymbols();
        params["target"] = "nnf";
        resultOutput->setText("Computing NNF...");
        m_agent->sendRequest("transform", params, 3);
    });

    connect(btnFNC, &QPushButton::clicked, [this]() {
        QJsonObject params;
        params["formula"] = getFormulaWithSymbols();
        params["target"] = "cnf";
        resultOutput->setText("Computing CNF...");
        m_agent->sendRequest("transform", params, 4);
    });

    connect(btnFND, &QPushButton::clicked, [this]() {
        QJsonObject params;
        params["formula"] = getFormulaWithSymbols();
        params["target"] = "dnf";
        resultOutput->setText("Computing DNF...");
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
        resultOutput->setText("Generating tree...");
        logOutput->clear();
        m_agent->sendRequest("parse", params, 1);
    });

    mainLayout->addWidget(logOutput);
}

LogicEditorWindow::~LogicEditorWindow() {
    if (m_agent) {
        m_agent->disconnect();
    }
}

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

void LogicEditorWindow::extractNodeRecursively(const QJsonObject &obj, QMap<QString, QJsonObject> &mapToFill) {
    if (obj.contains("id") && obj.contains("lexeme")) {
        QString idStr = obj["id"].isString() ? obj["id"].toString() : obj["id"].toObject()["value"].toString();
        mapToFill[idStr] = obj;
        if (obj.contains("children")) {
            for (const QJsonValue &childVal : obj["children"].toArray()) {
                if (childVal.isObject()) {
                    extractNodeRecursively(childVal.toObject(), mapToFill);
                }
            }
        }
    }
}

void LogicEditorWindow::onAgentResponse(int id, const QJsonObject &result) {
    resultOutput->setStyleSheet("");

    if (id == 1) {
        if (result.contains("tree")) {
            QJsonObject tree = result["tree"].toObject();
            extractNodeRecursively(tree, m_astNodes);

            resultOutput->setText("Success! The tree was generated.");
            SyntacticTreeViewer *treeWindow = new SyntacticTreeViewer();
            treeWindow->setWindowTitle("Syntactic Tree");
            treeWindow->resize(600, 500);
            treeWindow->setAttribute(Qt::WA_DeleteOnClose);
            treeWindow->loadHistory(m_parserEvents);
            treeWindow->show();
        } else {
            resultOutput->setText("Error: The response does not contain a valid tree.");
        }
    } else if (id == 2) {
        resultOutput->setText("Success! The table was generated.");
        QStringList headers;
        if (result.contains("headers")) {
            QJsonArray headersArr = result["headers"].toArray();
            for (const QJsonValue &h : headersArr) {
                QString headerText = h.toString();
                headerText.replace("\\wedge", "∧").replace("\\vee", "∨")
                    .replace("\\implies", "→").replace("\\iff", "↔")
                    .replace("\\neg", "¬").replace("\\bot", "⊥")
                    .replace("\\top", "⊤").replace("\\models", "⊨");
                headers.append(headerText);
            }
        }

        TruthTableViewer *tableWindow = new TruthTableViewer(headers);
        tableWindow->setWindowTitle("Truth Table");
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

            extractNodeRecursively(tree, m_astNodes);

            QString rootId = tree["id"].isString() ? tree["id"].toString() : tree["id"].toObject()["value"].toString();
            QString newFormula = astToFormula(rootId);

            resultOutput->setText("Success: " + newFormula);

            if (!newFormula.contains("[?]")) {
                formulaInput->setText(newFormula);
            }

            SyntacticTreeViewer *treeWindow = new SyntacticTreeViewer();
            treeWindow->setWindowTitle("Transformare pe pași - Arbore Sintactic");
            treeWindow->resize(600, 500);
            treeWindow->setAttribute(Qt::WA_DeleteOnClose);

            treeWindow->loadHistory(m_parserEvents);
            treeWindow->show();
        } else {
            resultOutput->setText("Response received, but it does not contain a tree.");
        }
    } else if (id == 6) {
    bool isSat = false;

    // Citim varianta trimisă de backend-ul tău: "state": "SAT"
    if (result.contains("state")) {
        QString stateStr = result["state"].toString().toUpper();
        if (stateStr == "SAT") {
            isSat = true;
        }
    }
    // Fallback în caz că în viitor modifici backend-ul să trimită boolean
    else if (result.contains("satisfiable")) {
        isSat = result["satisfiable"].toBool();
    }

    // Actualizăm interfața grafică
    if (isSat) {
        resultOutput->setText("SAT Result: The formula is SATISFIABLE.");
        resultOutput->setStyleSheet("color: green; font-weight: bold;");
    } else {
        resultOutput->setText("SAT Result: The formula is UNSATISFIABLE (Contradiction).");
        resultOutput->setStyleSheet("color: red; font-weight: bold;");
    }
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

    if (params.contains("node")) extractNodeRecursively(params["node"].toObject(), m_astNodes);
    if (params.contains("target")) extractNodeRecursively(params["target"].toObject(), m_astNodes);
    if (params.contains("source")) extractNodeRecursively(params["source"].toObject(), m_astNodes);
    if (params.contains("new_tree")) extractNodeRecursively(params["new_tree"].toObject(), m_astNodes);
    if (params.contains("parent")) extractNodeRecursively(params["parent"].toObject(), m_astNodes);
    if (params.contains("child")) extractNodeRecursively(params["child"].toObject(), m_astNodes);
    if (params.contains("merged_child")) extractNodeRecursively(params["merged_child"].toObject(), m_astNodes);

    if (method == "parser/didMakeNewSubtree" || method == "parser/didJoin" ||
        method == "transform/didSpawnNewSubtree" || method == "transform/didAdoptNode" ||
        method == "transform/didDisconnect" || method == "transform/didTransformNodeWithLexeme" ||
        method == "transform/didCondenseChild")
    {
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
    else if (token == "\\models") token = "⊨";

    QJsonArray children = node["children"].toArray();

    auto getChildId = [](const QJsonValue &val) -> QString {
        if (val.isString()) return val.toString();
        QJsonObject obj = val.toObject();
        if (obj.contains("id")) {
            return obj["id"].isString() ? obj["id"].toString() : obj["id"].toObject()["value"].toString();
        }
        return obj["value"].toString();
    };

    if (children.isEmpty()) {
        return token;
    } else if (children.size() == 1) {
        return token + astToFormula(getChildId(children[0]));
    } else {
        QStringList childFormulas;
        for (const QJsonValue &childVal : children) {
            childFormulas.append(astToFormula(getChildId(childVal)));
        }
        return "(" + childFormulas.join(" " + token + " ") + ")";
    }
}