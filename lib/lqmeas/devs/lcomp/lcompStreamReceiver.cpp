#include "lcompStreamReceiver.h"
#include "lcomp_headers.h"
#include "lqmeas/log/Log.h"
#include "lcompErrors.h"
#include "lcompRecvProcessorIface.h"
#include <string.h>
#include <QElapsedTimer>


namespace LQMeas {
    lcompStreamReceiver::lcompStreamReceiver(QObject *parent) :
        QThread(parent), m_buf_fill_size(0), m_buf_get_pos(0) {
    }

    Error lcompStreamReceiver::setStreamParams(IDaqLDevice *idev, _DAQ_PARAM_ *sp,
                                                     unsigned req_buf_size,
                                                     unsigned ch_cnt, unsigned StreamId) {
        Error err = Error::Success();
        m_ch_cnt = ch_cnt;
        ULONG pt_size, buf_size=req_buf_size;
#ifdef L_POINT_SIZE
        idev->GetParameter(L_POINT_SIZE, &pt_size);
#else
        /** @todo определение по имени устройства */
        pt_size = 2;
#endif

        if (idev->RequestBufferStream(&buf_size, L_STREAM_ADC) != L_SUCCESS) {
            err = lcompErrors::AdcReqBuffer();
        } else  if (idev->SetParametersStream(sp, &buf_size, (void**)&m_driver_buf, (void**)&m_sync, StreamId) != L_SUCCESS) {
            err = lcompErrors::AdcSetParams();
        } else {
            m_point_size = pt_size;
            m_driver_buf_size = buf_size;
        }
        return err;
    }

    void lcompStreamReceiver::dropData(unsigned size) {
        QMutexLocker lock(&m_lock);
        if (size > m_buf_fill_size)
            size = m_buf_fill_size;

        m_buf_fill_size -= size;
        m_buf_get_pos += size;
        if (m_buf_get_pos >= m_buf_size) {
            m_buf_get_pos -= m_buf_size;
        }
    }



    void lcompStreamReceiver::stop() {
        m_stop_req = true;
    }

    Error lcompStreamReceiver::getData(lcompRecvProcessorIface *iproc, double *data, unsigned size,
                                     unsigned flags, unsigned tout, unsigned *recvd_size) {

        QElapsedTimer timer;

        unsigned rdy_size = 0;
        unsigned proc_size = 0;
        int rem_tout = (int)tout;
        Error err = m_err;

        timer.start();


        while (err.isSuccess() && (rdy_size < size) && (rem_tout!=0)) {
            m_lock.lock();
            rdy_size = m_buf_fill_size;
            if (rdy_size < size) {
                if (m_wait.wait(&m_lock, rem_tout))
                    rdy_size = m_buf_fill_size;
            }
            err = m_err;
            m_lock.unlock();

            rem_tout = tout - timer.elapsed();
            if (rem_tout < 0)
                rem_tout = 0;

        }

        if (err.isSuccess() && (rdy_size!=0)) {
            if (rdy_size > size) {
                rdy_size = size;
            }

            rdy_size -= (rdy_size % m_ch_cnt);

            while (rdy_size && err.isSuccess()) {
                unsigned cur_proc = m_buf_size - m_buf_get_pos;
                if (cur_proc > rdy_size)
                    cur_proc = rdy_size;

                err = iproc->lcompProcessRecvdData(&m_buf[m_buf_get_pos*m_point_size], cur_proc,
                                                   &data[proc_size], flags,proc_size);
                if (err.isSuccess()) {
                    proc_size+=cur_proc;
                    rdy_size-=cur_proc;
                    dropData(cur_proc);
                }
            }


        }

        if (recvd_size) {
            *recvd_size = proc_size;
        }


        return err;
    }

    void lcompStreamReceiver::prestartInit() {
        m_err = Error::Success();
        m_buf_get_pos = 0;

        m_buf_size = 10000000;
        m_buf_fill_size = 0;
        m_stop_req = false;
    }


    void lcompStreamReceiver::run() {
        LONG s, prev_s=0;
        unsigned put_pos = 0;

        m_buf = new quint8[m_point_size*m_buf_size];

        while (!m_stop_req && m_err.isSuccess()) {
            s = *m_sync;
            if (s == m_driver_buf_size)
                s = 0;

            if (s!=prev_s) {
                unsigned rdy = s >= prev_s ? s - prev_s : m_driver_buf_size - (prev_s - s);
                unsigned free_size = m_buf_size - m_buf_fill_size;
                unsigned out = s < prev_s;

                while (rdy && free_size) {
                    unsigned move_size = m_buf_size - put_pos;
                    if (move_size > (m_driver_buf_size - prev_s))
                        move_size = m_driver_buf_size - prev_s;
                    if (move_size > rdy)
                        move_size = rdy;
                    if (move_size > free_size)
                        move_size = free_size;

                    memcpy(&m_buf[put_pos*m_point_size], &m_driver_buf[prev_s*m_point_size], move_size*m_point_size);

                    put_pos+=move_size;
                    if (put_pos == m_buf_size)
                        put_pos = 0;
                    prev_s += move_size;
                    if (prev_s == m_driver_buf_size)
                        prev_s =  0;

                    rdy-=move_size;
                    free_size-=move_size;

                    QMutexLocker lock(&m_lock);
                    m_buf_fill_size+=move_size;
                    m_wait.wakeAll();
                }

                if ((free_size==0) && (rdy > m_driver_buf_size/2)) {
                    LQMeasLog->error("Receive error!", lcompErrors::RecvBufOverflow());
                    m_err = lcompErrors::RecvBufOverflow();
                    m_wait.wakeAll();
                }
            }

            msleep(20);
        }

        delete [] m_buf;
    }
}
