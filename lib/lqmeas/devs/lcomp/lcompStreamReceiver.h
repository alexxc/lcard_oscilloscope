#ifndef LQMEAS_LCOMPSTREAMRECEIVER_H
#define LQMEAS_LCOMPSTREAMRECEIVER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "lqmeas/Error.h"

struct IDaqLDevice;
struct _DAQ_PARAM_;

namespace LQMeas {
    class lcompRecvProcessorIface;

    class lcompStreamReceiver : public QThread {
        Q_OBJECT
    public:
        explicit lcompStreamReceiver(QObject *parent = 0);

        Error setStreamParams(IDaqLDevice *idev, _DAQ_PARAM_ *sp, unsigned req_buf_size,
                                    unsigned ch_cnt, unsigned StreamId);



        void dropData(unsigned size);

        void prestartInit();
        void stop();

        Error getData(lcompRecvProcessorIface *iproc, double *data, unsigned size, unsigned flags,
                            unsigned tout, unsigned *recvd_size);



    protected:
        void run ();

    private:


        unsigned m_driver_buf_size;
        unsigned m_point_size;
        unsigned m_ch_cnt;



        quint8 *m_driver_buf;
        volatile unsigned   *m_sync;
        quint8 *m_buf;
        unsigned m_buf_size;
        volatile unsigned m_buf_fill_size;
        unsigned m_buf_get_pos;

        volatile int m_stop_req;
        Error m_err;

        QMutex m_lock;
        QWaitCondition m_wait;

    };
}

#endif // LQMEAS_LCOMPSTREAMRECEIVER_H
