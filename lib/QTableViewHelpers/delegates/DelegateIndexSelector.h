#ifndef DELEGATEINDEXSELECTOR_H
#define DELEGATEINDEXSELECTOR_H

class QModelIndex;

class DelegateIndexSelector {
public:
    virtual bool delegatePaintEnabled(const QModelIndex &index) = 0;
};

#endif // DELEGATEPAINTINDEXSELECTOR_H
