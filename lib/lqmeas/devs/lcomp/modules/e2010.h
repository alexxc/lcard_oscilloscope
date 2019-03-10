#ifndef E2010_H
#define E2010_H

#include "lcompDevice.h"

class lcompResolver;

class E2010 : public lcompDevice {
    Q_OBJECT
public:
    static QString typeDeviceName();

private:
    explicit E2010(__SLOT_PARAM *par, unsigned slot);

    friend class lcompResolver;


};

#endif // E2010_H
