#ifndef LOGICEDITORWINDOW_H
#define LOGICEDITORWINDOW_H

#include <QWidget>
#include <QString>

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

class LogicEditorWindow : public QWidget {
    Q_OBJECT
    QLineEdit *formulaInput;
    QLineEdit *resultOutput;

public:
    LogicEditorWindow(QWidget *parent = nullptr);
    ~LogicEditorWindow() override = default;
    QString getFormula() const;
    QString getFormulaWithSymbols() const;
};

#endif // LOGICEDITORWINDOW_H