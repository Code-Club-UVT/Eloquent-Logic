#include "agentconnection.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>

AgentConnection::AgentConnection(QObject *parent)
    : QObject(parent), m_process(new QProcess(this))
{
    connect(m_process, &QProcess::readyReadStandardOutput,
            this, &AgentConnection::onReadyReadStandardOutput);
}

AgentConnection::~AgentConnection()
{
    stopAgent();
}

bool AgentConnection::startAgent(const QString &executablePath)
{
    m_process->start(executablePath);
    return m_process->waitForStarted();
}

void AgentConnection::stopAgent()
{
    if (m_process->state() == QProcess::Running) {
        sendRequest("shutdown", QJsonObject(), 9999);
        m_process->waitForFinished(1000);
    }
}

void AgentConnection::sendRequest(const QString &method, const QJsonObject &params, int id)
{
    if (m_process->state() != QProcess::Running) {
        qWarning() << "Agentul nu rulează!";
        return;
    }

    QJsonObject requestObj;
    requestObj["jsonrpc"] = "2.0";
    requestObj["method"] = method;
    requestObj["params"] = params;
    requestObj["id"] = id;

    QJsonDocument doc(requestObj);
    m_process->write(doc.toJson(QJsonDocument::Compact) + "\n");
}

void AgentConnection::onReadyReadStandardOutput()
{
    while (m_process->canReadLine()) {
        QByteArray line = m_process->readLine().trimmed();
        if (line.isEmpty()) continue;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(line, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "Eroare la parsarea JSON-ului de la server:" << parseError.errorString() << line;
            continue;
        }

        QJsonObject obj = doc.object();

        if (obj.contains("id") && !obj["id"].isNull()) {
            int id = obj["id"].toInt();
            if (obj.contains("error")) {
                emit errorReceived(id, obj["error"].toObject());
            } else {
                emit responseReceived(id, obj["result"].toObject());
            }
        } else if (obj.contains("method")) {
            QString method = obj["method"].toString();
            QJsonObject params = obj["params"].toObject();
            emit notificationReceived(method, params);
        }
    }
}