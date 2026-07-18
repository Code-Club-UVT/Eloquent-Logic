#include "logiceditorwindow.h"
#include "launcherwidget.h"
#include <QApplication>
#include <QMainWindow>
#include <QStringList>
#include <QList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogicEditorWindow *w = new LogicEditorWindow;
    LauncherWidget *lw = new LauncherWidget;

    QMainWindow mainWindow;
    mainWindow.setCentralWidget(w);
    mainWindow.setMenuWidget(lw);
    mainWindow.show();
    return QApplication::exec();
}