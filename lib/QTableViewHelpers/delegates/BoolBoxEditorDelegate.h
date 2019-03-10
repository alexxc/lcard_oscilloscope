#ifndef BOOLBOXEDITORDELEGATE_H
#define BOOLBOXEDITORDELEGATE_H

#include "ComboBoxEditorDelegate.h"

class BoolBoxEditorDelegate : public ComboBoxEditorDelegate {
    Q_OBJECT
public:
    BoolBoxEditorDelegate(QObject *parent = 0);


    static QString defaultValText(bool val) {return val ? tr("Yes") : tr("No");}
    virtual QString valText(bool val) const {return defaultValText(val);}
protected:
    void fillBoxItems(QComboBox *box, const QModelIndex &index) const;
    QVariant getBoxData(QComboBox *box, int idx) const;
    int getBoxIndexByData(QComboBox *box, const QModelIndex &index) const;
};

#endif // BOOLBOXEDITORDELEGATE_H
