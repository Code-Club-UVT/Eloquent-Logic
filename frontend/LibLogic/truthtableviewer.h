#ifndef TRUTH_TABLE_VIEWER_H
#define TRUTH_TABLE_VIEWER_H

#include <QWidget>
#include <QTableWidget>
#include <QStringList>

class TruthTableViewer : public QWidget {
    Q_OBJECT
    QTableWidget *table;
    bool m_isLoading;
    bool m_isFinished;

public:
    TruthTableViewer(const QStringList& headers, QWidget *parent = nullptr);
    void addRow(const QList<bool>& rowData);
    void setLoading(bool loading);
    void setFinished(bool finished);

signals:
    void requestMoreData();

private slots:
    void onScroll(int value);
};

#endif