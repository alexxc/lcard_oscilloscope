#include "DeviceConfig.h"
#include "Device.h"
#include "lqmeas/log/Log.h"
#include "lqmeas/ifaces/in/DevAdcConfig.h"
#include "lqmeas/ifaces/out/DevOutSyncConfig.h"
#include "DeviceTypeInfo.h"

namespace LQMeas {
    Error DeviceConfig::loadConfig(QSettings &set, QString key) {
        if (set.childGroups().contains(key)) {
            m_curKey = key;
            set.beginGroup(key);
            QString devName = set.value(configDevTypeKeyName()).toString();
            if (devName == configDevTypeName()) {
                protLoadConfig(set);
                updateConfig();
            } else {
                LQMeasLog->report(LogLevel::Warning, Device::tr("Found configuration for invalid device type: expected %1, found %2")
                                  .arg(configDevTypeName()).arg(devName));
            }
            set.endGroup();
        }
        return Error::Success();
    }

    Error DeviceConfig::saveConfig(QSettings &set, QString key) const {
        Error err;
        if (key.isEmpty()) {
            key = m_curKey;
        }
        set.beginGroup(key);
        set.setValue(configDevTypeKeyName(), configDevTypeName());
        err = protSaveConfig(set);
        set.endGroup();
        return err;
    }

    Error DeviceConfig::updateConfig() {
        if (adcConfig())
            adcConfig()->adcUpdateConfig();
        return protUpdateConfig();
    }



    void DeviceConfig::reportLoadWarning(QString msg) const {
        LQMeasLog->report(LogLevel::Warning, Device::tr("Load config (device %1, key %2)").
                          arg(configDevTypeName()).arg(m_curKey) + ": " + msg);
    }

    void DeviceConfig::updateDevTypeInfo(const DeviceTypeInfo *info) {
        if (info) {
            if (outSyncConfig() && info->out())
                outSyncConfig()->outSyncUpdateDevInfo(info->out());
            if (adcConfig() && info->adc())
               adcConfig()->adcUpdateDevInfo(info->adc());
        }
    }

}
