#include "LTR11.h"
#include "LTR11Info.h"
#include "LTR11Config.h"
#include "lqmeas/lqtdefs.h"
#include "lqmeas/devs/LTR/crates/LTRCrate.h"
#include "lqmeas/ifaces/in/DevAdcConfig.h"


namespace LQMeas {
    LTR11::LTR11(QSharedPointer<LTRCrate> crate, unsigned slot, QObject *parent) :
        LTRModule(crate, slot, typeModuleID, new LTR11Config(), new LTR11Info(), parent),
        DevAdc(this, LTR11Config::adc_channels_cnt),
        m_receiver(this), m_run(false) {

        LTR11_Init(&m_hnd);
    }

    bool LTR11::isOpened() const {
        return LTR11_IsOpened(&m_hnd) == LTR_OK;
    }

    QString LTR11::errorString(int err) const {
        return QSTRING_FROM_CSTR(LTR11_GetErrorString(err));
    }

    Error LTR11::adcGetData(double *data, unsigned size, unsigned flags, unsigned tout, unsigned *recvd_size) {
        Error err = Error::Success();
        unsigned recvd_words=0;
        DWORD* wrds;
        INT proc_size = 0;

        err = m_receiver.getFrames(size, tout, &wrds, &recvd_words);

        if (err.isSuccess() && (recvd_words != 0)) {
            proc_size = (INT)recvd_words;

            err = error(LTR11_ProcessData(&m_hnd, wrds, data, &proc_size, TRUE, TRUE));
            if (!err.isSuccess()) {
                LQMeasLog->error(tr("Process data error"), err, this);
            }
        }

        if (err.isSuccess() && (recvd_size != NULL))
            *recvd_size = proc_size;

        return err;
    }

    const LTR11Config *LTR11::devspecConfig() const {
        return static_cast<const LTR11Config *>(currentConfig());
    }

    TLTR *LTR11::channel() const {
        return &m_hnd.Channel;
    }

    Error LTR11::protOpen() {
        Error err = error(LTR11_Open(&m_hnd, crate()->ltrdIpAddr(), crate()->ltrdTcpPort(),
                                     crate()->serial().toLatin1(), slot()));
        if (err.isSuccess()) {
            err = error(LTR11_GetConfig(&m_hnd));
        }

        if (err.isSuccess()) {
            setDevicInfo(new LTR11Info(LTR11TypeInfo::defaultInfo(),
                                       QSTRING_FROM_CSTR(m_hnd.ModuleInfo.Serial), m_hnd.ModuleInfo.Ver));
        } else {
            LTR11_Close(&m_hnd);
        }
        return err;
    }

    Error LTR11::protClose() {
        return error(LTR11_Close(&m_hnd));
    }

    Error LTR11::protConfigure(const DeviceConfig *cfg) {
        Error err = Error::Success();
        const LTR11Config *setCfg = dynamic_cast<const LTR11Config *>(cfg);
        if (setCfg) {
            LTR11_FindAdcFreqParams(setCfg->adcFreq(), &m_hnd.ADCRate.prescaler,
                                    &m_hnd.ADCRate.divider, NULL);
            m_hnd.ADCMode = LTR11_ADCMODE_ACQ;
            m_hnd.InpMode = setCfg->convMode() == LTR11Config::ConvModeExtFall ? LTR11_INPMODE_EXTFALL :
                            setCfg->convMode() == LTR11Config::ConvModeExtRise ? LTR11_INPMODE_EXTRISE : LTR11_INPMODE_INT ;
            m_hnd.StartADCMode = setCfg->startMode() == LTR11Config::StartModeExtFall ? LTR11_STARTADCMODE_EXTFALL :
                                 setCfg->startMode() == LTR11Config::StartModeExtRise ? LTR11_STARTADCMODE_EXTRISE :
                                                                                        LTR11_STARTADCMODE_INT;
            unsigned ch_cnt = 0;
            for (unsigned ch = 0; ch < LTR11Config::adc_channels_cnt; ch++) {
                if (setCfg->adcChEnabled(ch)) {
                    LTR11Config::AdcChMode mode = setCfg->adcChMode(ch);
                    BYTE mode_code = mode == LTR11Config::AdcChModeComm ? LTR11_CHMODE_COMM :
                                     (mode == LTR11Config::AdcChModeZero ? LTR11_CHMODE_ZERO : LTR11_CHMODE_DIFF);

                    m_hnd.LChTbl[ch_cnt++] = LTR11_CreateLChannel(
                                ch, mode_code, setCfg->adcChRangeNum(ch));
                }
            }
            m_hnd.LChQnt = ch_cnt;

            err = error(LTR11_SetADC(&m_hnd));
        } else {
            err = StdErrors::InvalidConfigType();
        }
        return err;
    }

    Error LTR11::rawWordsReceive(DWORD *wrds, unsigned size, unsigned tout, unsigned *recvd_size) {
        Error err = Error::Success();
        INT recv_res = LTR11_Recv(&m_hnd, wrds, 0, size, tout);
        if (recv_res >= 0) {
            *recvd_size = recv_res;
        } else {
            err = error(recv_res);
        }
        return err;
    }

    Error LTR11::protAdcStart() {
        Error err = error(LTR11_Start(&m_hnd));
        if (err.isSuccess()) {
            m_run = true;
            m_receiver.setFrameSize(adcConfig()->adcEnabledChCnt());
        }
        return err;
    }

    Error LTR11::protAdcStop() {
        Error err = error(LTR11_Stop(&m_hnd));
        m_run = false;
        return err;
    }
}
