#ifndef TREESTEP_H
#define TREESTEP_H

#include <QUuid>
#include <QColor>
#include <QString>

struct TreeStep {
    QUuid nodeId;
    QColor oldColor;
    QColor newColor;
    QString description;
};

#endif // TREESTEP_H