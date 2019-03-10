#ifndef LQMEAS_LTR51_H
#define LQMEAS_LTR51_H

#include "LtrModule.h"
#include "LtrMezzanine.h"
#include "ltr/include/ltr51api.h"
#include "lqmeas/ifaces/in/DevAdc.h"
#include "lqmeas/devs/DeviceFrameReceiver.h"


namespace LQMeas {
    class LTR51_Mezzanine : public LtrMezzanine {
    private:
        LTR51_Mezzanine(const TLTR51_MEZZANINE_INFO *info);
        Unit unit() const {return Units::Hz();}
        friend class LTR51;
    };

    class LTR51 : public LtrModule, public DevAdc {
        Q_OBJECT
    public:
        static QString typeModuleName();
        static const quint16 typeModuleID = LTR_MID_LTR51;
        static const unsigned typeMezzanineCnt = LTR51_MEZZANINE_CNT;


        struct AdcConfig {
            struct {
                bool enabled;
                BYTE thresholdRange;
                BYTE freqRange;
                BYTE edge;
                double lowThreshold;
                double highThreshold;
            } Ch[LTR51_CHANNEL_CNT];
            int acqTime;
            bool userBaseFs;
            unsigned base;
            double fs;
        };

        QString name() const;
        QString serial() const;
        QString modificationName() const;
        QString verFPGA() const;
        QString verFirmMCU() const;


        /* ----------------------- интерфейс ввода  --------------------------*/
        DevAdc *devAdc() {return static_cast<DevAdc*>(this);}

        double adcFreq() const;
        double adcChRange(int ch) const;


        QString errorString(int err) const;
        bool isOpened() const;

        bool adcChEnabled(int ch) const;

        const double adcRangeMaxVal(int range) const {return 250000;}
        const double adcRangeMinVal(int range) const {return 0; }
        unsigned adcRangesCnt() const {return 1;}
        double adcFreqMax() const {return 250000;}

        unsigned mezzanineCnt() {return m_mezzaninesList.size();}
        QSharedPointer<LtrMezzanine> mezzanine(unsigned slot);

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
        Error protAdcSetRawConfig(const void *rawCfg, size_t cfg_size);

        Error rawWordsReceive(DWORD *wrds, unsigned size, unsigned tout, unsigned *recvd_size);

        Error protAdcStart();
        Error protAdcStop();
    private:
		explicit LTR51(QSharedPointer<LtrCrate> crate, unsigned slot, QObject *parent = nullptr);

        mutable TLTR51 m_hnd;
        AdcConfig m_cfg;

        DeviceFrameReceiver<LTR51, DWORD> m_receiver;

        QVector<QSharedPointer<LTR51_Mezzanine> > m_mezzaninesList;

        friend class DeviceFrameReceiver<LTR51, DWORD>;
        friend class LtrResolver;
    };
}

#endif // LQMEAS_LTR51_H
