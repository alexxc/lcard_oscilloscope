#ifndef LQMEAS_DEVOUTSYNCIFACESTREAM_H
#define LQMEAS_DEVOUTSYNCIFACESTREAM_H

#include "DevOutSyncIfaceSend.h"

namespace LQMeas {
    class DevOutSyncIfaceStream : public virtual DevOutSyncIfaceSend {
    public:
        virtual Error privOutStreamInit() = 0;
        virtual Error privOutStreamStart() = 0;
        virtual Error privOutStreamStop(unsigned tout) = 0;
        virtual unsigned privOutStreamPreloadPoints() = 0;
    };
}

#endif // LQMEAS_DEVOUTSYNCIFACESTREAM_H
