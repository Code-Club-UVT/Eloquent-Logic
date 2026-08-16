#ifndef AGENTCONNECTION_H
#define AGENTCONNECTION_H

#include <QObject>
#include <QProcess>
#include <QJsonObject>

class AgentConnection : public QObject
{
    Q_OBJECT
public:
    explicit AgentConnection(QObject *parent = nullptr);
    ~AgentConnection();
    bool startAgent(const QString &executablePath);
    void stopAgent();
    void sendRequest(const QString &method, const QJsonObject &params, int id);

signals:
    void responseReceived(int id, const QJsonObject &result);
    void errorReceived(int id, const QJsonObject &error);
    void notificationReceived(const QString &method, const QJsonObject &params);

private slots:
    void onReadyReadStandardOutput();

private:
    QProcess *m_process;
};

#endif // AGENTCONNECTION_H