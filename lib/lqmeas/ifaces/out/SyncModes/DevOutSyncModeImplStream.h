#ifndef LQMEAS_DEVOUTSYNCMODEIMPLSTREAM_H
#define LQMEAS_DEVOUTSYNCMODEIMPLSTREAM_H

#include "DevOutSyncModeImpl.h"


namespace LQMeas {
    class Device;
    class DevOutSyncIfaceStream;
    class DevOutSyncStreamThread;

    class DevOutSyncModeImplStream : public DevOutSyncModeImpl {
        Q_OBJECT
    public:
        DevOutSyncModeImplStream(Device *dev, DevOutSyncIfaceStream *dev_stream);
        ~DevOutSyncModeImplStream();

        DevOutSyncConfig::OutSyncRamMode outSyncMode() {return DevOutSyncConfig::OutSyncRamModeStream;}
        Error outSyncModeImplSigSet(QSharedPointer<OutRamSignalGenerator> ramSigGenerator);
        Error outSyncModeImplStartPrepare();
        Error outSyncModeImplStart();
        Error outSyncModeImplStopRequest(unsigned tout);
        Error outSyncModeImplStopWait();
        Error outSyncModeImplSigUpdate();

    private:
        Device *m_dev;
        DevOutSyncIfaceStream *m_stream_iface;
        DevOutSyncStreamThread *m_thread;

        QSharedPointer<OutRamSignalGenerator> m_generator;
    };
}

#endif // LQMEAS_DEVOUTSYNCMODEIMPLSTREAM_H
