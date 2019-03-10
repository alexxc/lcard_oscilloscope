#include "TableViewSettings.h"
#include <QTableView>
#include <QHeaderView>
#include <QApplication>
#include <QSettings>

#include "actions/TableCopyAction.h"

TableViewSettings *TableViewSettings::instance() {
    static TableViewSettings settings;
    return &settings;
}

QBrush TableViewSettings::readonlyTableElementBrush() const {
    QPalette palette = QApplication::palette();
    palette.setCurrentColorGroup(QPalette::Disabled);
    return palette.background();
}

QBrush TableViewSettings::notAssignedTableElementBrush() const {
    QPalette palette = QApplication::palette();
    palette.setCurrentColorGroup(QPalette::Inactive);
    return palette.background();
}



void TableViewSettings::initDataView(QTableView *view, QString name) const {
    TableCopyAction *copy_act = new TableCopyAction(view);
    connect(this, SIGNAL(languageChanged()), copy_act, SLOT(retranslate()));
    view->addAction(copy_act);
    view->setAlternatingRowColors(true);
    view->verticalHeader()->setVisible(false);    


    if (!name.isEmpty()) {
        view->setObjectName(name);
        if (view->model()) {
            QSettings set;
            int col_cnt = set.value(name + "ColumnCnt").toInt();
            if ((col_cnt != 0) && (col_cnt == view->model()->columnCount())) {
                QByteArray state = set.value(name + "ColumnsState").toByteArray();
                if (state.size()!=0)
                    view->horizontalHeader()->restoreState(state);
            }
        }
    }
}

void TableViewSettings::saveDataView(QTableView *view) const {
    QString name = view->objectName();
    if (!name.isEmpty() && view->model()) {
        QSettings set;
        set.setValue(name + "ColumnCnt", view->model()->columnCount());
        set.setValue(name + "ColumnsState", view->horizontalHeader()->saveState());
    }
}

void TableViewSettings::retranslate() {
    Q_EMIT languageChanged();
}

TableViewSettings::TableViewSettings() :
    m_desabledTableElemBrush(Qt::lightGray),
    m_positiveTableElemBrush(Qt::darkGreen),
    m_negativeTableElemBrush(Qt::red) {

}
