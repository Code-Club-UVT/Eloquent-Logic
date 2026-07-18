#ifndef LOGICEDITORWINDOW_H
#define LOGICEDITORWINDOW_H

#include <QWidget>
#include <QString>

class QLineEdit;
class QPushButton;

class LogicEditorWindow : public QWidget {
    Q_OBJECT
    QLineEdit *formulaInput;

public:
    LogicEditorWindow(QWidget *parent = nullptr);
    ~LogicEditorWindow() override = default;
    QString getFormula() const;
};

#endif // LOGICEDITORWINDOW_H