#include "DevOutSync.h"
#include "DevOutInfo.h"
#include "lqmeas/StdErrors.h"
#include "lqmeas/devs/Device.h"
#include "lqmeas/devs/DeviceTypeInfo.h"
#include "DevOutSyncStatusTracker.h"
#include "SyncModes/DevOutSyncModeImpl.h"
#include "DevOutAsyncDac.h"
#include "DevOutAsyncDig.h"


namespace LQMeas {

    Error DevOutSync::outSyncDacSetGenSignal(int ch, QSharedPointer<DacSignal> signal) {
        return outSyncConfigMutable()->outSyncDacSetSignal(ch, signal);
    }

    QSharedPointer<DacSignal> DevOutSync::outSyncDacGenSignal(int ch) const {
        return outSyncConfig()->outSyncDacSignal(ch);
    }

    Error DevOutSync::outSyncDigSetGenSignal(int ch, QSharedPointer<DigSignal> signal) {
        return outSyncConfigMutable()->outSyncDigSetSignal(ch, signal);
    }

    QSharedPointer<DigSignal> DevOutSync::outSyncDigGenSignal(int ch) const {
        return outSyncConfig()->outSyncDigSignal(ch);
    }

    DevOutSync::~DevOutSync() {
        qDeleteAll(m_modesImpl);
        delete m_statusTracker;
    }

    bool DevOutSync::outSyncHasEnabledChannels() {
        bool ret = false;
        for (int ch_idx = 0; !ret && (ch_idx < outSyncConfig()->outSyncDacChannelsCount()); ch_idx++) {
            if (outSyncConfig()->outSyncDacChEnabled(ch_idx) && outSyncDacGenSignal(ch_idx)) {
                ret = true;
            }
        }
        for (int ch_idx = 0; !ret && (ch_idx < outSyncConfig()->outSyncDacChannelsCount()); ch_idx++) {
            if (outSyncConfig()->outSyncDacChEnabled(ch_idx) && outSyncDacGenSignal(ch_idx)) {
                ret = true;
                break;
            }
        }
        return ret;
    }

    bool DevOutSync::outSyncGenRunning() const {
        return (m_gen_state != GenStateStop) && (m_gen_state != GenStatePrepared);
    }

    Error DevOutSync::outSyncGenPrepare() {
        Error err = Error::Success();
        if (outSyncGenRunning()) {
            err = StdErrors::OutGenRunning();
        } else {
            /* обработка генерируемых в RAM сигналов */
            m_curMode = 0;

            /* получаем список каналов */
            QList<QSharedPointer <DacSignal> > dac_signals;
            QList<QSharedPointer <DigSignal> > dig_signals;
            QList<int> dac_chs;
            QList<int> dig_chs;


            for (int ch =0; ch < outSyncConfig()->outSyncDacChannelsCount(); ch++) {
                if (outSyncDacValidRamSignal(ch)) {
                    dac_signals.append(outSyncDacGenSignal(ch));
                    dac_chs.append(ch);
                }
            }
            for (int ch =0; ch < outSyncConfig()->outSyncDigChannelsCount(); ch++) {
                if (outSyncDigValidRamSignal(ch)) {
                    dig_signals.append(outSyncDigGenSignal(ch));
                    dig_chs.append(ch);
                }
            }


            if (!dac_chs.isEmpty() || dig_signals.isEmpty()) {
                /* ищем реализацию режима генерации из RAM */
                Q_FOREACH(DevOutSyncModeImpl *impl, m_modesImpl) {
                    if (impl->outSyncMode() == outSyncConfig()->outSyncRamMode()) {
                        m_curMode = impl;
                        break;
                    }
                }

                if (!m_curMode) {
                    err = StdErrors::UnsupOutSyncRamMode();
                } else {
                    /* создаем генератор для генерации точек резрешенных сигналов */
                    QSharedPointer<OutRamSignalGenerator> gen(
                                new OutRamSignalGenerator(dac_signals, dac_chs,
                                                          dig_signals, dig_chs));


                    /* назначение сигналов и их корректировака в реализации режимов */
                    err = m_curMode->outSyncModeImplSigSet(gen);
                    /* устанавливаем асинхронные начальные значения */
                    if (err.isSuccess())
                        err = presetVals(gen);
                    /* предзагрузка сигналов */
                    if (err.isSuccess()) {
                        gen->genSignalsInit();
                        err = m_curMode->outSyncModeImplStartPrepare();
                    }


                    QObject::connect(m_curMode, SIGNAL(underflowOccured()), m_statusTracker, SIGNAL(underflowOccured()));
                    QObject::connect(m_curMode, SIGNAL(errorOccured(LQMeas::Error)), m_statusTracker, SLOT(setError(LQMeas::Error)));
                }
            }


            /** @todo Запустить генерацию также аппаратных сигналов */
        }

        if (err.isSuccess())
            m_gen_state = GenStatePrepared;

        return err;
    }

    Error DevOutSync::outSyncGenStart() {
        Error err = Error::Success();
        /* если не выполнен prepare, то выполняем тут */
        if (m_gen_state == GenStateStop)
            err = outSyncGenPrepare();

        if (err.isSuccess() && (m_gen_state != GenStatePrepared))
            err = StdErrors::OutGenRunning();

        if (err.isSuccess() && m_curMode) {
            err = m_curMode->outSyncModeImplStart();
        }

        if (err.isSuccess())
            m_gen_state = GenStateRunning;

        return err;
    }

    Error DevOutSync::outSyncGenStopRequest(unsigned tout) {
        Error err = Error::Success();
        if (m_gen_state == GenStateRunning) {
            if (m_curMode) {
                err = m_curMode->outSyncModeImplStopRequest(tout);
            }            
        }
        if (err.isSuccess())
            m_gen_state = GenStateStopReq;
        return err;
    }

    Error DevOutSync::outSyncGenStop() {
        Error err = Error::Success();
        if (m_gen_state == GenStateRunning)
            err = outSyncGenStopRequest();

        if (err.isSuccess() && (m_gen_state == GenStateStopReq)) {
            if (m_curMode) {
                err = m_curMode->outSyncModeImplStopWait();
            }
            QObject::disconnect(m_curMode, SIGNAL(underflowOccured()), m_statusTracker, SIGNAL(underflowOccured()));
            QObject::disconnect(m_curMode, SIGNAL(errorOccured(LQMeas::Error)), m_statusTracker, SLOT(setError(LQMeas::Error)));
            m_curMode = 0;
        }

        if (err.isSuccess())
            m_gen_state = GenStateStop;

        return err;
    }

    DevOutSync::DevOutSync(Device *dev) : m_dev(dev), m_statusTracker(new DevOutSyncStatusTracker(0, dev)),
        m_gen_state(GenStateStop) {

    }

    Error DevOutSync::presetVals(QSharedPointer<OutRamSignalGenerator> generator) {

        Error err = Error::Success();
        if (m_dev->devTypeInfo()->out()->outSyncPresetSupport()) {
            DevOutAsyncDac *async_dac = m_dev->devOutAsyncDac();
            DevOutAsyncDig *async_dig = m_dev->devOutAsyncDig();

            QVector<double> dac_data;
            QVector<unsigned> dig_data;

            /* генерируем по одной точке сигнала для асинхронного вывода
               начальных значений */
            generator->genSignalsInit();
            generator->genNextData(1, dac_data, dig_data);


            if (async_dac) {
                for (int sig_idx =0; (sig_idx < dac_data.size()) && err.isSuccess(); sig_idx++) {
                    int ch_num = generator->dacSignalChannel(sig_idx);
                    DevOutSyncConfig::DacPreset preset = outSyncConfig()->outSyncDacPreset(ch_num);
                    double point = 0;
                    bool preset_en = false;

                    switch (preset.type) {
						case DevOutSyncConfig::DacPreset::TypeFirstPoint:
							preset_en = true;
							point = dac_data.at(sig_idx);
							break;
						case DevOutSyncConfig::DacPreset::TypeSpecValue:
							preset_en = true;
							point = preset.value;
							break;
						default:
							preset_en = false;
							break;
                    }
                    if (preset_en) {
                       err = async_dac->outAsyncDac(ch_num, point);
                    }
                }
            }

            if (async_dig) {
                int set_mask = 0;
                int set_val = 0;

                for (int sig_idx =0; (sig_idx < generator->digSignalsCount()) && err.isSuccess(); sig_idx++) {
                    int ch_num = generator->digSignalChannel(sig_idx);
                    DevOutSyncConfig::DigPreset preset = outSyncConfig()->outSyncDigPreset(ch_num);
                    unsigned point = 0;
                    bool preset_en = false;

                    switch (preset.type) {
						case DevOutSyncConfig::DigPreset::TypeFirstPoint:
							preset_en = true;
							point = (dig_data.first() >> ch_num) & 1;
							break;
						case DevOutSyncConfig::DigPreset::TypeSpecValue:
							preset_en = true;
							point = preset.value;
							break;
						default:
							preset_en = false;
							break;
                    }

                    if (preset_en) {
                        set_mask |= (1 << ch_num);
                        if (point)
                            set_val  |= (1 << ch_num);
                    }
                }

                if (set_mask != 0) {
                    async_dig->outAsyncDigAll(set_mask, ~set_mask);
                }
            }
        }
        return err;
    }


    bool DevOutSync::outSyncDacValidRamSignal(int ch) const {
        return outSyncConfig()->outSyncDacChEnabled(ch) && outSyncDacGenSignal(ch) && (outSyncConfig()->outSyncDacChGenMode(ch)== DevOutSyncConfig::OutSyncChGenModeRam);
    }

    bool DevOutSync::outSyncDigValidRamSignal(int ch) const {
        return outSyncConfig()->outSyncDigChEnabled(ch) && outSyncDigGenSignal(ch) && (outSyncConfig()->outSyncDigChGenMode(ch)== DevOutSyncConfig::OutSyncChGenModeRam);
    }

    void DevOutSync::outSyncAddMode(DevOutSyncModeImpl *impl) {
        m_modesImpl.append(impl);
    }
}

