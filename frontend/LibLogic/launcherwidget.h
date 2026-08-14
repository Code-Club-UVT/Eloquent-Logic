#ifndef LAUNCHER_WIDGET_H
#define LAUNCHER_WIDGET_H

#include <QWidget>

class QNetworkAccessManager;

class LauncherWidget : public QWidget {
    Q_OBJECT
    QNetworkAccessManager *networkManager;

public:
    explicit LauncherWidget(QWidget *parent = nullptr);
};

#endif // LAUNCHER_WIDGET_H