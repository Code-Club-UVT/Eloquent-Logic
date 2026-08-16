#ifndef LAUNCHER_WIDGET_H
#define LAUNCHER_WIDGET_H

#include <QWidget>

class LauncherWidget : public QWidget {
    Q_OBJECT

public:
    explicit LauncherWidget(QWidget *parent = nullptr);
};

#endif // LAUNCHER_WIDGET_H