#ifndef LQMEAS_LCOMPSTREAMDEVICE_H
#define LQMEAS_LCOMPSTREAMDEVICE_H


#include "lcompDevice.h"
#include "lqmeas/ifaces/in/DevAdc.h"
#include "lcompRecvProcessorIface.h"
#include "lcompStreamReceiver.h"

namespace LQMeas {
    class lcompStreamDevice : public lcompDevice, public DevAdc, public lcompRecvProcessorIface {
        Q_OBJECT
    public:
        DevAdc *devAdc() {return static_cast<DevAdc *>(this);}

        Error adcGetData(double *data, unsigned size, unsigned flags,
                         unsigned tout, unsigned *recvd_size);

        static const unsigned lcomp_max_adc_channels = 32;
    protected:
        lcompStreamDevice(IDaqLDevice *dev, const __SLOT_PARAM *par, unsigned slot,
                          unsigned max_channels, DeviceConfig *defaultConfig, const DeviceInfo *info);

        Error protAdcStart();
        Error protAdcStop();
        virtual Error lcompFillAdcParams(void *par) const = 0;

        Error protConfigure(const DeviceConfig *cfg);

        virtual double adcCbrK(int ch, int range_num) const {return 1.;}
        virtual double adcCbrB(int ch, int range_num) const {return 0.;}
        virtual unsigned adcCodeMax() const {return 8000;}
        virtual unsigned adcCodeMin() const {return -8000;}
    private:
        Error lcompProcessRecvdData(const void *src_buf, unsigned size, double *dst_buf, unsigned flags, unsigned offs);

        lcompStreamReceiver m_adcReceiver;


        int m_cur_ch_idx;
        double m_ch_conv_k[lcomp_max_adc_channels];
        double m_ch_conv_b[lcomp_max_adc_channels];
    };
}

#endif // LQMEAS_LCOMPSTREAMDEVICE_H
