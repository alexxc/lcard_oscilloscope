#ifndef LQMEAS_LTR35_H
#define LQMEAS_LTR35_H

#include "LtrModule.h"
#include "lqmeas/ifaces/out/DevOutStream.h"
#include "lqmeas/devs/DeviceFrameSender.h"

#include "ltr/include/ltr35api.h"

namespace LQMeas {
    class LTR35 : public LtrModule, public DevOutStream {
        Q_OBJECT
    public:
        static QString typeModuleName();
        static const quint16 typeModuleID = LTR_MID_LTR35;


        ~LTR35();

        QString name() const;
        QString modificationName() const;
        QString serial() const;
        unsigned verFPGA() const;
        unsigned verPLD() const;

        unsigned modification();


        QString errorString(int err) const;
        bool isOpened() const;

        /* ------------------- интерфейс вывода -----------------------------*/
        DevOutStream *devOutStream() {return static_cast<DevOutStream*>(this);}
        DevOut *devOut() {return static_cast<DevOut*>(this);}

        double outDacMaxVal(int ch) const;
        double outDacMinVal(int ch) const;
        double outDacMaxFreq() const;
        double outFreq() const {return m_outFreq;}

        bool outGenRunning() const;



        void setRange(int ch, int range);
        int range(int ch);
        void setOutFreq(double freq) {m_outFreq = freq;}
        double outDacMaxVal() const;
        double dacMaxRangeVal(unsigned range) const;

        int outDacSetChMode(unsigned ch, OutChMode mode) {
            m_dacModes[ch] = mode;
            return 0;
        }

        OutChMode outDacChMode(int ch) const {return m_dacModes[ch];}
        OutChMode outDigChMode(int ch) const {return OutChModeRam; }

    signals:


    protected:
        virtual TLTR* channel() const;

        Error protOpen();
        Error protClose();
        Error protConfigure();
        Error protLoadConfig(QSettings &set);
        Error protSaveConfig(QSettings &set) const;


        Error protOutStreamInit();
        Error protOutStreamStart();
        Error protOutStreamStop(unsigned tout);


        Error protOutCycleLoadStart(unsigned size);
        Error protOutCycleLoadFinish();
        Error protOutCycleGenStart();
        Error protOutCycleGenStop();

        Error protOutSendData(const double *dac_data, unsigned dac_size,
                               const unsigned *dig_data, unsigned dig_size, unsigned flags, unsigned tout);
        Error protOutFlushData(unsigned tout);


        Error rawWordsSend(const DWORD *wrds, unsigned size, unsigned tout, unsigned *sent_size);
    private:
        Error digStreamLoadPrepare();

        explicit LTR35(QSharedPointer<LtrCrate> crate, unsigned slot, QObject *parent = 0);
        double m_outFreq;
        OutChMode m_dacModes[LTR35_DAC_CHANNEL_CNT];

        mutable TLTR35 m_hnd;

        DeviceFrameSender<LTR35, DWORD> m_sender;

        friend class DeviceFrameSender<LTR35, DWORD>;
        friend class LtrResolver;
    };
}

#endif // LQMEAS_LTR35_H
