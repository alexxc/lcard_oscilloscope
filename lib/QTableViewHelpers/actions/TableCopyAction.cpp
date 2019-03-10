#include "TableCopyAction.h"
#include <QItemSelectionModel>
#include <QTableView>
#include <QApplication>
#include <QClipboard>
#include <QKeySequence>
#include <QEvent>
#include <QKeyEvent>
#include <algorithm>

TableCopyAction::TableCopyAction(QTableView *parent) : QAction(parent), m_view(parent) {
    setShortcuts(QKeySequence::Copy);
    setShortcutContext(Qt::WidgetShortcut);
    connect(this, SIGNAL(triggered()), this, SLOT(copy()));
    retranslate();
}

TableCopyAction::~TableCopyAction() {

}

void TableCopyAction::copy() {
    QItemSelectionModel *selection = m_view->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();

    if (!indexes.isEmpty()) {
        std::sort(indexes.begin(), indexes.end());

        QString selected_text;
        QModelIndex previous = indexes.first();
        QAbstractItemModel *model = m_view->model();
        indexes.removeFirst();




        foreach(QModelIndex current, indexes) {
            if (!m_view->isColumnHidden(current.column()) &&
                    !m_view->isRowHidden(current.row())) {
                QVariant data = model->data(previous);
                QString text = data.toString();
                /* At this point `text` contains the text in one cell */
                selected_text.append(text);

                /* If you are at the start of the row the row number of the previous index
                  isn't the same.  Text is followed by a row separator, which is a newline. */
                if (current.row() != previous.row()) {
                    selected_text.append('\n');
                } else {
                    /* Otherwise it's the same row, so append a column separator, which is a tab. */
                    selected_text.append('\t');
                }
                previous = current;
            }
        }

        // At this point `text` contains the text in one cell
        selected_text.append(model->data(previous).toString());

        QApplication::clipboard()->setText(selected_text);
    }
}

void TableCopyAction::retranslate() {
    setText(tr("&Copy"));
    setStatusTip(tr("Copy selected text"));
}



