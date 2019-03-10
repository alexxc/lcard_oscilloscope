#ifndef MULTIMETERTHREAD_H
#define MULTIMETERTHREAD_H

#include <QThread>
#include <QVector>
#include <QMutex>
#include <QSharedPointer>
#include "lqmeas_config.h"

#include "lqmeas/devs/Device.h"
#include "lqmeas/ifaces/in/DevAdc.h"


namespace LQMeas {
    class MultimeterProcessor;

    class MultimeterThread : public QThread {
        Q_OBJECT
    public:

        struct MultimeterChData {
            DevAdc::ChannelStatus status;
            double DC;
    #ifndef LQMEAS_SERVICE_MULTIMETER_DC_ONLY
            double AC;
            double freq;
            double SNR;
            double SINAD;
    #endif
        };

        struct MultimeterData {
            int cfgID;
            QVector<MultimeterChData> ch;
        };

		explicit MultimeterThread(QObject *parent = nullptr);

        void setDev(QSharedPointer<DevAdc> adc);

        void run();
        Error lastError() const;

        /* Включение старт-стопного режима. В этом режиме по приему блока
         * поток генерирует сигнал dataForProcess() с обработанными данными
         * и останавливает сбор до вызова blockProcessDone() */
        void setStartStopMode(bool en) {m_startStopMode = en;}
    signals:
        void dataReceived(LQMeas::MultimeterThread::MultimeterData data);
        void dataForProcess(LQMeas::DevAdc *adc, QVector<double> vals, LQMeas::MultimeterThread::MultimeterData data);

    public slots:
        void setConfig(QSharedPointer<Device::Config> cfg, int meas_time, unsigned *id);
        void stopRequest();
        void blockProcessDone() {m_processDone = true;}
    private:
        bool restartReq() {return m_stopReq || m_cfgReq;}
        mutable QMutex m_lock;
        Error m_err;
        QSharedPointer<Device>  m_dev;
        DevAdc *m_adc;
        QSharedPointer<Device::Config> m_curCfg;
        size_t m_cfgSize;

        int m_meas_time;
        bool m_startStopMode;
        bool m_processDone;
        bool m_stopReq;
        bool m_cfgReq;
        int m_cur_id;
        MultimeterProcessor *m_proc;
        QThread m_procThread;

    };
}

#endif // MULTIMETERTHREAD_H
