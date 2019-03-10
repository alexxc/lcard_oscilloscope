#ifndef LQMEAS_DEVFRAMERECEIVER_H
#define LQMEAS_DEVFRAMERECEIVER_H


#include <QVector>
#include "Device.h"
#include "lqmeas/ifaces/in/DevAdc.h"
#include "lqmeas/log/Log.h"

namespace LQMeas {
    template < class TypeDevice, class TypeWord> class DeviceFrameReceiver {
    public:
        DeviceFrameReceiver(TypeDevice *dev);

        Error getFrames(int size, unsigned tout, TypeWord **frame, unsigned *recvd_size);
		void setFrameSize(int size);

    private:
        TypeDevice *m_dev;
        QVector<TypeWord>  m_last_wrds;
        int m_last_wrds_cnt;
        int m_frame_size;
        int m_last_wrds_offs;
    };

    template < class TypeDevice, class TypeWord>
        DeviceFrameReceiver<TypeDevice, TypeWord>::DeviceFrameReceiver(TypeDevice *dev) :
            m_dev(dev), m_frame_size(1), m_last_wrds_cnt(0), m_last_wrds_offs(0) {
    }

    template < typename TypeDevice, typename TypeWord>
        Error DeviceFrameReceiver<TypeDevice, TypeWord>::getFrames(int size, unsigned tout, TypeWord **frame, unsigned *recvd_size) {

        Error err;
        unsigned recvd;
        if (size > m_last_wrds.size())
            m_last_wrds.resize(size);

        if ((m_last_wrds_cnt!=0) && (m_last_wrds_offs!=0))  {
            memmove(m_last_wrds.data(), &m_last_wrds.data()[m_last_wrds_offs],
                    m_last_wrds_cnt*sizeof(m_last_wrds[0]));
        }

        err = m_dev->rawWordsReceive(&m_last_wrds.data()[m_last_wrds_cnt],
                                     size - m_last_wrds_cnt, tout, &recvd);
        if (err.isSuccess()) {
            unsigned tail_size;
            recvd += m_last_wrds_cnt;

            tail_size = recvd % m_frame_size;
            recvd -= tail_size;

            if (recvd_size!=0) {
                *recvd_size = recvd;
            }

            if (recvd > 0) {
                unsigned ch_cnt = m_dev->devTypeInfo()->adc()->adcChannelsCnt();
                for (unsigned ch = 0; ch < ch_cnt; ch++) {
                    if (m_dev->adcConfig()->adcChEnabled(ch)) {
                        m_dev->adcSetChStatus(ch, DevAdc::ChStatusOk);
                    }
                }
            }

            *frame = m_last_wrds.data();
            m_last_wrds_cnt = tail_size;
            m_last_wrds_offs = recvd;


        } else {
            LQMeasLog->error(Device::tr("Data receive error"), err, m_dev);
        }

        return err;
    }

    template < typename TypeDevice, typename TypeWord>
		void DeviceFrameReceiver<TypeDevice, TypeWord>::setFrameSize(int size) {

        m_frame_size = size;
        m_last_wrds_cnt = 0;
        m_last_wrds_offs = 0;
    }
}
#endif // LQMEAS_DEVFRAMERECEIVER_H
