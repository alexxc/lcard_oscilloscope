#ifndef LQMEAS_DEVFRAMESENDER_H
#define LQMEAS_DEVFRAMESENDER_H

#include <QVector>
#include <string.h>
#include "lqmeas/Error.h"
#include "lqmeas/log/Log.h"

namespace LQMeas {
    template < class TypeDevice, class TypeWord> class DeviceFrameSender {
    public:
        DeviceFrameSender(TypeDevice *dev);

		Error putFrames(const TypeWord *frame, unsigned int size, unsigned tout, unsigned *put_size);
        Error flushData(unsigned tout) {
            return putFrames(0, 0, tout, 0);
        }

		void setFrameSize(unsigned int size);

        unsigned unsentWordsCnt() const {return m_last_wrds_cnt;}

    private:
        TypeDevice *m_dev;
        QVector<TypeWord>  m_last_wrds;
        unsigned m_last_wrds_cnt;
        unsigned m_frame_size;
    };

    template < class TypeDevice, class TypeWord>
        DeviceFrameSender<TypeDevice, TypeWord>::DeviceFrameSender(TypeDevice *dev) :
            m_dev(dev), m_frame_size(1), m_last_wrds_cnt(0) {
    }

    template < typename TypeDevice, typename TypeWord>
		Error DeviceFrameSender<TypeDevice, TypeWord>::putFrames(const TypeWord *frame, unsigned int size, unsigned tout, unsigned *put_size) {

        Error err = Error::Success();
        unsigned sent_size=0;

        /* если есть незавершенный остаток данных, то вначале пробуем послать его */
        if (m_last_wrds_cnt!=0) {
            err = m_dev->rawWordsSend(m_last_wrds.data(), m_last_wrds_cnt, tout, &sent_size);
            if (err.isSuccess()) {
                if ((sent_size > 0) && (sent_size < m_last_wrds_cnt)) {
                    memmove(m_last_wrds.data(), &m_last_wrds.data()[sent_size], sent_size*sizeof(m_last_wrds[0]));
                }
                m_last_wrds_cnt-=sent_size;
                sent_size = 0;
            }
        }

        /* если хвост послан успешно, то можно приступить к посылке новых данных */
        if (err.isSuccess() && (m_last_wrds_cnt==0) && (size!=0)) {
            err = m_dev->rawWordsSend(frame, size, tout, &sent_size);
            if (err.isSuccess()) {
                if (sent_size % m_frame_size) {
                    m_last_wrds_cnt = m_frame_size - sent_size % m_frame_size;
                    memmove(m_last_wrds.data(), &frame[sent_size], m_last_wrds_cnt*sizeof(m_last_wrds[0]));
                    sent_size += m_frame_size;
                }
            }
        }

        if (put_size)
            *put_size = 0;

        if (!err.isSuccess()) {
            LQMeasLog->error(LQMeas::Device::tr("Data send error"), err, m_dev);
        }

        return err;
    }

    template < typename TypeDevice, typename TypeWord>
		void DeviceFrameSender<TypeDevice, TypeWord>::setFrameSize(unsigned int size) {

        m_frame_size = size;
        m_last_wrds.resize(size);
        m_last_wrds_cnt = 0;
    }
}
#endif // LQMEAS_DEVFRAMESENDER_H
