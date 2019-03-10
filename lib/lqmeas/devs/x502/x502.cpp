#include "x502.h"
#include "x502Info.h"
#include "x502Config.h"
#include "lqmeas/lqtdefs.h"
#include "lqmeas/StdErrors.h"
#include "lqmeas/log/Log.h"
#include "lqmeas/ifaces/out/SyncModes/DevOutSyncModeImplStream.h"
#include "lqmeas/ifaces/out/SyncModes/DevOutSyncModeImplCycle.h"
#include <QMutexLocker>



namespace LQMeas {

	x502::x502(const t_x502_devrec *devRec, const x502Info *info) : DevAdc(this, info->devspecTypeInfo()->adcChannelsCnt()),
        Device(new x502Config(), info), DevOutSync(this),
		m_receiver(this), m_sender(this), m_opened(false), m_outStreams(0), m_outRunning(false),
		m_inRunning(false) {

		m_devrec = *devRec;
        m_hnd = X502_Create();

        outSyncAddMode(new DevOutSyncModeImplStream(this, this));
        outSyncAddMode(new DevOutSyncModeImplCycle(this, this));
    }

    void x502::setNewDevrecord(const t_x502_devrec *devrec) {
        X502_FreeDevRecordList(&m_devrec,1);
        m_devrec = *devrec;
    }

    x502Config *x502::mutableSpecConfig() const {
        return static_cast<x502Config *>(mutableConfig());
    }

    x502::~x502() {
        X502_Free(m_hnd);
        X502_FreeDevRecordList(&m_devrec,1);
    }

    QString x502::location() const {
        return QString::fromUtf8(m_devrec.location);
    }



    Device::DeviceInterface x502::iface() const {
        return m_devrec.iface == X502_IFACE_PCI ? Device::InterfacePCI :
               m_devrec.iface == X502_IFACE_USB ? Device::InterfaceUSB :
               m_devrec.iface == X502_IFACE_ETH ? Device::InterfaceEthernet :
                                                  Device::InterfaceUnknown;
    }


    bool x502::isOpened() const {
        return m_opened;
    }

    bool x502::supportCustomOutFreq() const {
        return X502_CheckFeature(m_hnd, X502_FEATURE_OUT_FREQ_DIV) == X502_ERR_OK;
    }

    bool x502::adcIsRunning() const {
        return X502_IsRunning(m_hnd) == X502_ERR_OK ? true : false;
    }

    Error x502::adcEnable() {
        return error(X502_StreamsEnable(m_hnd, X502_STREAM_ADC));
    }

    Error x502::adcDisable() {
        return error(X502_StreamsDisable(m_hnd, X502_STREAM_ADC));
    }


	Error x502::adcGetData(double *data, unsigned size, unsigned flags, unsigned tout, unsigned *recvd_size) {
		Q_UNUSED(flags);
        Error err = Error::Success();
		unsigned recvd_words=0;

        uint32_t *wrds;

        err = m_receiver.getFrames(size, tout, &wrds, &recvd_words);


        if ((err.isSuccess()) && (recvd_words!=0)) {
            uint32_t proc_flags = X502_PROC_FLAGS_VOLT;
            uint32_t proc_wrds = recvd_words;


            err = error(X502_ProcessAdcData(m_hnd, wrds, data, &proc_wrds, proc_flags));
            if (!err.isSuccess()) {
                LQMeasLog->error(tr("Process data error"), err, this);
            }

            recvd_words = proc_wrds;
        }

        if (err.isSuccess() && recvd_size)
            *recvd_size = recvd_words;

        return err;
    }


    Error x502::error(int err, QString descr) {
        return err == X502_ERR_OK ? Error::Success() :
                                    Error(err, (descr.isEmpty() ? descr : descr + ": ")
                                          + QSTRING_FROM_CSTR(X502_GetErrorString(err)),
                                          Error::ERRTYPE_DEVNATIVE_X502);
    }

    const x502Config *x502::devSpecConfig() const {
        return static_cast<const x502Config *>(currentConfig());
    }

    const x502Info *x502::devSpecInfo() const {
        return static_cast<const x502Info*>(devInfo());
    }

    Error x502::protOpen() {
        t_x502_info devinfo;
        Error err = error(X502_OpenByDevRecord(m_hnd, &m_devrec));
        if (err.isSuccess()) {
            err = error(X502_GetDevInfo(m_hnd, &devinfo));
        }
        if (err.isSuccess()) {
            m_opened = true;
            m_outStreams = 0;
            m_outRunning = false;
            m_inRunning = false;
            setDevicInfo(createInfo(&devinfo));
        } else {
            X502_Close(m_hnd);
        }
        return err;
    }

    Error x502::protClose() {
        Error err = error(X502_Close(m_hnd));
        m_opened = false;
        return err;
    }




    Error x502::protConfigure(const DeviceConfig *cfg) {
        Error err = Error::Success();
        const x502Config *setCfg = dynamic_cast<const x502Config *>(cfg);
        if (setCfg) {
            unsigned lch_cnt=0;

            if (!setCfg->bfIsEnabled()) {
                err = error(X502_SetMode(m_hnd, X502_MODE_FPGA), tr("Setup FPGA mode"));
            } else {
                err = error(X502_BfLoadFirmware(m_hnd, QSTRING_TO_CSTR(setCfg->bfFirmwareFilename())),
                            tr("Load blackfin firmware"));
            }

			for (signed ch=0; (ch < devTypeInfo()->adc()->adcChannelsCnt()) && err.isSuccess(); ++ch) {
                if (setCfg->adcChEnabled(ch)) {
                    x502Config::AdcChMode mode = setCfg->adcChMode(ch);
                    uint32_t mode_code = mode == x502Config::AdcChModeComm ? X502_LCH_MODE_COMM :
                                     (mode == x502Config::AdcChModeZero ? X502_LCH_MODE_ZERO :
                                                                             X502_LCH_MODE_DIFF);

					err = error(X502_SetLChannel(m_hnd, lch_cnt++, static_cast<unsigned>(ch),
                                                 mode_code,
												 static_cast<unsigned>(setCfg->adcChRangeNum(ch)),
												 static_cast<unsigned>(setCfg->adcChAvg(ch))),
                                tr("Setup channel %1 parameters").arg(QString::number(ch+1)));
                }
            }

            if (err.isSuccess()) {
                err = error(X502_SetLChannelCount(m_hnd, lch_cnt), tr("Setup channels count"));
            }
			if (err.isSuccess()) {
                err = error(X502_SetSyncMode(m_hnd, setCfg->freqMode()), tr("Setup sync mode"));
			}
			if (err.isSuccess()) {
                err = error(X502_SetSyncStartMode(m_hnd, setCfg->startMode()), tr("Setup sync start mode"));
			}
			if (err.isSuccess()) {
                err = error(X502_SetRefFreq(m_hnd, setCfg->refFreqInternal()), tr("Setup reference frequency"));
			}
			if (err.isSuccess()) {
                err = error(X502_SetExtRefFreqValue(m_hnd, setCfg->refFreqExternal()), tr("Setup external reference frequency value"));
			}

            if (err.isSuccess()) {
                double adc_freq = setCfg->adcFreq();
                double frame_freq = setCfg->adcChFreq();
                err = error(X502_SetAdcFreq(m_hnd, &adc_freq, &frame_freq), tr("Setup adc frequency"));
            }

            if (err.isSuccess()) {
                /* Изначально данная возможность не была реализована в прошивке
                   blackfin, не считаем этот случай ошибокой */
                int32_t err_code = X502_SetDigInPullup(m_hnd, setCfg->pullups());
                if (err_code == X502_ERR_NOT_IMPLEMENTED)
                    err_code = X502_ERR_OK;
                err = error(err_code, tr("Setup pullup resistors"));
            }

            if (err.isSuccess()) {
                double out_freq = setCfg->outSyncGenFreq();
                err = error(X502_SetOutFreq(m_hnd, &out_freq), tr("Setup output frequency"));
            }
        } else {
            err = StdErrors::InvalidConfigType();
        }

		if (err.isSuccess()) {
            err = error(X502_Configure(m_hnd, 0));
		}
        return err;
    }


    Error x502::protAdcStart() {
        Error err = Error::Success();
		m_receiver.setFrameSize(static_cast<signed>(adcConfig()->adcEnabledChCnt()));

        QMutexLocker lock(&m_streamLock);
		if (!m_outRunning) {
            err = error(X502_StreamsStart(m_hnd));
		}
		if (err.isSuccess()) {
            m_inRunning = true;
		}
        return err;
    }

    Error x502::protAdcStop() {
        QMutexLocker lock(&m_streamLock);
        m_inRunning = false;
        return m_outRunning == 0 ? error(X502_StreamsStop(m_hnd)) : Error::Success();
    }

    Error x502::protInAsyncDig(unsigned *val) {
        return error(X502_AsyncInDig(m_hnd, val));
    }

    Error x502::rawWordsReceive(uint32_t *wrds, unsigned size, unsigned tout, unsigned *recvd_size) {
        Error err = Error::Success();
        int32_t recv_res = X502_Recv(m_hnd, wrds, size, tout);
        if (recv_res >= 0) {
			*recvd_size = static_cast<unsigned>(recv_res);
        } else {
            err = error(recv_res);
        }
        return err;
    }

    Error x502::setOutStreams()  {
        int32_t err_code = X502_ERR_OK;
        uint32_t new_streams = 0;
        if (devTypeInfo()->out()->outDacChannelsCnt() >= X502_DAC_CH_CNT) {
            if (outSyncDacValidRamSignal(0))
                new_streams |= X502_STREAM_DAC1;
            if (outSyncDacValidRamSignal(1))
                new_streams |= X502_STREAM_DAC2;
        }
        bool outDig = false;
        for (int ch=0; (ch < devTypeInfo()->out()->outDigChannelsCnt()) && !outDig; ch++) {
            if (outSyncDigValidRamSignal(ch))
                outDig = true;
        }
        if (outDig)
            new_streams |= X502_STREAM_DOUT;

        uint32_t cur_stream;
        /* Явно получаем список разрешенных потоков, а не используем из
           m_outStreams, чтобы избежать возможной разсинхронизации в случае
           работы с BlackFin */
        err_code = X502_GetEnabledStreams(m_hnd, &cur_stream);
        if (err_code == X502_ERR_OK) {
            cur_stream &= X502_STREAM_ALL_OUT;

            uint32_t streams_en = new_streams & ~cur_stream;
            uint32_t streams_dis = ~new_streams & cur_stream;

            if (streams_en) {
                err_code = X502_StreamsEnable(m_hnd, streams_en);
            }
            if (streams_dis && (err_code == X502_ERR_OK)) {
                err_code = X502_StreamsDisable(m_hnd, streams_dis);
            }
        }

        if (err_code == X502_ERR_OK) {
            m_outStreams = new_streams;
        }


        return error(err_code);
    }



	Error x502::protOutAsyncDac(unsigned int ch, double val) {
		return error(X502_AsyncOutDac(m_hnd, static_cast<unsigned>(ch), val, X502_DAC_FLAGS_VOLT | X502_DAC_FLAGS_CALIBR));
    }

    Error x502::protOutAsyncDig(unsigned val, unsigned mask) {
        return error(X502_AsyncOutDig(m_hnd, val, mask));
    }

    Error x502::rawWordsSend(const uint32_t *wrds, unsigned size, unsigned tout, unsigned *sent_size) {
        Error err = Error::Success();
        int send_res = X502_Send(m_hnd, wrds, size, tout);
        if (send_res >= 0) {
			*sent_size = static_cast<unsigned>(send_res);
        } else {
            err = error(send_res);
        }
        return err;
    }

    DevOutSyncConfig *x502::outSyncConfigMutable() const {
        return static_cast<DevOutSyncConfig*>(mutableSpecConfig());
    }

    Error x502::privOutStreamInit() {
        return setOutStreams();
    }

    Error x502::privOutStreamStart() {
        Error err = Error::Success();
        QMutexLocker lock(&m_streamLock);
        if (!m_inRunning)
            err = error(X502_StreamsStart(m_hnd));
        if (err.isSuccess())
            m_outRunning = true;
        return err;
    }

    Error x502::privOutStreamStop(unsigned tout) {
		Q_UNUSED(tout);
        QMutexLocker lock(&m_streamLock);
        m_outRunning = false;
        return m_inRunning == 0 ? error(X502_StreamsStop(m_hnd)) : Error::Success();
    }

    unsigned x502::privOutStreamPreloadPoints() {
		return unsigned(outSyncConfig()->outSyncGenFreq());
    }

	Error x502::privOutSyncSendData(const double *dac_data, unsigned dac_size, const unsigned *digData, unsigned dig_size, unsigned flags, unsigned tout) {
		Q_UNUSED(flags);
        Error err = Error::Success();
        if (tout == 0) {
            tout = 3000 + (dac_size + dig_size)/1000;
        }

        if (m_sender.unsentWordsCnt()) {
            err = m_sender.flushData(tout);
        }

        if (err.isSuccess()) {
            if (m_sender.unsentWordsCnt()!=0) {
                err = StdErrors::SendBusy();
            } else {
                uint32_t snd_dac_req_size = dac_size;
                uint32_t snd_dig_req_size = dig_size;
                uint32_t snd_size = snd_dac_req_size  + snd_dig_req_size;
                QScopedArrayPointer<uint32_t> wrds (new uint32_t[snd_size]);

                if ((m_outStreams & X502_STREAM_DAC1) && (m_outStreams & X502_STREAM_DAC2)) {
                    dac_size/=2;
                    QScopedArrayPointer<double> dac1 (new double[dac_size]);
                    QScopedArrayPointer<double> dac2 (new double[dac_size]);
                    for (unsigned i=0; i < dac_size; i++) {
                        dac1[i] = *dac_data++;
                        dac2[i] = *dac_data++;
                    }
					err = error(X502_PrepareData(m_hnd, dac1.data(), dac2.data(), const_cast<const uint32_t*>(digData),
                                                 dac_size, X502_DAC_FLAGS_CALIBR  | X502_DAC_FLAGS_VOLT,
                                                 wrds.data()));
                } else {
					err = error(X502_PrepareData(m_hnd, dac_data, dac_data, const_cast<const uint32_t*>(digData),
                                                 dac_size, X502_DAC_FLAGS_CALIBR  | X502_DAC_FLAGS_VOLT,
                                                 wrds.data()));
                }

                if (!err.isSuccess()) {
                    LQMeasLog->error(tr("Prepare data error"), err, this);
                } else {
                    m_sender.setFrameSize(snd_size);
					err = m_sender.putFrames(wrds.data(), snd_size, tout, nullptr);
                }
            }
        }
        return err;

    }

    Error x502::privOutSyncFlushData(unsigned tout) {
        Error err = Error::Success();

        if (m_sender.unsentWordsCnt()) {
            err = m_sender.flushData(tout);
        }

        if (err.isSuccess()) {
			if (m_sender.unsentWordsCnt() != 0) {
                err = StdErrors::SendBusy();
            }
        }

        if (err.isSuccess()) {
            m_sender.setFrameSize(0);
        }
        return err;
    }

    bool x502::privOutSyncUnderflowOccured() const {
        bool ret = false;
        uint32_t status;
        int32_t res = X502_OutGetStatusFlags(m_hnd, &status);
        if (res == X502_ERR_OK) {
            if (status & X502_OUT_STATUS_FLAG_BUF_WAS_EMPTY)
                ret = true;
        }
        return ret;
    }

    Error x502::privOutCycleLoadStart(const OutRamSignalGenerator *generator, size_t size) {
        Error err = setOutStreams();
        if (err.isSuccess()) {
			unsigned int chCnt = static_cast<unsigned>(generator->dacSignalsCount() + (generator->digSignalsCount() == 0 ? 0 : 1));
            err = error(X502_OutCycleLoadStart(m_hnd, size*chCnt));
        }
        return err;
    }

    Error x502::privOutCycleLoadFinish() {
        return error(X502_OutCycleSetup(m_hnd, !m_outRunning ? X502_OUT_CYCLE_FLAGS_WAIT_DONE : 0));
    }

    Error x502::privOutCycleGenStart() {
        Error err = Error::Success();
        QMutexLocker lock(&m_streamLock);
        if (!m_inRunning && !m_outRunning)
            err = error(X502_StreamsStart(m_hnd));
        if (err.isSuccess())
            m_outRunning = true;
        return err;
    }

    Error x502::privOutCycleGenStopRequest(unsigned tout) {
        return error(X502_OutCycleStop(m_hnd, tout));
    }

    Error x502::privOutCycleGenStop() {
        Error err = Error::Success();
        QMutexLocker lock(&m_streamLock);
        m_outRunning = false;
        /** @note может есть смысл делать X502_OutCycleCheckSetupDone() */
        if (!m_inRunning) {
            /* останавливаем поток только если поток ввода тоже уже остановлен */
            err = error(X502_StreamsStop(m_hnd));
        }
        return err;
    }
}
