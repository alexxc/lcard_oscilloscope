#include "LTR24.h"
#include "LTR24Config.h"
#include "LTR24Info.h"
#include "lqmeas/lqtdefs.h"
#include "lqmeas/devs/LTR/crates/LTRCrate.h"
#include "lqmeas/ifaces/in/DevAdcConfig.h"


namespace LQMeas {
    LTR24::LTR24(QSharedPointer<LTRCrate> crate, unsigned slot, QObject *parent) :
        LTRModule(crate, slot, typeModuleID, new LTR24Config(), new LTR24Info(), parent),
        DevAdc(this, LTR24TypeInfo::adc_channels_cnt),
        m_receiver(this) {

        LTR24_Init(&m_hnd);
    }


    bool LTR24:: isOpened() const {
        return LTR24_IsOpened(&m_hnd)==LTR_OK;
    }

    bool LTR24::adcIsRunning() const {
        return m_hnd.Run ? true : false;
    }

    Error LTR24::adcGetData(double *data, unsigned size, unsigned flags, unsigned tout,
                           unsigned *recvd_size) {
        Error err = Error::Success();
        int wrds_cnt = m_hnd.DataFmt == LTR24_FORMAT_20 ? size : 2*size;
        unsigned recvd_words=0;
        DWORD* wrds;
        QScopedArrayPointer<BOOL> ovrlds(new BOOL[size]);
        INT proc_size = 0;

        if (err.isSuccess()) {
            err = m_receiver.getFrames(wrds_cnt, tout, &wrds, &recvd_words);
        }

        if (err.isSuccess() && (recvd_words!=0)) {
            DWORD proc_flags = LTR24_PROC_FLAG_VOLT | LTR24_PROC_FLAG_AFC_COR | LTR24_PROC_FLAG_CALIBR;
            proc_size = (INT)recvd_words;

            err = error(LTR24_ProcessData(&m_hnd, wrds, data, &proc_size,
                                          proc_flags, ovrlds.data()));
            if (!err.isSuccess()) {
                LQMeasLog->error(tr("Process data error"), err, this);
            } else {
                if (m_hnd.DataFmt == LTR24_FORMAT_24) {
                    unsigned en_ch_cnt = adcConfig()->adcEnabledChCnt();
                    for (unsigned ch_idx = 0; ch_idx < en_ch_cnt; ch_idx++) {
                        ChannelStatus status = ChStatusOk;
                        for (unsigned i=ch_idx; ((INT)i < proc_size) && (status == ChStatusOk); i+=en_ch_cnt) {
                            if (ovrlds[i]) {
                                status = ChStatusOverload;
                            }
                        }
                        adcSetChStatus(adcChNum(ch_idx), status);
                    }
                }
            }
        }

        if (err.isSuccess() && recvd_size)
            *recvd_size = proc_size;

        return err;
    }

    const LTR24Config *LTR24::devspecConfig() const {
        return static_cast<const LTR24Config*>(currentConfig());
    }

    const LTR24Info *LTR24::devspecInfo() const {
        return static_cast<const LTR24Info *>(devInfo());
    }

    TLTR *LTR24::channel() const {
        return &m_hnd.Channel;
    }

    Error LTR24::protOpen() {
        Error err = error(LTR24_Open(&m_hnd, crate()->ltrdIpAddr(), crate()->ltrdTcpPort(),
                                     crate()->serial().toLatin1(), slot()));
        if (err.isSuccess()) {
            err = error(LTR24_GetConfig(&m_hnd));
        }

        if (err.isSuccess()) {
            setDevicInfo(new LTR24Info(m_hnd.ModuleInfo.SupportICP ? LTR24TypeInfo::typeInfoMod2() : LTR24TypeInfo::typeInfoMod2(),
                                       QSTRING_FROM_CSTR(m_hnd.ModuleInfo.Serial), m_hnd.ModuleInfo.VerPLD));
        }

        if (!err.isSuccess()) {
            LTR24_Close(&m_hnd);
        }
        return err;
    }

    Error LTR24::protClose() {
        return error(LTR24_Close(&m_hnd));
    }


    Error LTR24::protConfigure(const DeviceConfig *cfg) {
        Error err = Error::Success();
        const LTR24Config *setCfg = dynamic_cast<const LTR24Config *>(cfg);
        if (setCfg) {
            m_hnd.ADCFreqCode = setCfg->adcFreqCode();
            m_hnd.DataFmt =   setCfg->adcDataFormat() == LTR24Config::AdcDataFormat20 ? LTR24_FORMAT_20 : LTR24_FORMAT_24;
            m_hnd.ISrcValue = setCfg->adcISrcValue() == LTR24Config::ISrcValue_10mA ? LTR24_I_SRC_VALUE_10 : LTR24_I_SRC_VALUE_2_86;
            m_hnd.TestMode = setCfg->adcTestModesEnabled();
            for (int ch=0; ch < LTR24TypeInfo::adc_channels_cnt; ch++) {
                m_hnd.ChannelMode[ch].Enable = setCfg->adcChEnabled(ch);
                m_hnd.ChannelMode[ch].AC = setCfg->adcChAcMode(ch) == LTR24Config::ChAcOnly;
                m_hnd.ChannelMode[ch].Range = setCfg->adcChRangeNum(ch);
                m_hnd.ChannelMode[ch].ICPMode = setCfg->adcChMode(ch) == LTR24Config::ChModeICP;
            }

            err = error(LTR24_SetADC(&m_hnd));
        } else {
            err = StdErrors::InvalidConfigType();
        }
        return err;
    }

    Error LTR24::rawWordsReceive(DWORD *wrds, unsigned size, unsigned tout,
                                 unsigned *recvd_size) {
        Error err = Error::Success();
        INT recv_res = LTR24_Recv(&m_hnd, wrds, 0, size, tout);
        if (recv_res >= 0) {
            *recvd_size = recv_res;
        } else {
            err = error(recv_res);
        }
        return err;
    }

    Error LTR24::protAdcStart() {
        Error err = error(LTR24_Start(&m_hnd));
        if (err.isSuccess()) {
            m_receiver.setFrameSize(adcConfig()->adcEnabledChCnt() *
                                    (m_hnd.DataFmt == LTR24_FORMAT_20 ? 1 : 2));
        }
        return err;
    }

    Error LTR24::protAdcStop() {
        return error(LTR24_Stop(&m_hnd));
    }

    QString LTR24::errorString(int err) const {
        return QSTRING_FROM_CSTR(LTR24_GetErrorString(err));
    }
}
