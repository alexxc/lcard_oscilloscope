#ifndef TABLEPARAMSMODELHELPER_H
#define TABLEPARAMSMODELHELPER_H

#include "TableParameter.h"
#include "TableParamsModelIface.h"
#include <QTableView>


template <typename ParamItem> class TableParamsModelHelper {
public:
    virtual ~TableParamsModelHelper() {qDeleteAll(m_params);}
    void paramInitView(QTableView *view) const;

    int paramCount() const {return m_params.size();}
    int paramItemsCount() const {return m_items.size();}

    void paramItemsClear() {m_items.clear();}
    ParamItem *paramItem(int idx) const {return m_items[idx];}
    void paramItemRem(int idx) {onItemRemove(idx, paramItem(idx)); m_items.removeAt(idx);}
    void paramItemAdd(ParamItem *item) { m_items.append(item); onItemInsert(m_items.size()-1, item); }
    void paramItemInsert(int idx, ParamItem *item) {m_items.insert(idx, item);  onItemInsert(idx, item);}

    QVariant paramData(QModelIndex index, int role) const;
    bool paramSetData(TableParamsModelIface *model, const QModelIndex &index, const QVariant &value, int role);



    QVariant paramHeaderData(int section, Qt::Orientation orientation,
                             int role = Qt::DisplayRole ) const {
        QVariant ret;
        if (orientation==Qt::Horizontal) {
            if (role == Qt::DisplayRole)
                return m_params[section]->name();
            if (role == Qt::ToolTipRole)
                return m_params[section]->toolTip();
        }
        return ret;
    }

    Qt::ItemFlags paramFlags(const QModelIndex &index) const {
        if (!index.isValid())
            return 0;
        return m_params[index.column()]->flags(m_items[index.row()]);
    }

    class Param : public TableParameter<ParamItem *> {
    };

    const Param *paramAt(int idx) const {return m_params.at(idx);}

    QList<int> paramIndexList(bool (*check_cb)(const Param *)) const;
protected:
    void paramAdd(Param *param);

    /* данные функции вызываются всякий раз после добавления или перед удалением
       элемента соответственно и служат для того, чтобы их можно было переопределить
       в унаследованном классе для выполнения каких-либо доп. действий, например
       подключения сигналов о изменении параметров и т.п. */
    virtual void onItemInsert(int idx, ParamItem *item) {}
    virtual void onItemRemove(int idx, ParamItem *item) {}
private:
    QList<Param *> m_params;
    QList<ParamItem *> m_items;
};



template <typename ParamItem>  void TableParamsModelHelper<ParamItem>::paramInitView(QTableView *view) const {
    for (int col = 0; col < m_params.size(); col++) {
        QAbstractItemDelegate *deletgate = m_params[col]->createDelegate(view);
        if (deletgate)
            view->setItemDelegateForColumn(col, deletgate);
        if (m_params[col]->columnWidth() > 0)
            view->setColumnWidth(col, m_params[col]->columnWidth());
    }
}

template <typename ParamItem> QVariant TableParamsModelHelper<ParamItem>::paramData(QModelIndex index, int role) const {
    QVariant ret;
    if (index.isValid()) {
        ret = m_params[index.column()]->data(m_items[index.row()], role);
    }
    return ret;
}

template <typename ParamItem> bool TableParamsModelHelper<ParamItem>::paramSetData(
        TableParamsModelIface *model, const QModelIndex &index, const QVariant &value, int role) {
    bool ok = false;
    if (index.isValid()) {
        ok = m_params[index.column()]->setData(m_items[index.row()], value, role);
        if (ok) {
            if (m_params[index.column()]->itemUpdateRequired()) {
                model->updateItem(index.row());
            } else if (m_params[index.column()]->itemParamUpdateReqired()) {
                model->updateItemParam(index.row(), index.column());
            }
        }
    }
    return ok;
}

template <typename ParamItem> QList<int> TableParamsModelHelper<ParamItem>::paramIndexList(bool (*check_cb)(const Param *)) const {
    QList<int> ret;
    for (int param_idx = 0; param_idx < paramCount(); param_idx++) {
        if (check_cb(paramAt(param_idx))) {
            ret.append(param_idx);
        }
    }
    return ret;
}

template <typename ParamItem> void TableParamsModelHelper<ParamItem>::paramAdd(Param *param) {
    m_params.append(param);
}

#endif // TABLEPARAMSMODELHELPER_H
