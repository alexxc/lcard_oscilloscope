#include "ltr35.h"
#include "lqmeas/lqtdefs.h"
#include "lqmeas/devs/ltr/crates/LtrCrate.h"
#include "lqmeas/ifaces/out/DacSignals/DacSignalSin.h"
#include <math.h>

#ifndef M_PI
    #define M_PI 3.1415926535897932384626433832795
#endif


namespace LQMeas {
    LTR35::LTR35(QSharedPointer<LtrCrate> crate, unsigned slot, QObject *parent) :
        LtrModule(crate, slot, typeModuleID, parent),
        DevOutStream(this),
        m_outFreq(LTR35_DAC_FREQ_DEFAULT),
        m_sender(this) {

        LTR35_Init(&m_hnd);

        for (int ch = 0; ch < LTR35_DAC_CHANNEL_CNT; ch++)
            m_dacModes[ch] = OutChModeRam;
    }

    QString LTR35::typeModuleName() {
        return "LTR35";
    }

    LTR35::~LTR35() {
        if (isOpened())
            close();
    }



    QString LTR35::name() const {
        return typeModuleName();
    }

    QString LTR35::modificationName() const {
        return QString("%1-%2-%3").arg(name()).arg(QString::number(m_hnd.ModuleInfo.Modification))
                .arg(QString::number(m_hnd.ModuleInfo.DacChCnt));
    }

    QString LTR35::serial() const {
        return QSTRING_FROM_CSTR(m_hnd.ModuleInfo.Serial);
    }

    unsigned LTR35::verFPGA() const {
        return m_hnd.ModuleInfo.VerFPGA;
    }

    unsigned LTR35::verPLD() const {
        return m_hnd.ModuleInfo.VerPLD;
    }

    unsigned LTR35::modification() {
        return m_hnd.ModuleInfo.Modification;
    }

    QString LTR35::errorString(int err) const {
        return QSTRING_FROM_CSTR(LTR35_GetErrorString(err));
    }

    bool LTR35::isOpened() const {
        return LTR35_IsOpened(&m_hnd) == LTR_OK;
    }



#if 0
    Error LTR35::outSignalsGenerate() {
        Error err = Error::Success();
        unsigned int sig_size;

        m_hnd.Cfg.Mode = LTR35_MODE_CYCLE;
        m_hnd.Cfg.DataFmt = LTR35_FORMAT_24;
        err = configure();

        /* обновление реально установленных частот сигналов */
        if (err.isSuccess()) {
            unsigned out_ch = m_hnd.State.SDRAMChCnt + 1;
            unsigned max_sig_size = LTR35_MAX_POINTS_PER_PAGE/out_ch;
            err = calcCycleSigsParams(m_hnd.State.DacFreq, max_sig_size, &sig_size);
        }


        if (err.isSuccess()) {
            QVector<double> dac_data;
            QVector<unsigned> dig_data;

            err = outGenRamData(0, sig_size, dac_data, dig_data);

            if (err.isSuccess()) {
                if (dig_data.size()==0) {
                    unsigned dig_data = LTR35_DIGOUT_WORD_DIS_H | LTR35_DIGOUT_WORD_DIS_L;
                    err = protSendStreamData(0, 0, &dig_data, 1, 0, 1000);
                }
            }

            if (err.isSuccess() && ((dac_data.size() + dig_data.size())!=0)) {
                err = protSendStreamData(dac_data.data(), dac_data.size(),
                                         dig_data.data(), dig_data.size(),
                                         0, 3000 + (dig_data.size() + dac_data.size())/1000);
            }

            if (err.isSuccess() && m_sender.unsentWordsCnt()) {
                err = error(LTR_ERROR_SEND_INSUFFICIENT_DATA);
                reportError("Передано в модуль меньше данных, чем запрашивалось", err);
            }
        }

        if (err.isSuccess()) {
            err = error(LTR35_SwitchCyclePage(&m_hnd, 0, 30000));
            if (!err.isSuccess()) {
                reportError("Ошибка установки сигнала", err);
            }
        }

        return err;
    }
#endif

    void LTR35::setRange(int ch, int range) {
        Q_ASSERT((ch < LTR35_DAC_CHANNEL_CNT) && (range < LTR35_DAC_CH_OUTPUTS_CNT));
        m_hnd.Cfg.Ch[ch].Output = range;
    }

    int LTR35::range(int ch) {
        Q_ASSERT(ch < LTR35_DAC_CHANNEL_CNT);
        return m_hnd.Cfg.Ch[ch].Output;
    }

    double LTR35::outDacMaxVal(int ch) const {
        Q_ASSERT(ch < LTR35_DAC_CHANNEL_CNT);
        return m_hnd.ModuleInfo.DacOutDescr[m_hnd.Cfg.Ch[ch].Output].AmpMax;
    }

    double LTR35::outDacMinVal(int ch) const {
        Q_ASSERT(ch < LTR35_DAC_CHANNEL_CNT);
        return m_hnd.ModuleInfo.DacOutDescr[m_hnd.Cfg.Ch[ch].Output].AmpMin;
    }

    double LTR35::dacMaxRangeVal(unsigned range) const {
        Q_ASSERT(range < LTR35_DAC_CH_OUTPUTS_CNT);
        return m_hnd.ModuleInfo.DacOutDescr[range].AmpMax;
    }

    double LTR35::outDacMaxVal() const {
        return m_hnd.ModuleInfo.DacOutDescr[0].AmpMax;
    }

    double LTR35::outDacMaxFreq() const {
        return LTR35_DAC_FREQ_MAX;
    }

    bool LTR35::outGenRunning() const {
        return m_hnd.State.Run ? true : false;
    }


    TLTR *LTR35::channel()  const {
        return &m_hnd.Channel;
    }

    Error LTR35::protOpen() {
        Error err = error(LTR35_Open(&m_hnd, crate()->ltrdIpAddr(), crate()->ltrdTcpPort(),
                                crate()->serial().toLatin1(), slot()));
        if (err.isSuccess()) {
            outSetDacChCnt(m_hnd.ModuleInfo.DacChCnt);
            outSetDigChCnt(m_hnd.ModuleInfo.DoutLineCnt);
        }

        if (!err.isSuccess())
            LTR35_Close(&m_hnd);

        return err;
    }

    Error LTR35::protClose() {
        return error(LTR35_Close(&m_hnd));
    }

    Error LTR35::protConfigure() {
        static const BYTE arith_src_codes[LTR35_ARITH_SRC_CNT] = {
            LTR35_CH_SRC_COS1,
            LTR35_CH_SRC_COS2,
            LTR35_CH_SRC_COS3,
            LTR35_CH_SRC_COS4
        };
        INT arith_src[LTR35_DAC_CHANNEL_CNT];
        QList<double> arith_freqs;
        double dac_freq = m_outFreq;
        Error err;

        err = error(LTR35_Stop(&m_hnd, 0));

        for (int ch=0; (ch < outDacChannelsCnt()) && err.isSuccess(); ch++) {
            if (outDacChMode(ch) == OutChModeHard) {
                QSharedPointer<DacSignal> sig = outDacSignal(ch);
                if (sig) {
                    QSharedPointer<DacSignalSin> sin = qSharedPointerDynamicCast<DacSignalSin>(sig);
                    if (!sin) {
                        /* в арифметическом режиме можем выводить только синус */
                        err = error(LTR35_ERR_UNSUPPORTED_CONFIG);
                    } else {
                        /** @todo проверка, запоминание фазы и выбор sin/cos */
                        double freq = sig->signalFreq();
                        if (freq>0) {
                            int src = -1;


                            /* пробуем найти арифметический источник с такой же частотой */
                            for (int i=0; i < arith_freqs.size(); i++) {
                                if (qAbs(freq-arith_freqs.at(i)) < 0.001) {
                                    src = i;
                                }
                            }

                            /* если не нашли источник, значит новый, но может
                             * быть не больше LTR35_ARITH_SRC_CNT */
                            if (src < 0) {
                                if (arith_freqs.size()!=LTR35_ARITH_SRC_CNT) {
                                    src = arith_freqs.size();
                                    arith_freqs.append(freq);
                                } else {
                                    err = error(LTR35_ERR_UNSUPPORTED_CONFIG);
                                }
                            }

                            if (err.isSuccess())
                                arith_src[ch] = src;
                        }
                    }
                }
            }
        }

        if (err.isSuccess()) {
            err = error(LTR35_FillFreq(&m_hnd.Cfg, dac_freq, &dac_freq));
            LQMeasLog->report(LogLevel::DebugMedium, tr("synthesizer parameters a = %1, b = %2, r = %3")
                          .arg(QString::number(m_hnd.Cfg.Synt.a))
                          .arg(QString::number(m_hnd.Cfg.Synt.b))
                          .arg(QString::number(m_hnd.Cfg.Synt.r)), this);
        }

        /* установка арифметических источников */
        if (err.isSuccess() && arith_freqs.size()) {
            for (int s=0; s < arith_freqs.size(); s++) {
                m_hnd.Cfg.ArithSrc[s].Phase = 0; /** @todo */
                m_hnd.Cfg.ArithSrc[s].Delta = 2.*M_PI*arith_freqs.at(s)/dac_freq;
            }
        }


        if (err.isSuccess()) {
            for (unsigned ch = 0; ch < m_hnd.ModuleInfo.DacChCnt; ch++) {
                QSharedPointer<DacSignal> sig = outDacSignal(ch);
                if (sig) {
                    m_hnd.Cfg.Ch[ch].Enabled = 1;
                    if (outDacChMode(ch) == OutChModeRam) {
                        m_hnd.Cfg.Ch[ch].Source = LTR35_CH_SRC_SDRAM;
                    } else {
                        QSharedPointer<DacSignalSin> sin = qSharedPointerDynamicCast<DacSignalSin>(sig);
                        m_hnd.Cfg.Ch[ch].Source = arith_src_codes[arith_src[ch]];
                        m_hnd.Cfg.Ch[ch].ArithAmp = sin->amplitude();
                        m_hnd.Cfg.Ch[ch].ArithOffs = sin->offset();

                        double sig_freq = dac_freq * m_hnd.Cfg.ArithSrc[arith_src[ch]].Delta/(2.*M_PI);

                        sig->setRealParams(sig_freq, dac_freq);
                    }
                } else {
                    m_hnd.Cfg.Ch[ch].Enabled  = 0;
                }
            }

            err = error(LTR35_Configure(&m_hnd));
        }
        return err;
    }

    Error LTR35::protLoadConfig(QSettings &set) {

        int intval;
        bool ok;

        int ch_cnt = set.beginReadArray("Channels");
        for (int ch=0; (ch < ch_cnt) && (ch< outDacChannelsCnt()); ch++) {
            set.setArrayIndex(ch);            
            intval = set.value("Output", LTR35_DAC_OUT_FULL_RANGE).toInt(&ok);
            if (ok && ((intval == LTR35_DAC_OUT_FULL_RANGE) || (intval == LTR35_DAC_OUT_DIV_RANGE))) {
                m_hnd.Cfg.Ch[ch].Output = intval;
            } else {
                LQMeasLog->report(LogLevel::Warning, tr("Load config: invalid output number value for channel %1")
                                  .arg(QString::number(ch+1)), this);
            }
            intval = set.value("GenMode", OutChModeRam).toInt(&ok);
            if (ok && ((intval == OutChModeRam) || (intval == OutChModeHard))) {
                outDacSetChMode(ch, (OutChMode)intval);
            } else {
                LQMeasLog->report(LogLevel::Warning, tr("Load config: invalid output generation mode for channel %1")
                                  .arg(QString::number(ch+1)), this);
            }
        }

        set.endArray();
        outLoadSignalConfig(set);

        DevOut::OutRamSyncMode outMode = (DevOut::OutRamSyncMode)set.value("OutMode", DevOut::OutRamSyncModeStream).toInt();
        if ((outMode == DevOut::OutRamSyncModeStream) || (outMode == DevOut::OutRamSyncModeCycle)) {
            outSetRamSyncMode(outMode);
        } else {
            LQMeasLog->report(LogLevel::Warning, tr("Load config: invalid output mode value"), this);
        }

        /** @note
           Настройки источников и т.п. не сохраняются, т.к. они рассчитываются
           по установленным сигналам.
           Также не сохраняется значение выходной частоты, т.к. сейчас используется
           только 192 КГц
           Пока не сохраняются настройки: StreamStatusPeriod, EchoEnable, EchoChannel */
        return Error::Success();
    }

    Error LTR35::protSaveConfig(QSettings &set) const {
        set.beginWriteArray("Channels");

        for (int ch=0; ch < outDacChannelsCnt(); ch++) {
            set.setArrayIndex(ch);
            set.setValue("Output",  m_hnd.Cfg.Ch[ch].Output);
            set.setValue("GenMode", outDacChMode(ch));
        }
        set.endArray();

        set.setValue("OutMode", outRamSyncMode());

        return Error::Success();
    }

    Error LTR35::protOutStreamInit() {
        m_hnd.Cfg.Mode = LTR35_MODE_STREAM;
        m_hnd.Cfg.DataFmt = LTR35_FORMAT_20;
        Error err = configure();
        if (err.isSuccess()) {
            err = digStreamLoadPrepare();
        }
        return err;
    }

    Error LTR35::protOutStreamStart() {
        return error(LTR35_StreamStart(&m_hnd, 0));
    }

    Error LTR35::protOutStreamStop(unsigned tout) {
        return protOutCycleGenStop();
    }

    Error LTR35::protOutCycleLoadStart(unsigned size) {
        Error err = Error::Success();
        if (!outGenRunning()) {
            m_hnd.Cfg.Mode = LTR35_MODE_CYCLE;
            m_hnd.Cfg.DataFmt = LTR35_FORMAT_24;
            err = configure();
        }

        if (err.isSuccess())
            err = digStreamLoadPrepare();
        return err;
    }

    Error LTR35::protOutCycleLoadFinish() {
        if (outGenRunning())
            return error(LTR35_SwitchCyclePage(&m_hnd, 0, 30000));
        return Error::Success();
    }

    Error LTR35::protOutCycleGenStart() {
        return  error(LTR35_SwitchCyclePage(&m_hnd, 0, 30000));
    }

    Error LTR35::protOutCycleGenStop() {
        return error(LTR35_StopWithTout(&m_hnd, 0, 10000));
    }

    Error LTR35::protOutSendData(const double *dac_data, unsigned dac_size, const unsigned *dig_data,
                                 unsigned dig_size, unsigned flags, unsigned tout) {
        Error err = Error::Success();

        if (m_sender.unsentWordsCnt()) {
            err = m_sender.flushData(tout);
        }

        if (err.isSuccess()) {
            if (m_sender.unsentWordsCnt()!=0) {
                err = StdErrors::SendBusy();
            } else {
                DWORD snd_dac_req_size = dac_size;
                DWORD snd_dig_req_size = dig_size;
                DWORD snd_size = (snd_dac_req_size  + snd_dig_req_size) *
                                  (m_hnd.Cfg.DataFmt==LTR35_FORMAT_20 ? 1 : 2);
                QScopedArrayPointer<DWORD> wrds (new DWORD[snd_size]);

                err = error(LTR35_PrepareData(&m_hnd, dac_data, &snd_dac_req_size,
                                            (const DWORD*)dig_data, &snd_dig_req_size,
                                            LTR35_PREP_FLAGS_VOLT, wrds.data(), &snd_size));

                if (!err.isSuccess()) {
                    LQMeasLog->error(tr("Prepare data error"), err, this);
                } else {
                    m_sender.setFrameSize(snd_size);
                    err = m_sender.putFrames(wrds.data(), snd_size, tout, 0);
                }
            }
        }
        return err;
    }

    Error LTR35::protOutFlushData(unsigned tout) {
        Error err = Error::Success();

        if (m_sender.unsentWordsCnt()) {
            err = m_sender.flushData(tout);
        }

        if (err.isSuccess()) {
            if (m_sender.unsentWordsCnt()!=0) {
                err = StdErrors::SendBusy();
            }
        }

        if (err.isSuccess()) {
            m_sender.setFrameSize(0);
        }
        return err;
    }



    Error LTR35::rawWordsSend(const DWORD *wrds, unsigned size, unsigned tout, unsigned *sent_size) {
        Error err = Error::Success();
        int send_res = LTR35_Send(&m_hnd, wrds, size, tout);
        if (send_res >= 0) {
            *sent_size = send_res;
        } else {
            err = error(send_res);
        }
        return err;
    }

    Error LTR35::digStreamLoadPrepare()  {
        /* если цифровые выходы не разрешены, то загружаем одиночное слово,
           переводящее их в третье состояние */
        Error err = Error::Success();
        bool dout_en = false;
        for (int ch=0; (ch < outDigChannelsCnt()) && !dout_en; ch++) {
            if (outDigValidRamSignal(ch)) {
                dout_en = true;
            }
        }

        /* передача слова для запрета цифровых выходов, если ни один не был разрешен */
        if (!dout_en) {
            unsigned dig_data = LTR35_DIGOUT_WORD_DIS_H | LTR35_DIGOUT_WORD_DIS_L;
            err = protOutSendData(0, 0, &dig_data, 1, 0, 1000);
        }

        return err;
    }


}

