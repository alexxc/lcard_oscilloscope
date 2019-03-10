#ifndef LQMEAS_LTR210_H
#define LQMEAS_LTR210_H

#include "LtrModule.h"
#include "ltr/include/ltr210api.h"
#include "lqmeas/ifaces/in/DevAdc.h"
#include "lqmeas/devs/DeviceFrameReceiver.h"

class QElapsedTimer;

namespace LQMeas {
    class LTR210 : public LtrModule, public DevAdc {
        Q_OBJECT
    public:
        static QString typeModuleName();
        static const quint16 typeModuleID = LTR_MID_LTR210;
        static const unsigned typeAdcChCnt = LTR210_CHANNEL_CNT;


        ~LTR210();

        QString name() const;
        QString serial() const;
        unsigned verFPGA() const;
        unsigned verPLD() const;


        /* ----------------------- интерфейс ввода  --------------------------*/
        DevAdc *devAdc() {return static_cast<DevAdc*>(this);}

        double adcFreq() const;
        double adcChRange(int ch_num) const;

        const double adcRangeMaxVal(int range) const;
        const double adcRangeMinVal(int range) const;
        unsigned adcRangesCnt() const;
        double adcFreqMax() const;

        QString errorString(int err) const;
        bool isOpened() const;

        bool adcIsRunning() const;
        bool adcChEnabled(int ch) const;


        Error startFrame();
        Error measZeroAllRanges();
        Error getFrame(double *data, unsigned size, unsigned flags, unsigned tout);
        Error waitSof(QElapsedTimer &tmr, unsigned tout);


        Error adcGetData(double *data, unsigned size, unsigned flags, unsigned tout,
                       unsigned *recvd_size, TLTR210_FRAME_STATUS *status);

        Error loadFPGA(QString filename);



        Error adcGetData(double *data, unsigned size, unsigned flags,
                               unsigned tout, unsigned *recvd_size);

    protected:
        virtual TLTR* channel() const;

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
        explicit LTR210(QSharedPointer<LtrCrate> crate, unsigned slot, QObject *parent = 0);

        static void APIENTRY cb_load_porgr(void* cb, TLTR210 *hnd, DWORD done_cnt, DWORD total_cnt);
        static QString loadFpgaOpDescr() {return tr("Loading FPGA firmware");}

        mutable TLTR210 m_hnd;

        friend class LtrResolver;
        double m_zeroOffs[typeAdcChCnt][LTR210_RANGE_CNT];
        DeviceFrameReceiver<LTR210, DWORD> m_receiver;

        friend class DeviceFrameReceiver<LTR210, DWORD>;

    };
}

#endif // LQMEAS_LTR210_H
