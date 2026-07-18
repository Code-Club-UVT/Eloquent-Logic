#ifndef TRUTH_TABLE_VIEWER_H
#define TRUTH_TABLE_VIEWER_H

#include <QWidget>
#include <QTableWidget>
#include <QStringList>
#include <QList>

class TruthTableViewer : public QWidget {
    Q_OBJECT
    QTableWidget *table;

public:
    TruthTableViewer(const QStringList& headers, QWidget *parent = nullptr);
    void addRow(const QList<bool>& rowData);

};

#endif