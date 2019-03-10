#include "DevOutSyncModeImplStream.h"
#include "../DevOutSync.h"
#include "DevOutSyncIfaceStream.h"
#include "lqmeas/StdErrors.h"
#include "lqmeas/devs/Device.h"
#include "DevOutSyncStreamThread.h"

namespace LQMeas {
    DevOutSyncModeImplStream::DevOutSyncModeImplStream(Device *dev, DevOutSyncIfaceStream *dev_stream) :
        m_dev(dev), m_stream_iface(dev_stream), m_thread(0) {

    }

    DevOutSyncModeImplStream::~DevOutSyncModeImplStream() {
        delete m_thread;
    }

    Error DevOutSyncModeImplStream::outSyncModeImplSigSet(QSharedPointer<OutRamSignalGenerator> ramSigGenerator) {
        double out_freq = m_dev->devOutSync()->outSyncConfig()->outSyncGenFreq();
        QList<QSharedPointer<OutSignal> > sigs = ramSigGenerator->allSignals();

        m_generator = ramSigGenerator;

        Q_FOREACH (QSharedPointer<OutSignal> sig, sigs) {
            if (sig->signalFreq()) {
                if (sig->periodStrictPoints()) {
                    unsigned pts = out_freq/sig->signalFreq() + 0.5;
                    if (pts < sig->minPoints())
                        pts = sig->minPoints();
                    sig->setRealParams(out_freq/pts, out_freq);
                } else {
                    sig->setRealParams(sig->signalFreq(), out_freq);
                }
            }
        }

        return Error::Success();
    }

    Error DevOutSyncModeImplStream::outSyncModeImplStartPrepare() {
        Error err = m_stream_iface->privOutStreamInit();
        if (err.isSuccess()) {
            QVector<double> dac_data;
            QVector<unsigned> dig_data;


            err = m_generator->genNextData(m_stream_iface->privOutStreamPreloadPoints(), dac_data, dig_data);

            if (err.isSuccess()) {
                err = m_stream_iface->privOutSyncSendData(
                            dac_data.data(), dac_data.size(),
                            dig_data.data(), dig_data.size(), 0);
            }
        }
        return err;
    }

    Error DevOutSyncModeImplStream::outSyncModeImplStart() {
        Error err = m_stream_iface->privOutStreamStart();
        if (err.isSuccess()) {
            if (m_thread)
                delete m_thread;
            m_thread = new DevOutSyncStreamThread(m_generator, m_dev, m_stream_iface);
            connect(m_thread, SIGNAL(underflowOccured()), this, SIGNAL(underflowOccured()));
            connect(m_thread, SIGNAL(errorOccured(LQMeas::Error)), this, SIGNAL(errorOccured(LQMeas::Error)));
            m_thread->start();
        }
        return err;
    }

    Error DevOutSyncModeImplStream::outSyncModeImplStopRequest(unsigned tout) {
        if (m_thread) {
            m_thread->setStopReq(tout);
        }
        return Error::Success();
    }

    Error DevOutSyncModeImplStream::outSyncModeImplStopWait() {
        Error err = Error::Success();
        if (m_thread) {
            m_thread->wait();
            err = m_thread->error();
            delete m_thread;
            m_thread = 0;
        }
        return err;
    }

    Error DevOutSyncModeImplStream::outSyncModeImplSigUpdate() {
        /* Пока не поддерживаем замену на лету */
        return StdErrors::OutGenRunning();
    }

}

