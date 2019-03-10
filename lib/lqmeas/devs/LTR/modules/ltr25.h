#ifndef LQMEAS_LTR25_H
#define LQMEAS_LTR25_H

#include "LtrModule.h"
#include "ltr/include/ltr25api.h"
#include "lqmeas/ifaces/in/DevAdc.h"
#include "lqmeas/devs/DeviceFrameReceiver.h"

namespace LQMeas {
    class LTR25 : public LtrModule, public DevAdc {
        Q_OBJECT
    public:
        static QString typeModuleName();
        static const quint16 typeModuleID = LTR_MID_LTR25;

        QString name() const;
        QString serial() const;
        unsigned verFPGA() const;
        unsigned verPLD() const;
        unsigned boardRevision() const;
        bool industrial() const;


        /* ----------------------- интерфейс ввода  --------------------------*/
        DevAdc *devAdc() {return static_cast<DevAdc*>(this);}

        const double adcRangeMaxVal(int range) const;
        const double adcRangeMinVal(int range) const;
        unsigned adcRangesCnt() const;
        double adcFreqMax() const;


        static double typeAdcFreqVal(int idx);
        static unsigned typeAdcFreqsCnt();
        double adcFreqVal(int idx) const {return typeAdcFreqVal(idx); }
        unsigned adcFreqsCnt() const {return typeAdcFreqsCnt(); }


        double adcFreq() const;
        double adcChRange(int ch) const;

        QString errorString(int err) const;
        bool isOpened() const;

        bool adcIsRunning() const;
        bool adcChEnabled(int ch) const;


        void *rawHandle() {return &m_hnd;}
    signals:

    public slots:
        Error adcGetData(double *data, unsigned size, unsigned flags,
                       unsigned tout, unsigned *recvd_size);
    protected:
        TLTR* channel() const;

        Error protOpen();
        Error protClose();
        Error protConfigure();
        Error protLoadConfig(QSettings &set);
        Error protSaveConfig(QSettings &set) const;
        Error protAdcSetRawConfig(const void *cfg, size_t cfg_size);

        Error rawWordsReceive(DWORD *wrds, unsigned size, unsigned tout, unsigned *recvd_size);

        Error protAdcStart();
        Error protAdcStop();
    private:
        explicit LTR25(QSharedPointer<LtrCrate> crate, unsigned slot, QObject *parent = 0);

        mutable TLTR25 m_hnd;

        DeviceFrameReceiver<LTR25, DWORD> m_receiver;

        friend class DeviceFrameReceiver<LTR25, DWORD>;
        friend class LtrResolver;
    };
}

#endif // LQMEAS_LTR25_H
