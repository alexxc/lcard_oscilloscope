#ifndef LQMEAS_DEVOUTSYNCSTREAMTHREAD_H
#define LQMEAS_DEVOUTSYNCSTREAMTHREAD_H

#include "../OutRamSignalGenerator.h"
#include <QThread>

namespace LQMeas {

    class DevOutSyncIfaceStream;
    class Device;

    class DevOutSyncStreamThread  : public QThread {
        Q_OBJECT
    public:
        explicit DevOutSyncStreamThread(QSharedPointer<OutRamSignalGenerator> generator, Device *dev, DevOutSyncIfaceStream *stream_iface);

        void setStopReq(unsigned tout) {m_stop_req = true; m_stop_tout = tout;}

        Error error() const {return m_err;}
    Q_SIGNALS:
        void underflowOccured();
        void errorOccured(LQMeas::Error err);
    protected:
        void run();


    private:
        bool m_stop_req;
        unsigned m_stop_tout;
        Error m_err;

        DevOutSyncIfaceStream *m_stream;
        Device *m_dev;
        QSharedPointer<OutRamSignalGenerator> m_generator;

        static const unsigned out_block_time_ms = 1000;

    };
}

#endif // LQMEAS_DEVOUTSYNCSTREAMTHREAD_H
