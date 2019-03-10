#ifndef LQMEAS_LTR11_H
#define LQMEAS_LTR11_H



#include "ltr/include/ltr11api.h"
#include "lqmeas/devs/LTR/modules/LTRModule.h"
#include "lqmeas/ifaces/in/DevAdc.h"
#include "lqmeas/devs/DeviceFrameReceiver.h"


namespace LQMeas {
    class LTR11Config;
    class LTR11TypeInfo;

    class LTR11 : public LTRModule, public DevAdc {
        Q_OBJECT
    public:
        static const quint16 typeModuleID = LTR_MID_LTR11;

        bool isOpened() const;
        /* ----------------------- интерфейс ввода  --------------------------*/
        DevAdc *devAdc() {return static_cast<DevAdc*>(this);}

        QString errorString(int err) const;

        bool adcIsRunning() const {return m_run;}


        Error adcGetData(double *data, unsigned size, unsigned flags,
                         unsigned tout, unsigned *recvd_size);

        const LTR11Config *devspecConfig() const;
    protected:
        TLTR* channel() const;

        Error protOpen();
        Error protClose();
        Error protConfigure(const DeviceConfig *cfg);

        Error rawWordsReceive(DWORD *wrds, unsigned size, unsigned tout, unsigned *recvd_size);

        Error protAdcStart();
        Error protAdcStop();
    private:
        explicit LTR11(QSharedPointer<LTRCrate> crate, unsigned slot, QObject *parent = 0);
        mutable TLTR11 m_hnd;
        DeviceFrameReceiver<LTR11, DWORD> m_receiver;

        friend class DeviceFrameReceiver<LTR11, DWORD>;
        friend class LTRResolver;

        bool m_run;
    };
}

#endif // LQMEAS_LTR11_H
