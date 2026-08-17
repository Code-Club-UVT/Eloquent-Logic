#ifndef LOGICEDITORWINDOW_H
#define LOGICEDITORWINDOW_H

#include <QWidget>
#include <QString>
#include <QMap>
#include <QTextEdit>
#include <QJsonArray>
#include <QComboBox>

namespace eloquent::liblogic::symbols {
constexpr char SYMB_NOT[] =  R"(\neg)";
constexpr char SYMB_AND[] = R"(\wedge)";
constexpr char SYMB_OR[] = R"(\vee)";
constexpr char SYMB_IMPL[] = R"(\implies)";
constexpr char SYMB_IFF[] = R"(\iff)";
constexpr char SYMB_LEQUI[] = R"(\models)";
constexpr char SYMB_TAUTOLOGY[] = R"(\top)";
constexpr char SYMB_CONTRADICTION[] = R"(\bot)";
}

class QLineEdit;
class QPushButton;
class AgentConnection;

class LogicEditorWindow : public QWidget {
    Q_OBJECT
    QLineEdit *formulaInput;
    QLineEdit *resultOutput;
    AgentConnection *m_agent;
    QTextEdit *logOutput;
    QMap<QString, QJsonObject> m_astNodes;
    QJsonArray m_parserEvents;
    QComboBox *satAlgoSelector;

public:
    LogicEditorWindow(QWidget *parent = nullptr);
    ~LogicEditorWindow() override;
    QString getFormula() const;
    QString getFormulaWithSymbols() const;

private:
    QString astToFormula(const QString &nodeId);
    void extractNodeRecursively(const QJsonObject &obj, QMap<QString, QJsonObject> &mapToFill);

private slots:
    void onAgentResponse(int id, const QJsonObject &result);
    void onAgentError(int id, const QJsonObject &error);
    void onAgentNotification(const QString &method, const QJsonObject &params);
};

#endif // LOGICEDITORWINDOW_H