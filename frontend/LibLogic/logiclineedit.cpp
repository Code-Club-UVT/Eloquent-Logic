#include "LogicLineEdit.h"

LogicLineEdit::LogicLineEdit(QWidget *parent) : QLineEdit(parent), justReplaced(false) {
    replacements["and"] = "∧";
    replacements["or"] = "∨";
    replacements["nand"] = "∇";
    replacements["nor"] = "|";
    replacements["not"] = "¬";
    replacements["ent"] = "⊨";
    replacements["eq"] = "↔";
    replacements["impl"] = "→";

    connect(this, &QLineEdit::textEdited, this, &LogicLineEdit::onTextEdited);
}

void LogicLineEdit::onTextEdited(const QString &text) {
    int cursorPos = cursorPosition();
    if (cursorPos == 0) return;

    QString textBeforeCursor = text.left(cursorPos);

    int lastSpaceIndex = textBeforeCursor.lastIndexOf(' ');
    QString currentWord = textBeforeCursor.mid(lastSpaceIndex + 1);

    if (replacements.contains(currentWord.toLower())) {
        QString symbol = replacements[currentWord.toLower()];

        QString newText = text;
        newText.remove(cursorPos - currentWord.length(), currentWord.length());
        newText.insert(cursorPos - currentWord.length(), symbol);

        blockSignals(true);
        setText(newText);
        setCursorPosition(cursorPos - currentWord.length() + symbol.length());
        blockSignals(false);

        justReplaced = true;
        lastWordReplaced = currentWord;
    }
}

void LogicLineEdit::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Backspace && justReplaced) {
        int cursorPos = cursorPosition();

        QString currentTxt = text();
        currentTxt.remove(cursorPos - 1, 1);

        QString restoredWord = lastWordReplaced;
        restoredWord.chop(1);

        currentTxt.insert(cursorPos - 1, restoredWord);

        blockSignals(true);
        setText(currentTxt);
        setCursorPosition(cursorPos - 1 + restoredWord.length());
        blockSignals(false);

        justReplaced = false;

        return;
    }

    if (event->key() != Qt::Key_Backspace) {
        justReplaced = false;
    }

    QLineEdit::keyPressEvent(event);
}