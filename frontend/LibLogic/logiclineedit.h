#ifndef LOGICLINEEDIT_H
#define LOGICLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QMap>

class LogicLineEdit : public QLineEdit {
    Q_OBJECT
    QMap<QString, QString> replacements;
    bool justReplaced;
    QString lastWordReplaced;

public:
    LogicLineEdit(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onTextEdited(const QString &text);
};

#endif // LOGICLINEEDIT_H