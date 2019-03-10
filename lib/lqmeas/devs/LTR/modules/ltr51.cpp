#include "ltr51.h"
#include "lqmeas/lqtdefs.h"
#include "lqmeas/devs/ltr/crates/LtrCrate.h"

namespace LQMeas {
    QString LTR51::typeModuleName() {
        return "LTR51";
    }

    LTR51::LTR51(QSharedPointer<LtrCrate> crate, unsigned slot, QObject *parent) :
        LtrModule(crate, slot, typeModuleID, parent), DevAdc(this, TypeParallel),
        m_receiver(this) {
        LTR51_Init(&m_hnd);
        adcSetChCnt(LTR51_CHANNEL_CNT);
        m_mezzaninesList.resize(typeMezzanineCnt);
    }


    QString LTR51::name() const {
        return typeModuleName();
    }

    QString LTR51::serial() const {
        return QSTRING_FROM_CSTR(m_hnd.ModuleInfo.Serial);
    }

    QString LTR51::modificationName() const {
        return m_hnd.ModuleInfo.Modification == LTR51_MOD_M ? "LTR51М" : "LTR51";
    }

    QString LTR51::verFPGA() const {
        return QSTRING_FROM_CSTR(m_hnd.ModuleInfo.FPGA_Version);
    }

    QString LTR51::verFirmMCU() const {
        return QSTRING_FROM_CSTR(m_hnd.ModuleInfo.FirmwareVersion);
    }

    double LTR51::adcFreq() const {
        return 1000./m_hnd.AcqTime;
    }

    double LTR51::adcChRange(int ch) const {
        return m_hnd.Fs/2;
    }

    QString LTR51::errorString(int err) const {
        return QSTRING_FROM_CSTR(LTR51_GetErrorString(err));
    }

    bool LTR51::isOpened() const {
        return LTR51_IsOpened(&m_hnd) == LTR_OK;
    }

    bool LTR51::adcChEnabled(int ch) const {
        Q_ASSERT((ch>=0) && (ch < LTR51_CHANNEL_CNT));
        return m_cfg.Ch[ch].enabled;
    }

    QSharedPointer<LtrMezzanine> LTR51::mezzanine(unsigned slot) {
        Q_ASSERT((slot < (unsigned)m_mezzaninesList.size()));
        return m_mezzaninesList[slot];
    }

    Error LTR51::adcGetData(double *data, unsigned size, unsigned flags, unsigned tout, unsigned *recvd_size) {
        Error err = Error::Success();
        DWORD* wrds;
        unsigned recvd_words;
        unsigned proc_frame_size = m_hnd.TbaseQnt*2*LTR51_CHANNEL_CNT;
        unsigned wrds_cnt = proc_frame_size * (size/adcEnabledChCnt());
        unsigned done_size = 0;

        if (err.isSuccess()) {
            err = m_receiver.getFrames(wrds_cnt, tout, &wrds, &recvd_words);
        }

        if (err.isSuccess() && (recvd_words!=0)) {
            for (unsigned i=0; (i < recvd_words) && err.isSuccess(); i+=proc_frame_size) {
                DWORD proc_words = recvd_words;
                err = error(LTR51_ProcessData(&m_hnd, wrds, 0, &data[done_size], &proc_words));
                if (err.isSuccess()) {
                    done_size+=adcEnabledChCnt();
                } else {
                    LQMeasLog->error(tr("Process data error"), err, this);
                }
            }
        }
        if (err.isSuccess() && recvd_size)
            *recvd_size = done_size;
        return err;
    }

    TLTR *LTR51::channel() const {
        return &m_hnd.Channel;
    }

    Error LTR51::protOpen() {
        int err = LTR51_Open(&m_hnd, crate()->ltrdIpAddr(), crate()->ltrdTcpPort(),
                             crate()->serial().toLatin1(), slot(), "");
        if (err==LTR_OK) {
            QScopedArrayPointer<TLTR51_MEZZANINE_INFO> mezInfo(new TLTR51_MEZZANINE_INFO[LTR51_MEZZANINE_CNT]);


            err = LTR51_GetMezzaninesInfo(&m_hnd, mezInfo.data());
            if (err==LTR_OK) {
                for (unsigned m=0; m < LTR51_MEZZANINE_CNT; m++) {
                    if (mezInfo[m].Name[0]!='\0') {
                        m_mezzaninesList[m] = QSharedPointer<LTR51_Mezzanine>(new LTR51_Mezzanine(&mezInfo[m]));
                    } else {
                        m_mezzaninesList[m] = QSharedPointer<LTR51_Mezzanine>();
                    }
                }
            }
        }

        if (err!=LTR_OK)
            LTR51_Close(&m_hnd);
        return error(err);
    }

    Error LTR51::protClose() {
        return error(LTR51_Close(&m_hnd));
    }

    Error LTR51::protConfigure() {
        return error(LTR51_Config(&m_hnd));
    }

    Error LTR51::protLoadConfig(QSettings &set) {
        /** @todo */
        return error(LTR_ERROR_NOT_IMPLEMENTED);
    }

    Error LTR51::protSaveConfig(QSettings &set) const {
        /** @todo */
        return error(LTR_ERROR_NOT_IMPLEMENTED);
    }


    Error LTR51::protAdcSetRawConfig(const void *rawCfg, size_t cfg_size) {
        Error err = Error::Success();

        if (cfg_size != sizeof(AdcConfig)) {
            err = StdErrors::InvalidAdcConfig();
        } else {
            memcpy(&m_cfg, rawCfg, cfg_size);
            unsigned lch_cnt = 0;
            for (unsigned ch = 0; ch < LTR51_CHANNEL_CNT; ch++) {
                if (m_cfg.Ch[ch].enabled) {
                    m_hnd.LChTbl[lch_cnt++] = LTR51_CreateLChannelEx(ch+1, &m_cfg.Ch[ch].highThreshold,
                                                                    &m_cfg.Ch[ch].lowThreshold,
                                                                    m_cfg.Ch[ch].thresholdRange,
                                                                    m_cfg.Ch[ch].edge,
                                                                    m_cfg.Ch[ch].freqRange);
                }
            }
            m_hnd.LChQnt = lch_cnt;
            m_hnd.AcqTime = m_cfg.acqTime;
            m_hnd.SetUserPars = m_cfg.userBaseFs;
            if (m_cfg.userBaseFs) {
                m_hnd.Fs = m_cfg.fs;
                m_hnd.Base = m_cfg.base;
            }
        }
        return err;
    }

    Error LTR51::rawWordsReceive(DWORD *wrds, unsigned size, unsigned tout, unsigned *recvd_size) {
        Error err = Error::Success();
        INT recv_res = LTR51_Recv(&m_hnd, wrds, 0, size, tout);
        if (recv_res >= 0) {
            *recvd_size = recv_res;
        } else {
            err = error(recv_res);
        }
        return err;
    }

    Error LTR51::protAdcStart() {
        m_receiver.setFrameSize(m_hnd.TbaseQnt*2*LTR51_CHANNEL_CNT);
        return error(LTR51_Start(&m_hnd));
    }

    Error LTR51::protAdcStop() {
        return error(LTR51_Stop(&m_hnd));
    }



    LTR51_Mezzanine::LTR51_Mezzanine(const TLTR51_MEZZANINE_INFO *info) :
        LtrMezzanine(QSTRING_FROM_CSTR(info->Name), QSTRING_FROM_CSTR(info->Serial),
                     LTR51_MEZZANINE_CHANNEL_CNT) {

    }
}
