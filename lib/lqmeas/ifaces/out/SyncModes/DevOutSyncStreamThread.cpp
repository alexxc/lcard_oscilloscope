#include "DevOutSyncStreamThread.h"
#include "lqmeas/devs/Device.h"
#include "lqmeas/ifaces/out/DevOutSync.h"
#include "DevOutSyncIfaceStream.h"
#include "lqmeas/log/Log.h"


namespace LQMeas {
    DevOutSyncStreamThread::DevOutSyncStreamThread(QSharedPointer<LQMeas::OutRamSignalGenerator> generator, Device *dev,
                                                   LQMeas::DevOutSyncIfaceStream *stream_iface) :
		m_stop_req(false), m_stop_tout(30000), m_stream(stream_iface), m_dev(dev), m_generator(generator) {

    }

    void DevOutSyncStreamThread::run() {
        Error err = Error::Success();
        while (err.isSuccess() && !m_stop_req) {
            unsigned gen_points = m_dev->devOutSync()->outSyncConfig()->outSyncGenFreq()*1000/out_block_time_ms;
            QVector<double> dac_data;
            QVector<unsigned> dig_data;
            bool send_done = false;

            err = m_generator->genNextData(gen_points, dac_data, dig_data);

            if (err.isSuccess() && !m_stop_req && !send_done) {
                err = m_stream->privOutSyncSendData(dac_data.data(), dac_data.size(),
                                                    dig_data.data(), dig_data.size(), 0);
                if (err == StdErrors::SendBusy()) {
                    err = Error::Success();
                } else if (err.isSuccess()) {
                    send_done = true;
                    /* проверяем опустошение буфера и сигнализируем сообщением
                       о данном событии, если оно произошло */
                    if (m_stream->privOutSyncUnderflowOccured()) {
                        Q_EMIT underflowOccured();
                    }
                }
            }
        }

        Error stop_err = m_stream->privOutStreamStop(m_stop_tout);
        if (err.isSuccess())
            err = stop_err;

        m_err = err;


        if (err.isSuccess()) {
            LQMeasLog->detail(tr("Output data stream stopped successfully"), m_dev);
        } else {
            LQMeasLog->error(tr("Output data stream stop error"), err, m_dev);
            Q_EMIT errorOccured(err);
        }
    }
}
