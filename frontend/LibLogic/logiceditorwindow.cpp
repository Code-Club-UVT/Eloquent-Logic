#include "LogicEditorWindow.h"
#include "logiclineedit.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QKeySequence>
#include <QRegularExpression>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

LogicEditorWindow::LogicEditorWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    networkManager = new QNetworkAccessManager(this);
    formulaInput = new LogicLineEdit(this);
    formulaInput->setPlaceholderText("Introdu formula aici...");
    QFont font = formulaInput->font();
    font.setPointSize(14);
    formulaInput->setFont(font);

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
    QPushButton *btnEval = new QPushButton("See Result", this);

    btnFNN->setFont(font);
    btnFNC->setFont(font);
    btnFND->setFont(font);
    btnEval->setFont(font);

    actionLayout->addWidget(btnFNN);
    actionLayout->addWidget(btnFNC);
    actionLayout->addWidget(btnFND);
    actionLayout->addStretch();
    actionLayout->addWidget(btnEval);

    resultOutput = new QLineEdit(this);
    resultOutput->setPlaceholderText("The result will appear here");
    resultOutput->setFont(font);
    resultOutput->setReadOnly(true);

    mainLayout->addWidget(formulaInput);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(actionLayout);
    mainLayout->addWidget(resultOutput);

    connect(btnAnd, &QPushButton::clicked, [this]() { formulaInput->insert("and"); formulaInput->setFocus(); });
    connect(btnOr, &QPushButton::clicked, [this]() { formulaInput->insert("or"); formulaInput->setFocus(); });
    connect(btnImplies, &QPushButton::clicked, [this]() { formulaInput->insert("impl"); formulaInput->setFocus(); });
    connect(btnEquiv, &QPushButton::clicked, [this]() { formulaInput->insert("eq"); formulaInput->setFocus(); });
    connect(btnNot, &QPushButton::clicked, [this]() { formulaInput->insert("not"); formulaInput->setFocus(); });
    connect(btnEntailment, &QPushButton::clicked, [this]() { formulaInput->insert("ent"); formulaInput->setFocus(); });
    connect(btnTautology, &QPushButton::clicked, [this]() { formulaInput->insert("ta"); formulaInput->setFocus(); });
    connect(btnContradiction, &QPushButton::clicked, [this]() { formulaInput->insert("co"); formulaInput->setFocus(); });

    connect(btnFNN, &QPushButton::clicked, [this]() {
        formulaInput->setText("FNN form");
    });

    connect(btnFNC, &QPushButton::clicked, [this]() {
        formulaInput->setText("FNC form");
    });

    connect(btnFND, &QPushButton::clicked, [this]() {
        formulaInput->setText("FND form");
    });

    connect(btnEval, &QPushButton::clicked, [this]() {
        resultOutput->setText("Evaluating");

        QJsonObject jsonRequest;
        jsonRequest["jsonrpc"] = "2.0";
        jsonRequest["method"] = "evaluate_formula_properties";
        jsonRequest["id"] = 5;

        QJsonObject params;
        params["formula"] = getFormulaWithSymbols();
        jsonRequest["params"] = params;

        QJsonDocument doc(jsonRequest);
        QByteArray data = doc.toJson();

        QUrl url("http://localhost:8080");
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = networkManager->post(request, data);

        connect(reply, &QNetworkReply::finished, [this, reply]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray responseData = reply->readAll();
                QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
                QJsonObject responseObj = responseDoc.object();

                if (responseObj.contains("result")) {
                    QJsonObject resultObj = responseObj["result"].toObject();
                    QString classification = resultObj["classification"].toString();
                    resultOutput->setText(classification);
                }
            } else {
                resultOutput->setText("Server connection error.");
            }
            reply->deleteLater();
        });
    });
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