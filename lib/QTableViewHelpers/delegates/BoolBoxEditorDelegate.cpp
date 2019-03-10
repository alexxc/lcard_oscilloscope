#include "BoolBoxEditorDelegate.h"
#include <QComboBox>

BoolBoxEditorDelegate::BoolBoxEditorDelegate(QObject *parent) : ComboBoxEditorDelegate(parent) {
}

void BoolBoxEditorDelegate::fillBoxItems(QComboBox *box, const QModelIndex &index) const {
    box->addItem(valText(true), true);
    box->addItem(valText(false), false);
}

QVariant BoolBoxEditorDelegate::getBoxData(QComboBox *box, int idx) const {
    return box->currentData().toBool();
}

int BoolBoxEditorDelegate::getBoxIndexByData(QComboBox *box, const QModelIndex &index) const {
    bool val = index.data(Qt::EditRole).toBool();
    for (int i=0; i < box->count(); i++) {
        if (box->itemData(i).toBool()==val) {
           return i;
        }
    }
    return - 1;
}
