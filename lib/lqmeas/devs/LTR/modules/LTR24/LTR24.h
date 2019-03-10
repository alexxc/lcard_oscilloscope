#ifndef LQMEAS_LTR24_H
#define LQMEAS_LTR24_H

#include "ltr/include/ltr24api.h"
#include "lqmeas/devs/LTR/modules/LTRModule.h"
#include "lqmeas/ifaces/in/DevAdc.h"
#include "lqmeas/devs/DeviceFrameReceiver.h"

namespace LQMeas {
    class LTR24Config;
    class LTR24Info;

    class LTR24 : public LTRModule, public DevAdc {
        Q_OBJECT
    public:
        static const quint16 typeModuleID = LTR_MID_LTR24;

        bool isOpened() const;

        /* ----------------------- интерфейс ввода  --------------------------*/
        DevAdc *devAdc() {return static_cast<DevAdc*>(this);}
        QString errorString(int err) const;

        bool adcIsRunning() const;

        Error adcGetData(double *data, unsigned size, unsigned flags,
                         unsigned tout, unsigned *recvd_size);

        const LTR24Config *devspecConfig() const;
        const LTR24Info *devspecInfo() const;
    protected:
        TLTR* channel() const;

        Error protOpen();
        Error protClose();
        Error protConfigure(const DeviceConfig *cfg);

        Error rawWordsReceive(DWORD *wrds, unsigned size, unsigned tout, unsigned *recvd_size);

        Error protAdcStart();
        Error protAdcStop();


    private:
        explicit LTR24(QSharedPointer<LTRCrate> crate, unsigned slot, QObject *parent = 0);
        mutable TLTR24 m_hnd;

        DeviceFrameReceiver<LTR24, DWORD> m_receiver;

        friend class DeviceFrameReceiver<LTR24, DWORD>;
        friend class LTRResolver;

    };
}

#endif // LQMEAS_LTR24_H
