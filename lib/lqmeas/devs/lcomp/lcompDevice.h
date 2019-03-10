#ifndef LCOMPDEVICE_H
#define LCOMPDEVICE_H

#include "lqmeas/devs/Device.h"
#include "lcompErrors.h"

struct __SLOT_PARAM;
struct IDaqLDevice;

namespace LQMeas {
    class lcompDevice : public Device {
        Q_OBJECT
    public:

        static QString firmwareDir();

        virtual QString firmwareName() { return QString(); }

        bool isOpened() const {return m_is_opened; }

        ~lcompDevice();

        IDaqLDevice *lcompIDaqIface() {return m_daqIface;}
    protected:
        Error protOpen();
        Error protClose();
        virtual Error lcompFillDescr(const void *descr) = 0;
        Error lcompDeviceStart();

        lcompDevice(IDaqLDevice *dev, const __SLOT_PARAM *par, unsigned slot,
                    DeviceConfig *defaultCfg, const DeviceInfo *info);



    private:
        bool m_is_opened;
        unsigned m_slot;
        unsigned m_board_type;
        IDaqLDevice *m_daqIface;
    };
}

#endif // LCOMPDEVICE_H
