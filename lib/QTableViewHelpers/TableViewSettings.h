#ifndef TABLEVIEWSETTINGS_H
#define TABLEVIEWSETTINGS_H

#include <QBrush>
#include <QObject>
class QTableView;

class TableViewSettings : public QObject {
    Q_OBJECT
public:
    static TableViewSettings *instance();

    QBrush disabledTableElementBrush() const {return m_desabledTableElemBrush;}
    QBrush readonlyTableElementBrush() const;
    QBrush notAssignedTableElementBrush() const;
    QBrush positiveTableElementBrush() const {return m_positiveTableElemBrush;}
    QBrush negativeTableElementBrush() const {return m_negativeTableElemBrush;}

    void initDataView(QTableView *view, QString name) const;
    void saveDataView(QTableView *view) const;

public Q_SLOTS:
    void retranslate();
Q_SIGNALS:
    void languageChanged();
private:
    TableViewSettings();

    QBrush m_desabledTableElemBrush;
    QBrush m_positiveTableElemBrush;
    QBrush m_negativeTableElemBrush;
};

#endif // TABLEVIEWSETTINGS_H
