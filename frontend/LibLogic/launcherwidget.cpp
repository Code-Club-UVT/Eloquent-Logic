#include "LauncherWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QKeySequence>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

#include "TruthTableViewer.h"
#include "syntactictreeviewer.h"

LauncherWidget::LauncherWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *btnOpenTable = new QPushButton("Truth Table", this);
    QPushButton *btnOpenTree = new QPushButton("Syntactic Tree", this);
    networkManager = new QNetworkAccessManager(this);

    QFont btnFont = btnOpenTable->font();
    btnFont.setPointSize(11);
    btnOpenTable->setFont(btnFont);
    btnOpenTree->setFont(btnFont);

    buttonLayout->addWidget(btnOpenTable);
    buttonLayout->addWidget(btnOpenTree);

    mainLayout->addLayout(buttonLayout);

    connect(btnOpenTable, &QPushButton::clicked, [this]() {
        QString currentFormula = "A ∧ B";

        QJsonObject jsonRequest;
        jsonRequest["jsonrpc"] = "2.0";
        jsonRequest["method"] = "truth_table";
        jsonRequest["id"] = 1;

        QJsonObject params;
        params["formula"] = currentFormula;
        params["limit"] = 50;
        params["reset"] = true;
        jsonRequest["params"] = params;

        QNetworkRequest request(QUrl("http://localhost:8080"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = networkManager->post(request, QJsonDocument(jsonRequest).toJson());

        connect(reply, &QNetworkReply::finished, [this, reply]() {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonObject responseObj = QJsonDocument::fromJson(reply->readAll()).object();

                if (responseObj.contains("result")) {
                    QJsonObject resultObj = responseObj["result"].toObject();

                    QStringList headers;
                    QJsonArray headersJson = resultObj["headers"].toArray();
                    for (const QJsonValue& val : headersJson) {
                        headers.append(val.toString());
                    }

                    TruthTableViewer *tableWindow = new TruthTableViewer(headers);
                    tableWindow->setWindowTitle("Truth Table");
                    tableWindow->resize(450, 400);
                    tableWindow->setAttribute(Qt::WA_DeleteOnClose);

                    QJsonArray rowsJson = resultObj["rows"].toArray();
                    for (const QJsonValue& rowVal : rowsJson) {
                        QList<bool> rowData;
                        for (const QJsonValue& boolVal : rowVal.toArray()) {
                            rowData.append(boolVal.toBool());
                        }
                        tableWindow->addRow(rowData);
                    }

                    bool isFinished = resultObj["is_finished"].toBool();
                    tableWindow->setFinished(isFinished);
                    tableWindow->setLoading(false);

                    connect(tableWindow, &TruthTableViewer::requestMoreData, [this, tableWindow]() {
                        QJsonObject reqNext;
                        reqNext["jsonrpc"] = "2.0";
                        reqNext["method"] = "truth_table";
                        reqNext["id"] = 2;

                        QJsonObject paramNext;
                        paramNext["formula"] = QJsonValue::Null;
                        paramNext["limit"] = 50;
                        paramNext["reset"] = false;
                        reqNext["params"] = paramNext;

                        QNetworkRequest requestNext(QUrl("http://localhost:8080"));
                        requestNext.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                        QNetworkReply *replyNext = networkManager->post(requestNext, QJsonDocument(reqNext).toJson());

                        connect(replyNext, &QNetworkReply::finished, [tableWindow, replyNext]() {
                            if (replyNext->error() == QNetworkReply::NoError) {
                                QJsonObject resObj = QJsonDocument::fromJson(replyNext->readAll()).object();

                                if (resObj.contains("result")) {
                                    QJsonObject rObj = resObj["result"].toObject();

                                    QJsonArray rJson = rObj["rows"].toArray();
                                    for (const QJsonValue& rowV : rJson) {
                                        QList<bool> rData;
                                        for (const QJsonValue& bVal : rowV.toArray()) {
                                            rData.append(bVal.toBool());
                                        }
                                        tableWindow->addRow(rData);
                                    }

                                    tableWindow->setFinished(rObj["is_finished"].toBool());
                                }
                            }
                            tableWindow->setLoading(false);
                            replyNext->deleteLater();
                        });
                    });

                    tableWindow->show();
                }
            } else {
            }
            reply->deleteLater();
        });
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