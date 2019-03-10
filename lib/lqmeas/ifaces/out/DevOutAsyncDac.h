#ifndef LQMEAS_DEVOUTASYNCDAC_H
#define LQMEAS_DEVOUTASYNCDAC_H

#include "lqmeas/Error.h"


namespace LQMeas {

    /* Интерфейс для асинхронного вывода на ЦАП */
    class DevOutAsyncDac {
    public:
        Error outAsyncDac(int ch, double val);
    protected:
		virtual Error protOutAsyncDac(unsigned int ch, double val) = 0;
    };
}

#endif // LQMEAS_DEVOUTASYNCDAC_H
