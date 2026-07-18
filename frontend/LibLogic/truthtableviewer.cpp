#include "TruthTableViewer.h"
#include <QVBoxLayout>
#include <QHeaderView>

TruthTableViewer::TruthTableViewer(const QStringList& headers, QWidget *parent)
    : QWidget(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);

    table = new QTableWidget(this);

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setAlternatingRowColors(true);

    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);

    layout->addWidget(table);
}

void TruthTableViewer::addRow(const QList<bool>& rowData) {
    if (rowData.size() != table->columnCount()) {
        return;
    }

    int row = table->rowCount();
    table->insertRow(row);

    for (int i = 0; i < rowData.size(); ++i) {
        bool val = rowData[i];
        QTableWidgetItem *item = new QTableWidgetItem(val ? "T" : "F");
        item->setTextAlignment(Qt::AlignCenter);

        item->setBackground(val ? QColor(144, 238, 144, 100) : QColor(255, 182, 193, 100));

        if (i == rowData.size() - 1) {
            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
        }

        table->setItem(row, i, item);
    }
}