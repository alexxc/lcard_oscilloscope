#ifndef TABLECOPYACTION_H
#define TABLECOPYACTION_H

#include <QAction>
class QTableView;
class QEvent;

class TableCopyAction : public QAction {
    Q_OBJECT
public:
    TableCopyAction(QTableView *parent);
    ~TableCopyAction();



public slots:
    void copy();
    void retranslate();
private:
    QTableView *m_view;
};

#endif // LQTABLECOPYACTION_H
