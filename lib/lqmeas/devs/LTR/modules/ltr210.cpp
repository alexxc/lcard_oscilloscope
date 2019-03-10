#include "ltr210.h"
#include "lqmeas/lqtdefs.h"
#include "lqmeas/devs/ltr/crates/LtrCrate.h"
#include "lqmeas/log/Log.h"

#include <QElapsedTimer>
#include <QScopedArrayPointer>


namespace LQMeas {
    static double f_ranges[] = {10.,5.,2.,1.,0.5};

    LTR210::LTR210(QSharedPointer<LtrCrate> crate, unsigned slot, QObject *parent) :
        LtrModule(crate, slot, typeModuleID, parent), DevAdc(this, TypeParallel),
        m_receiver(this) {

        adcSetChCnt(typeAdcChCnt);
        LTR210_Init(&m_hnd);
    }


    QString LTR210::typeModuleName() {
        return "LTR210";
    }

    LTR210::~LTR210() {
        if (isOpened())
            close();
    }

    QString LTR210::name() const {
        return typeModuleName();
    }

    QString LTR210::serial() const {
        return QSTRING_FROM_CSTR(m_hnd.ModuleInfo.Serial);
    }

    unsigned LTR210::verFPGA() const {
        return m_hnd.ModuleInfo.VerFPGA;
    }

    unsigned LTR210::verPLD() const {
        return m_hnd.ModuleInfo.VerPLD;
    }

    double LTR210::adcFreq() const {
        return m_hnd.State.AdcFreq;;
    }

    double LTR210::adcChRange(int ch_num) const {
        Q_ASSERT((ch_num>=0) && (ch_num < adcChannelsCnt()));
        return f_ranges[m_hnd.Cfg.Ch[ch_num].Range];
    }

    const double LTR210::adcRangeMaxVal(int range) const {
        return f_ranges[range];
    }

    const double LTR210::adcRangeMinVal(int range) const {
        return -f_ranges[range];
    }

    unsigned LTR210::adcRangesCnt() const {
        return sizeof(f_ranges)/sizeof(f_ranges[0]);
    }

    double LTR210::adcFreqMax() const {
        return 10000000;
    }

    QString LTR210::errorString(int err) const {
        return QSTRING_FROM_CSTR(LTR210_GetErrorString(err));
    }

    bool LTR210::isOpened() const {
        return LTR210_IsOpened(&m_hnd) == LTR_OK;
    }

    bool LTR210::adcIsRunning() const {
        return m_hnd.State.Run ? true : false;
    }

    bool LTR210::adcChEnabled(int ch) const {
        Q_ASSERT((ch>=0) && (ch < adcChannelsCnt()));
        return m_hnd.Cfg.Ch[ch].Enabled;
    }

    Error LTR210::getFrame(double *data, unsigned size, unsigned flags, unsigned tout) {
        Error err = Error::Success();
        INT wrds_cnt = (m_hnd.Cfg.SyncMode==LTR210_SYNC_MODE_CONTINUOUS) ? size :  m_hnd.State.RecvFrameSize;
        QElapsedTimer tmr;

        tmr.start();

        err = waitSof(tmr, tout);
        if (err.isSuccess()) {
            QScopedArrayPointer<DWORD> wrds(new DWORD[wrds_cnt]);
            QScopedArrayPointer<double> tmp_data(((INT)size<(wrds_cnt-1)) ?
                                                 new double[wrds_cnt] : 0);
            unsigned recvd_size;

            err = adcGetData(data, wrds_cnt, 0, tout - tmr.elapsed(), &recvd_size);
            if (err.isSuccess() && (recvd_size != wrds_cnt)) {
                err = error(LTR_ERROR_RECV_INSUFFICIENT_DATA);
                LQMeasLog->error(tr("Received insufficient data: requestet %1, received %2")
                            .arg(QString::number(wrds_cnt))
                            .arg(QString::number(recvd_size)),
                            err, this);
            }

            if (err.isSuccess() && tmp_data) {
                memcpy(data, tmp_data.data(), size);
            }
        }

        return err;
    }

    Error LTR210::waitSof(QElapsedTimer& tmr, unsigned tout) {
        Error err = Error::Success();

        if (m_hnd.Cfg.SyncMode!=LTR210_SYNC_MODE_CONTINUOUS) {
            DWORD evt=LTR210_RECV_EVENT_TIMEOUT;

            while ((evt!=LTR210_RECV_EVENT_SOF) && !tmr.hasExpired(tout) && err.isSuccess()) {
                err = error(LTR210_WaitEvent(&m_hnd, &evt, NULL, (tout - tmr.elapsed())));
                if (!err.isSuccess()) {
                    LQMeasLog->error(tr("Wait event error"), err, this);
                }
            }

            if (err.isSuccess() && (evt!=LTR210_RECV_EVENT_SOF)) {
                err = error(LTR_ERROR_RECV_INSUFFICIENT_DATA);
                LQMeasLog->error(tr("Start of frame waiting timeout"), err, this);
            }
        }
        return err;
    }

    Error LTR210::adcGetData(double *data, unsigned size, unsigned flags, unsigned tout,
                                   unsigned *recvd_size, TLTR210_FRAME_STATUS *status) {
        Error err = Error::Success();
        int wrds_cnt = size;
        unsigned recvd_words=0;
        DWORD* wrds;

        if (err.isSuccess()) {
            err = m_receiver.getFrames(wrds_cnt, tout, &wrds, &recvd_words);
        }

        if (err.isSuccess() && (recvd_words!=0)) {
            DWORD proc_flags = LTR210_PROC_FLAG_VOLT | LTR210_PROC_FLAG_AFC_COR | LTR210_PROC_FLAG_ZERO_OFFS_COR;
            TLTR210_FRAME_STATUS cur_status;
            INT proc_wrds = recvd_words;

            err = error(LTR210_ProcessData(&m_hnd, wrds, data, &proc_wrds,
                                            proc_flags, &cur_status, NULL));
            if (!err.isSuccess()) {
                LQMeasLog->error(tr("Process data error"), err, this);
            } else if (cur_status.Result==LTR210_FRAME_RESULT_ERROR) {
                err = error(LTR_ERROR_RECV);
                LQMeasLog->error(tr("Invalid frame status received"), err, this);
            } else if (proc_wrds > (int)adcEnabledChCnt()){
                for (int ch=0; ch < adcChannelsCnt(); ch++) {
                    if (adcChEnabled(ch))
                        adcSetChStatus(ch, ChStatusOk);
                }
            }

            if (status)
                *status = cur_status;
        }

        if (err.isSuccess() && recvd_size)
            *recvd_size = recvd_words;

        return err;
    }


    Error LTR210::startFrame() {
        Error err = Error::Success();
        if (!m_hnd.State.Run) {
            err = adcStart();
        }

        if (err.isSuccess()) {
            m_receiver.setFrameSize((m_hnd.Cfg.SyncMode == LTR210_SYNC_MODE_CONTINUOUS) ?
                                        adcEnabledChCnt() : m_hnd.Cfg.FrameSize);
        }

        if (err.isSuccess() && (m_hnd.Cfg.SyncMode==LTR210_SYNC_MODE_INTERNAL)) {
            err = error(LTR210_FrameStart(&m_hnd));
            if (!err.isSuccess()) {
                LQMeasLog->error(tr("Software frame start request error"), err, this);
            }
        }
        return err;
    }

    Error LTR210::measZeroAllRanges() {
        TLTR210_CONFIG cfg = m_hnd.Cfg;
        Error err = Error::Success();

        LQMeasLog->info(tr("Start zero offset measurement  for all ranges"), this);
        for (unsigned range = 0; (range < LTR210_RANGE_CNT) && err.isSuccess(); range++) {
            for (int ch=0; ch < adcChannelsCnt(); ch++)
                m_hnd.Cfg.Ch[ch].Range = range;
            err = error(LTR210_MeasAdcZeroOffset(&m_hnd, 0));
            if (err.isSuccess()) {
                for (int ch=0; ch < adcChannelsCnt(); ch++)
                    m_zeroOffs[ch][range] = m_hnd.State.AdcZeroOffset[ch];
            } else {
                LQMeasLog->error(tr("Zero offset measurement error"), err, this);
            }
        }

        Error setErr = adcSetRawConfig(&cfg, sizeof(cfg));
        if (setErr.isSuccess())
            setErr = configure();
        if (err.isSuccess())
            err = setErr;

        if (err.isSuccess())
            LQMeasLog->info(tr("Zero offset measurement done successfully"), this);
        return err;
    }


    Error LTR210::loadFPGA(QString filename) {
        Error err;

        LQMeasLog->info(tr("Start load FPGA firmware"), this);
        err = error(LTR210_LoadFPGA(&m_hnd, filename.toLatin1(), cb_load_porgr, this));

        sendOpProgess(OP_FINISH, loadFpgaOpDescr(), 0, 0, err);

        if (!err.isSuccess()) {
            LQMeasLog->error(tr("Load FPGA firmware error"), err, this);
        } else {
            LQMeasLog->info(tr("FPGA firmware was loaded successfully"));
            emit devDescrChanged();
        }
        return err;
    }

    Error LTR210::protAdcSetRawConfig(const void *cfg, size_t cfg_size) {
        Error err = Error::Success();
        if (cfg_size != sizeof(TLTR210_CONFIG)) {
            err = StdErrors::InvalidAdcConfig();
        } else {
            memcpy(&m_hnd.Cfg, cfg, cfg_size);
        }
        return err;
    }

    Error LTR210::rawWordsReceive(DWORD *wrds, unsigned size, unsigned tout, unsigned *recvd_size)  {
        Error err = Error::Success();
        INT recv_res = LTR210_Recv(&m_hnd, wrds, 0, size, tout);
        if (recv_res >= 0) {
            *recvd_size = recv_res;
        } else {
            err = error(recv_res);
        }
        return err;
    }

    Error LTR210::adcGetData(double *data, unsigned size, unsigned flags,
                           unsigned tout, unsigned *recvd_size) {
        return adcGetData(data, size, flags, tout, recvd_size, 0);
    }

    TLTR *LTR210::channel()  const {
        return &m_hnd.Channel;
    }

    Error LTR210::protOpen() {
        Error err = error(LTR210_Open(&m_hnd, crate()->ltrdIpAddr(), crate()->ltrdTcpPort(),
                                    crate()->serial().toLatin1(), slot()));
        if (err.isSuccess() && (LTR210_FPGAIsLoaded(&m_hnd)!=LTR_OK))
            err = loadFPGA("");

        if (err.isSuccess()) {
            for (int ch=0; ch < adcChannelsCnt(); ch++) {
                for (unsigned range=0; range < LTR210_RANGE_CNT; range++) {
                    m_zeroOffs[ch][range] = 0;
                }
            }
        }

        if (!err.isSuccess())
            LTR210_Close(&m_hnd);

        return err;
    }

    Error LTR210::protClose() {
        return error(LTR210_Close(&m_hnd));
    }

    Error LTR210::protConfigure() {
        Error err = error(LTR210_SetADC(&m_hnd));

        if (err.isSuccess()) {
            for (int ch=0; ch < adcChannelsCnt(); ch++) {
                m_hnd.State.AdcZeroOffset[ch] = m_zeroOffs[ch][m_hnd.Cfg.Ch[ch].Range];
            }
        }
        return err;
    }

    Error LTR210::protLoadConfig(QSettings &set) {
        /** @todo */
        return error(LTR_ERROR_NOT_IMPLEMENTED);
    }

    Error LTR210::protSaveConfig(QSettings &set) const {
        /** @todo */
        return error(LTR_ERROR_NOT_IMPLEMENTED);
    }

    Error LTR210::protAdcStart() {
        return error(LTR210_Start(&m_hnd));
    }

    Error LTR210::protAdcStop() {
        return error(LTR210_Stop(&m_hnd));
    }

    void LTR210::cb_load_porgr(void *cb, TLTR210 *hnd, DWORD done_cnt, DWORD total_cnt) {
        LTR210 *dev = reinterpret_cast<LTR210*>(cb);
        if (dev) {
            dev->sendOpProgess(done_cnt == 0 ? OP_START : OP_PROGR, loadFpgaOpDescr(),
                               done_cnt, total_cnt, Error::Success());
        }
    }
}
