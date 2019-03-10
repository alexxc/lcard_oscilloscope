#ifndef TABLEPARAMMODELIFACE_H
#define TABLEPARAMMODELIFACE_H

class TableParamsModelIface {
public:
    /* обновление всех параметров указанного элемента */
    virtual void updateItem(int item_idx) = 0;
    /* обновление только указанного параметра указанного элемента */
    virtual void updateItemParam(int item_idx, int param_idx) = 0;
};

#endif // TABLEPARAMMODELIFACE_H

