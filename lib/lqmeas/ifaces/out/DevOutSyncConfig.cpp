#include "DevOutSyncConfig.h"
#include "DevOutInfo.h"
#include "OutSignalFabric.h"

namespace LQMeas {

    DevOutSyncConfig::DevOutSyncConfig(const DevOutInfo *devinfo)     {
        outSyncUpdateDevInfo(devinfo);
    }

    DevOutSyncConfig::DevOutSyncConfig(const DevOutSyncConfig *cfg) {
        m_outRamSyncMode = cfg->m_outRamSyncMode;
        m_dacParams = cfg->m_dacParams;
        m_digParams = cfg->m_digParams;

		m_ram_mode_configurable = cfg->m_ram_mode_configurable;
    }

    void DevOutSyncConfig::outSyncDacChEnable(int ch, bool en) {
        if (outSyncDacChEnabled(ch) != en)
            m_dacParams[ch].enabled = en;
    }

    void DevOutSyncConfig::outSyncDigChEnable(int ch, bool en) {
        m_digParams[ch].enabled = en;
    }

    Error DevOutSyncConfig::outSyncDacSetSignal(int ch, QSharedPointer<DacSignal> signal) {
        m_dacParams[ch].sig = signal;
        return Error::Success();
    }

    Error DevOutSyncConfig::outSyncDacSetPreset(int ch, DevOutSyncConfig::DacPreset &val)     {
        m_dacParams[ch].preset = val;
        return Error::Success();
    }

    Error DevOutSyncConfig::outSyncDigSetSignal(int ch, QSharedPointer<DigSignal> signal) {
        m_digParams[ch].sig = signal;
        return Error::Success();
    }

    void DevOutSyncConfig::outSyncClearSignals() {
        for (int ch = 0; ch < outSyncDacChannelsCount(); ch++) {
            m_dacParams[ch].sig.clear();
        }
        for (int ch = 0; ch < outSyncDigChannelsCount(); ch++) {
            m_digParams[ch].sig.clear();
        }
    }

    Error DevOutSyncConfig::outSyncDigSetPreset(int ch, DevOutSyncConfig::DigPreset &val) {
        m_digParams[ch].preset = val;
        return Error::Success();
    }

    Error DevOutSyncConfig::setOutSyncDacChGenMode(int ch, DevOutSyncConfig::OutSyncChGenMode mode) {
        m_dacParams[ch].gen_mode = mode;
        return Error::Success();
    }

    Error DevOutSyncConfig::outSyncSetRamMode(DevOutSyncConfig::OutSyncRamMode mode) {
        m_outRamSyncMode = mode;
        return Error::Success();
    }

    Error DevOutSyncConfig::outSyncLoadSignalConfig(QSettings &set) {
        if (outSyncDacChannelsCount() != 0) {
            int size = set.beginReadArray("DacChSignals");
            for (int ch = 0; (ch < outSyncDacChannelsCount()) && (ch < size); ch++) {
                set.setArrayIndex(ch);
                outSyncDacChEnable(ch, set.value("en").toBool());
                outSyncDacSetSignal(ch, OutSignalFabric::instance()->createDacSignal(set));

                set.beginGroup("PresetValue");
                DacPreset preset;
                preset.type = (DacPreset::Type)set.value("type").toInt();
                preset.value = set.value("value").toDouble();
                set.endGroup();
                outSyncDacSetPreset(ch, preset);

            }
            set.endArray();
        }

        if (outSyncDigChannelsCount() != 0) {
            int size = set.beginReadArray("DoutChSignals");
            for (int ch = 0; (ch < outSyncDigChannelsCount()) && (ch < size); ch++) {
                set.setArrayIndex(ch);
                outSyncDigChEnable(ch, set.value("en").toBool());
                outSyncDigSetSignal(ch, OutSignalFabric::instance()->createDigSignal(set));

                set.beginGroup("PresetValue");
                DigPreset preset;
                preset.type = (DigPreset::Type)set.value("type").toInt();
                preset.value = set.value("value").toDouble();
                set.endGroup();
                outSyncDigSetPreset(ch, preset);
            }
            set.endArray();
        }

        if (m_ram_mode_configurable) {
            bool ok;
            int intval  = set.value("OutMode", DevOutSyncConfig::OutSyncRamModeStream).toInt(&ok);
            if (ok && ((intval == DevOutSyncConfig::OutSyncRamModeStream) || (intval == DevOutSyncConfig::OutSyncRamModeCycle))) {
                outSyncSetRamMode((DevOutSyncConfig::OutSyncRamMode)intval);
            }
        }

        return Error::Success();
    }

    Error DevOutSyncConfig::outSyncSaveSignalConfig(QSettings &set) const {
        if (outSyncDacChannelsCount() != 0) {
            set.beginWriteArray("DacChSignals", outSyncDacChannelsCount());
            for (int ch = 0; ch < outSyncDacChannelsCount(); ch++) {
                set.setArrayIndex(ch);
                QSharedPointer<DacSignal> sig = outSyncDacSignal(ch);
                set.setValue("en", outSyncDacChEnabled(ch));
                if (sig) {
                    OutSignalFabric::instance()->saveSignal(sig, set);
                }

                set.beginGroup("PresetValue");
                DacPreset preset = outSyncDacPreset(ch);
                set.setValue("type", preset.type);
                set.setValue("value", preset.value);
                set.endGroup();
            }
            set.endArray();
        }

        if (outSyncDigChannelsCount() != 0) {
            set.beginWriteArray("DoutChSignals", outSyncDigChannelsCount());
            for (int ch = 0; ch < outSyncDigChannelsCount(); ch++) {
                set.setArrayIndex(ch);
                QSharedPointer<DigSignal> sig = outSyncDigSignal(ch);
                set.setValue("en", outSyncDigChEnabled(ch));
                if (sig) {
                    OutSignalFabric::instance()->saveSignal(sig, set);
                }

                set.beginGroup("PresetValue");
                DigPreset preset = outSyncDigPreset(ch);
                set.setValue("type", preset.type);
                set.setValue("value", preset.value);
                set.endGroup();
            }
            set.endArray();
        }

        if (m_ram_mode_configurable)
            set.setValue("OutMode", (int)outSyncRamMode());

        return Error::Success();
    }

    void DevOutSyncConfig::outSyncUpdateDevInfo(const DevOutInfo *devinfo) {
        m_ram_mode_configurable = devinfo->outSyncRamModeIsConfigurable();
        m_dacParams.resize(devinfo->outDacChannelsCnt());
        m_digParams.resize(devinfo->outDigChannelsCnt());
    }


    DevOutSyncConfig::DacChParams::DacChParams(const DevOutSyncConfig::DacChParams *ch_par) {
        init(ch_par->enabled, ch_par->sig, ch_par->preset, ch_par->gen_mode);
    }

    DevOutSyncConfig::DacChParams::DacChParams(bool init_en, QSharedPointer<DacSignal> init_sig, DevOutSyncConfig::DacPreset init_preset, OutSyncChGenMode gen_mode) {
        init(init_en, init_sig, init_preset, gen_mode);
    }

    DevOutSyncConfig::DigChParams::DigChParams(const DevOutSyncConfig::DigChParams *ch_par) {
        init(ch_par->enabled, ch_par->sig, ch_par->preset, ch_par->gen_mode);
    }

    DevOutSyncConfig::DigChParams::DigChParams(bool init_en, QSharedPointer<DigSignal> init_sig, DevOutSyncConfig::DigPreset init_preset, DevOutSyncConfig::OutSyncChGenMode gen_mode) {
        init(init_en, init_sig, init_preset, gen_mode);
    }

}
