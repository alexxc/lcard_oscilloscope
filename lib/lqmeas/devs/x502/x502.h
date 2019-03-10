#ifndef LQMEAS_X502_H
#define LQMEAS_X502_H

#include "x502api.h"
#include "lqmeas/devs/Device.h"
#include "lqmeas/devs/DeviceFrameReceiver.h"
#include "lqmeas/devs/DeviceFrameSender.h"
#include "lqmeas/ifaces/in/DevAdc.h"
#include "lqmeas/ifaces/in/DevInAsyncDig.h"
#include "lqmeas/ifaces/out/DevOutSync.h"
#include "lqmeas/ifaces/out/DevOutAsyncDac.h"
#include "lqmeas/ifaces/out/DevOutAsyncDig.h"

#include "lqmeas/ifaces/out/SyncModes/DevOutSyncIfaceStream.h"
#include "lqmeas/ifaces/out/SyncModes/DevOutSyncIfaceCycle.h"

#include <QMutex>



namespace LQMeas {
    class x502Info;
    class x502Config;

    class x502 : public Device, public DevAdc, public DevOutSync,
            public DevOutAsyncDac, public DevOutAsyncDig, public DevInAsyncDig,
            DevOutSyncIfaceStream, DevOutSyncIfaceCycle {
        Q_OBJECT
    public:
        ~x502();

        QString location() const;
        DeviceInterface iface() const;

        bool isOpened() const;

        bool supportCustomOutFreq() const;


        /* ----------------------- интерфейс ввода  --------------------------*/
        DevAdc *devAdc() {return static_cast<DevAdc*>(this);}
        DevInAsyncDig *devInAsyncDig() {return static_cast<DevInAsyncDig*>(this);}


        bool adcIsRunning() const;

        Error adcEnable();
        Error adcDisable();


		Error adcGetData(double *data, unsigned size, unsigned flags,
							   unsigned tout, unsigned *recvd_size);



        /* ------------------- интерфейс вывода --------------------------------*/        
        DevOutSync *devOutSync() {return static_cast<DevOutSync*>(this);}
        DevOutAsyncDac *devOutAsyncDac() {return static_cast<DevOutAsyncDac*>(this);}
        DevOutAsyncDig *devOutAsyncDig() {return static_cast<DevOutAsyncDig*>(this);}

        /* ------------------- дополнительные функции --------------------------*/
        static Error error(int err, QString descr = QString());

        t_x502_hnd devHandle() const {return m_hnd;}

		const x502Config *devSpecConfig() const;
		const x502Info *devSpecInfo() const;
    protected:
		x502(const t_x502_devrec *devRec, const x502Info *info);


        Error protOpen();
        Error protClose();
        Error protConfigure(const DeviceConfig *cfg);

        Error protAdcStart();
        Error protAdcStop();

        Error protInAsyncDig(unsigned *val);

		Error protOutAsyncDac(unsigned int ch, double val);
        Error protOutAsyncDig(unsigned val, unsigned mask);

        Error rawWordsSend(const uint32_t *wrds, unsigned size, unsigned tout, unsigned *sent_size);

        virtual DevOutSyncConfig *outSyncConfigMutable() const;
        virtual const x502Info *createInfo(const t_x502_info *info) const = 0;
    private:
        Error privOutStreamInit();
        Error privOutStreamStart();
        Error privOutStreamStop(unsigned tout);
        unsigned privOutStreamPreloadPoints();

        Error privOutSyncSendData(const double *dac_data, unsigned dac_size,
                                  const unsigned *dig_data, unsigned dig_size, unsigned flags, unsigned tout = 0);
        Error privOutSyncFlushData(unsigned tout);

        bool privOutSyncUnderflowOccured() const;

        Error privOutCycleLoadStart(const OutRamSignalGenerator *generator, size_t size);
        Error privOutCycleLoadFinish();
        Error privOutCycleGenStart();
        Error privOutCycleGenStopRequest(unsigned tout);
        Error privOutCycleGenStop();


        Error rawWordsReceive(uint32_t *wrds, unsigned size, unsigned tout, unsigned *recvd_size);
        Error setOutStreams();
        void setNewDevrecord(const t_x502_devrec *devrec);

        x502Config *mutableSpecConfig() const;



        DeviceFrameReceiver<x502, uint32_t> m_receiver;
        DeviceFrameSender<x502, uint32_t> m_sender;

        t_x502_devrec m_devrec;
        t_x502_hnd m_hnd;
        bool m_opened;

        uint32_t m_outStreams;
        bool     m_outRunning;
        uint32_t m_inRunning;

        QMutex m_streamLock;

        friend class DeviceFrameReceiver<x502, uint32_t>;
        friend class DeviceFrameSender<x502, uint32_t>;
        friend class x502Resolver;

    };
}

#endif // LQMEAS_X502_H
