#ifndef LQMEAS_E14_440_H
#define LQMEAS_E14_440_H

#include "lqmeas/devs/lcomp/lcompStreamDevice.h"

class lcompResolver;

namespace LQMeas {
    class e14_440: public lcompStreamDevice {
        Q_OBJECT
    public:
        static QString typeDeviceName() {return "E14-440";}

        QString name() const {return typeDeviceName();}
        QString modificationName() const;
        QString serial() const {return m_serial;}
        DeviceInterface iface() const {return InterfaceUSB;}
        QString firmwareName() {return "E440";}

    protected:


        Error lcompFillDescr(const void *descr);
    private:
        explicit e14_440(IDaqLDevice *dev, const __SLOT_PARAM *par, unsigned slot);

        friend class lcompResolver;

        QString m_serial;
        bool m_dac_present;
    };
}

#endif // LQMEAS_E14_440_H
