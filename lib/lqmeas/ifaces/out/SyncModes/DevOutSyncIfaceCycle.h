#ifndef LQMEAS_DEVOUTSYNCIFACECYCLE_H
#define LQMEAS_DEVOUTSYNCIFACECYCLE_H

#include "DevOutSyncIfaceSend.h"

namespace LQMeas {
    class OutRamSignalGenerator;

    class DevOutSyncIfaceCycle : public virtual DevOutSyncIfaceSend {
    public:
        /* Максимально возможное количество точек на сигнал для циклической генерации */
        virtual size_t outCycleGenMaxSize(const OutRamSignalGenerator *generator) const {return 0; }

        virtual Error privOutCycleLoadStart(const OutRamSignalGenerator *generator, size_t size) = 0;
        virtual Error privOutCycleLoadFinish() = 0;
        virtual Error privOutCycleGenStart() = 0;
        virtual Error privOutCycleGenStopRequest(unsigned tout) = 0;
        virtual Error privOutCycleGenStop() = 0;
    };
}



#endif // DEVOUTSYNCIFACECYCLE_H
