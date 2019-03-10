#include "MultimeterThread.h"
#include <QMetaType>
#include <QElapsedTimer>
#include <QApplication>
#include "lqmeas/devs/Device.h"
#include "lqmeas/Error.h"
#include "lmath.h"
#include "MultimeterProcessor.h"
#include "lqmeas/log/Log.h"

namespace LQMeas {
    MultimeterThread::MultimeterThread(QObject *parent) :
        QThread(parent), m_cfgReq(false), m_stopReq(false), m_cur_id(0),
        m_curCfg(0), m_cfgSize(0), m_startStopMode(false) {

        m_proc = new MultimeterProcessor();




        m_proc->moveToThread(&m_procThread);

        qRegisterMetaType<LQMeas::MultimeterThread::MultimeterData>("LQMeas::MultimeterThread::MultimeterData");
        qRegisterMetaType<QVector<double> >("QVector<double>");
        connect(m_proc, SIGNAL(dataProcessed(LQMeas::MultimeterThread::MultimeterData)),
                SIGNAL(dataReceived(LQMeas::MultimeterThread::MultimeterData)),  Qt::DirectConnection);
        connect(this, SIGNAL(dataForProcess(LQMeas::DevAdc*,QVector<double>,LQMeas::MultimeterThread::MultimeterData)),
                m_proc, SLOT(processData(LQMeas::DevAdc*,QVector<double>,LQMeas::MultimeterThread::MultimeterData)));
    }

    void MultimeterThread::setDev(QSharedPointer<LQMeas::DevAdc> adc) {
        m_dev = qSharedPointerDynamicCast<Device>(adc);
        m_adc = adc.data();
    }

    void MultimeterThread::run() {
        bool out = false;
        Error err = Error::Success();
        int meas_time;
        int adc_ch_cnt = m_adc->adcChannelsCnt();
        int recv_size, ch_size;


        MultimeterData data;
        data.ch.resize(adc_ch_cnt);

        m_procThread.start();


        m_cfgReq = true;
        m_stopReq = false;
        m_err = Error::Success();

        LQMeasLog->info(tr("Measurement started"), m_dev.data());


        while (!out && err.isSuccess()) {
            m_lock.lock();
            /* проверка изменений настроек */
            if (m_cfgReq) {

                /* останавливаем сбор, если он был запущен */
                if (m_adc->adcIsRunning())
                    err = m_adc->adcStop();

                if (err.isSuccess()) {
                    LQMeasLog->info(tr("Device configuration update"), m_dev.data());

                    err = m_dev->configure(m_curCfg.data());

                    m_cfgReq = false;
                    meas_time = m_meas_time;
                    data.cfgID = m_cur_id;

                    if (err.isSuccess()) {
                        ch_size = meas_time*m_adc->adcFreq()/1000.;
    #ifndef LQMEAS_SERVICE_MULTIMETER_DC_ONLY
                        if (ch_size < 2)
                            ch_size = 2;
    #endif

                        recv_size = ch_size*m_adc->adcEnabledChCnt();
                    }
                }

                if (!m_startStopMode && err.isSuccess())
                    err = m_adc->adcStart();
            }

            if (m_stopReq) {
                out = true;
                m_stopReq = false;
            }

            m_lock.unlock();

            QVector<double> vals;
            vals.resize(recv_size);


            while (!out && err.isSuccess() && !restartReq()) {
                int tout = meas_time+4000;
                QElapsedTimer tmr;
                int recvd_size = 0;

                if (m_startStopMode)
                    err = m_adc->adcStart();

                tmr.start();

                while (err.isSuccess() && !restartReq() && (recvd_size!=recv_size) && !tmr.hasExpired(tout)) {
                    unsigned cur_recvd_size=0;
					// CHECKME: why operator[], not “at”?
					err = m_adc->adcGetData(&vals.data()[recvd_size], recv_size-recvd_size, 0,
                                         500, &cur_recvd_size);
					if (err.isSuccess())
                        recvd_size+=cur_recvd_size;
                }

                if (err.isSuccess() && !restartReq() && (recvd_size!=recv_size)) {
                    err = StdErrors::RecvInsufficientData();
                    LQMeasLog->error(tr("Data block receive timeout"), err);
                }

                if (m_startStopMode && err.isSuccess())
                    err = m_adc->adcStop();

                if (err.isSuccess() && !restartReq()) {
                    m_processDone = false;
                    /* раскладываем данные по каналам */
                    for (int ch=0; ch < adc_ch_cnt; ch++) {
                        data.ch[ch].status = m_adc->adcGetChStatus(ch, true);
                    }
                    emit dataForProcess(m_adc, vals, data);

                    if (m_startStopMode) {
                        /* ожидание подтверждения обработки данных */
                        while (!m_processDone && !m_stopReq) {
                            qApp->processEvents(QEventLoop::WaitForMoreEvents, 100);
                        }
                    }
                }


            }
        }

        if (m_adc->adcIsRunning()) {
            Error stop_err = m_adc->adcStop();
            if (err.isSuccess())
                err = stop_err;
        }

        m_err = err;

        m_procThread.quit();


        LQMeasLog->info(tr("Measurements stopped"), m_dev.data());
    }

    Error MultimeterThread::lastError() const {
        return m_err;
    }

    void MultimeterThread::setConfig(QSharedPointer<Device::Config> cfg, int meas_time, unsigned *id) {
        QMutexLocker locker(&m_lock);
        m_cfgReq = true;
        m_meas_time = meas_time;
        m_curCfg = cfg;
        m_cur_id++;
        if (id!=0)
            *id = m_cur_id;
    }

    void MultimeterThread::stopRequest() {
        QMutexLocker locker(&m_lock);
        m_stopReq = true;
    }
}



