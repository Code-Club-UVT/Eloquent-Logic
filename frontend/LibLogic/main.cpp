#include "logiceditorwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogicEditorWindow w;
    w.setWindowTitle("Eloquent Logic Editor");
    w.resize(800, 600);
    w.show();

    return a.exec();
}