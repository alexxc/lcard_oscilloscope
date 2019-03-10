#ifndef LQMEAS_DEVOUTASYNCDIG_H
#define LQMEAS_DEVOUTASYNCDIG_H

#include "lqmeas/Error.h"

namespace LQMeas {

    /* Интерфейс для асинхронного вывода на цифровые линии */
    class DevOutAsyncDig {
    public:
        Error outAsyncDigAll(unsigned val, unsigned mask=0);
    protected:
        virtual Error protOutAsyncDig(unsigned val, unsigned mask) = 0;
    };
}

#endif // LQMEAS_DEVOUTASYNCDIG_H
