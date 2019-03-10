#ifndef LQMEAS_DEVOUTSYNCIFACESEND_H
#define LQMEAS_DEVOUTSYNCIFACESEND_H

#include "lqmeas/Error.h"

namespace LQMeas {
    class DevOutSyncIfaceSend {
    public:
        virtual Error privOutSyncSendData(const double *dac_data, unsigned dac_size,
                               const unsigned *dig_data, unsigned dig_size, unsigned flags, unsigned tout = 0) = 0;
        virtual Error privOutSyncFlushData(unsigned tout) = 0;

        virtual bool privOutSyncUnderflowOccured() const = 0;
    };
}

#endif // LQMEAS_DEVOUTSYNCIFACESEND_H
