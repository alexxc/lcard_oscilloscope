#include "ltr25.h"
#include "lqmeas/lqtdefs.h"
#include "lqmeas/devs/ltr/crates/LtrCrate.h"

    namespace LQMeas {
    static double f_adc_ranges[] = {LTR25_ADC_RANGE_PEAK};

    static const double f_adc_freqs[] = {78.125e3, 39.0625e3, 19.53125e3, 9.765625e3,
                                    4.8828125e3, 2.44140625e3, 1.220703125e3, 610.3515625};


    LTR25::LTR25(QSharedPointer<LtrCrate> crate, unsigned slot, QObject *parent) :
        LtrModule(crate, slot, typeModuleID, parent), DevAdc(this, DevAdc::TypeParallel),
        m_receiver(this) {

        LTR25_Init(&m_hnd);
        adcSetChCnt(LTR25_CHANNEL_CNT);
    }

    QString LTR25::typeModuleName() {
        return "LTR25";
    }

    QString LTR25::name() const {
        return typeModuleName();
    }

    QString LTR25::serial() const {
        return QSTRING_FROM_CSTR(m_hnd.ModuleInfo.Serial);
    }

    unsigned LTR25::verFPGA() const {
        return m_hnd.ModuleInfo.VerFPGA;
    }

    unsigned LTR25::verPLD() const {
        return m_hnd.ModuleInfo.VerPLD;
    }

    unsigned LTR25::boardRevision() const {
        return m_hnd.ModuleInfo.BoardRev;
    }

    bool LTR25::industrial() const {
        return m_hnd.ModuleInfo.Industrial ? true : false;
    }

    const double LTR25::adcRangeMaxVal(int range) const {
        return f_adc_ranges[range];
    }

    const double LTR25::adcRangeMinVal(int range) const {
        return -f_adc_ranges[range];
    }


    unsigned LTR25::adcRangesCnt() const {
        return sizeof(f_adc_ranges)/sizeof(f_adc_ranges[0]);
    }

    double LTR25::adcFreqMax() const {
        return f_adc_freqs[0];
    }

    double LTR25::typeAdcFreqVal(int idx) {
        return f_adc_freqs[idx];
    }

    unsigned LTR25::typeAdcFreqsCnt() {
        return sizeof(f_adc_freqs)/sizeof(f_adc_freqs[0]);
    }

    double LTR25::adcFreq() const {
        return m_hnd.State.AdcFreq;
    }

    double LTR25::adcChRange(int ch) const {
        return f_adc_ranges[0];
    }

    QString LTR25::errorString(int err) const {
        return QSTRING_FROM_CSTR(LTR25_GetErrorString(err));
    }

    bool LTR25::isOpened() const {
        return LTR25_IsOpened(&m_hnd) == LTR_OK;
    }

    bool LTR25::adcIsRunning() const {
        return m_hnd.State.Run ? true : false;
    }

    bool LTR25::adcChEnabled(int ch) const {
        return ch < LTR25_CHANNEL_CNT ? m_hnd.Cfg.Ch[ch].Enabled : false;
    }

    Error LTR25::protAdcStart() {
        m_receiver.setFrameSize(m_hnd.State.EnabledChCnt *
                               (m_hnd.Cfg.DataFmt == LTR25_FORMAT_20 ? 1 : 2));
        return error(LTR25_Start(&m_hnd));

    }

    Error LTR25::protAdcStop() {
        return error(LTR25_Stop(&m_hnd));
    }

    Error LTR25::protAdcSetRawConfig(const void *cfg, size_t cfg_size) {
        Error err;
        if (cfg_size != sizeof(TLTR25_CONFIG)) {
            err = error(LTR_ERROR_PARAMETERS);
        } else {
            memcpy(&m_hnd.Cfg, cfg, cfg_size);
        }
        return err;
    }

    Error LTR25::adcGetData(double *data, unsigned size, unsigned flags, unsigned tout, unsigned *recvd_size) {
        Error err = Error::Success();
        int wrds_cnt = m_hnd.Cfg.DataFmt == LTR25_FORMAT_20 ? size : 2*size;
        unsigned recvd_words=0;
        INT proc_size;

        DWORD* wrds;
        DWORD cur_ch_status[LTR25_CHANNEL_CNT];

        if (err.isSuccess()) {
            err = m_receiver.getFrames(wrds_cnt, tout, &wrds, &recvd_words);
        }

        if (err.isSuccess() && (recvd_words!=0)) {
            DWORD proc_flags = LTR25_PROC_FLAG_VOLT;
            proc_size = (INT)recvd_words;

            err = error(LTR25_ProcessData(&m_hnd, wrds, data, &proc_size,
                                          proc_flags, cur_ch_status));
            if (!err.isSuccess()) {
                LQMeasLog->error(tr("Process data error"), err, this);
            } else {
                unsigned cur_ch = 0;
                for (unsigned ch=0; ch < LTR25_CHANNEL_CNT; ch++) {
                    if (adcChEnabled(ch)) {
                        switch (cur_ch_status[cur_ch]) {
                            case LTR25_CH_STATUS_OK:
                                adcSetChStatus(ch, DevAdc::ChStatusOk);
                                break;
                            case LTR25_CH_STATUS_OPEN:
                                adcSetChStatus(ch, DevAdc::ChStatusOpen);
                                break;
                            case LTR25_CH_STATUS_SHORT:
                                adcSetChStatus(ch, DevAdc::ChStatusShort);
                                break;
                            default:
                                adcSetChStatus(ch, DevAdc::ChStatusError);
                                break;
                        }
                        cur_ch++;
                    }
                }
            }
        }

        if (err.isSuccess()) {
            if (recvd_size) {
                *recvd_size = proc_size;
            } else if (size != proc_size) {
                err = error(LTR_ERROR_RECV_INSUFFICIENT_DATA);
            }
        }

        return err;
    }

    TLTR *LTR25::channel() const {
        return &m_hnd.Channel;
    }

    Error LTR25::protOpen() {
        Error err = error(LTR25_Open(&m_hnd, crate()->ltrdIpAddr(), crate()->ltrdTcpPort(),
                                           crate()->serial().toLatin1(), slot()));
        if (!err.isSuccess())
            LTR25_Close(&m_hnd);
        return err;
    }

    Error LTR25::protClose() {
        return error(LTR25_Close(&m_hnd));
    }

    Error LTR25::protConfigure() {
        return error(LTR25_SetADC(&m_hnd));
    }

    Error LTR25::protLoadConfig(QSettings &set) {

        int intval;
        bool ok;

        int ch_cnt = set.beginReadArray("Channels");
        if (ch_cnt == 0) {
            ch_cnt = 1;
            m_hnd.Cfg.Ch[0].Enabled = TRUE;
        } else {
            for (int ch=0; (ch < ch_cnt) && (ch< adcChannelsCnt()); ch++) {
                set.setArrayIndex(ch);
                m_hnd.Cfg.Ch[ch].Enabled = set.value("Enabled").toBool();
            }
        }

        for (int ch=ch_cnt; ch < adcChannelsCnt(); ch++) {
            m_hnd.Cfg.Ch[ch].Enabled = false;
        }
        set.endArray();

        intval = set.value("DataFmt", LTR25_FORMAT_32).toInt(&ok);
        if (ok && ((intval==LTR25_FORMAT_20) || (intval==LTR25_FORMAT_32))) {
            m_hnd.Cfg.DataFmt = intval;
        } else {
            LQMeasLog->report(LogLevel::Warning, tr("Load config: invalid data format value"), this);
        }

        intval = set.value("FreqCode", LTR25_FREQ_78K).toInt(&ok);
        if (ok && (intval >= LTR25_FREQ_78K) && (intval <= LTR25_FREQ_610)) {
            m_hnd.Cfg.FreqCode = intval;
        } else {
             LQMeasLog->report(LogLevel::Warning, tr("Load config: invalid frequency code value"), this);
        }

        intval = set.value("ISrcValue", LTR25_I_SRC_VALUE_2_86).toInt(&ok);
        if (ok && ((intval==LTR25_I_SRC_VALUE_2_86) || (intval==LTR25_I_SRC_VALUE_10))) {
            m_hnd.Cfg.ISrcValue = intval;
        } else {
            LQMeasLog->report(LogLevel::Warning, tr("Load config: invalid current source value"), this);
        }
        return Error::Success();
    }

    Error LTR25::protSaveConfig(QSettings &set) const {
        set.beginWriteArray("Channels", adcChannelsCnt());

        for (int ch=0; ch< adcChannelsCnt(); ch++) {
            set.setArrayIndex(ch);
            set.setValue("Enabled", m_hnd.Cfg.Ch[ch].Enabled);
        }
        set.endArray();

        set.setValue("DataFmt", m_hnd.Cfg.DataFmt);
        set.setValue("FreqCode", m_hnd.Cfg.FreqCode);
        set.setValue("ISrcValue", m_hnd.Cfg.ISrcValue);

        return Error::Success();
    }

    Error LTR25::rawWordsReceive(DWORD *wrds, unsigned size, unsigned tout, unsigned *recvd_size) {
        Error err = Error::Success();
        INT recv_res = LTR25_Recv(&m_hnd, wrds, 0, size, tout);
        if (recv_res >= 0) {
            *recvd_size = recv_res;
        } else {
            err = error(recv_res);
        }
        return err;
    }
}



