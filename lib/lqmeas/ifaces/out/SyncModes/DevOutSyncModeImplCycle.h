#ifndef LQMEAS_DEVOUTSYNCMODEIMPLCYCLE_H
#define LQMEAS_DEVOUTSYNCMODEIMPLCYCLE_H


#include "DevOutSyncModeImpl.h"


namespace LQMeas {
    class Device;
    class DevOutSyncIfaceCycle;

    class DevOutSyncModeImplCycle : public DevOutSyncModeImpl {
        Q_OBJECT
    public:
        DevOutSyncModeImplCycle(Device *dev, DevOutSyncIfaceCycle *dev_cycle);


        DevOutSyncConfig::OutSyncRamMode outSyncMode() {return DevOutSyncConfig::OutSyncRamModeCycle;}
        Error outSyncModeImplSigSet(QSharedPointer<OutRamSignalGenerator> ramSigGenerator);
        Error outSyncModeImplStartPrepare();
        Error outSyncModeImplStart();
        Error outSyncModeImplStopRequest(unsigned tout);
        Error outSyncModeImplStopWait();
        Error outSyncModeImplSigUpdate();

    private:
        Device *m_dev;
        DevOutSyncIfaceCycle *m_iface;
        const double m_cycle_freq_rel_err;


        QSharedPointer<OutRamSignalGenerator> m_generator;
        size_t m_sig_size;

        Error outCalcCycleSigsParams(OutRamSignalGenerator *ramSigGenerator,
                                     double out_freq, unsigned max_sig_size,
                                     size_t *res_size);
    };
}

#endif // LQMEAS_DEVOUTSYNCMODEIMPLCYCLE_H
