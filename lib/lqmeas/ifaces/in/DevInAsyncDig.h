#ifndef LQMEAS_DEVINASYNCDIG_H
#define LQMEAS_DEVINASYNCDIG_H

#include "lqmeas/Error.h"

namespace LQMeas {
    class DevInAsyncDig {
    public:
        Error inAsyncDig(unsigned *val);
    protected:
        virtual Error protInAsyncDig(unsigned *val) = 0;
    };
}

#endif // LQMEAS_DEVINASYNCDIG_H
