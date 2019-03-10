#ifndef LQMEAS_E14_140_H
#define E14_140_H

#include "lqmeas/devs/lcomp/lcompStreamDevice.h"

namespace LQMeas {
    class E14_140Config;
    class E14_140Info;

    class E14_140: public lcompStreamDevice {
        Q_OBJECT
    public:
        DeviceInterface iface() const {return InterfaceUSB;}

        const E14_140Config *devspecConfig() const;
        const E14_140Info *devspecInfo() const;

        static const unsigned adc_ranges_cnt = 4;
    protected:
        Error lcompFillDescr(const void *descr);
        Error lcompFillAdcParams(void *par) const;
        Error protConfigure(const DeviceConfig *cfg);

        double adcCbrK(int ch, int range_num) const {return m_cbr_k[range_num];}
        double adcCbrB(int ch, int range_num) const {return m_cbr_b[range_num];}
    private:
        explicit E14_140(IDaqLDevice *dev, const __SLOT_PARAM *par, unsigned slot);

        friend class lcompResolver;

        double m_cbr_k[adc_ranges_cnt];
        double m_cbr_b[adc_ranges_cnt];
    };
}


#endif // E14_140_H
