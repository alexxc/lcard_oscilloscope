#include "Device.h"
#include "DeviceConfig.h"
#include "lqmeas/log/Log.h"
#include "DeviceTypeInfo.h"
#include "DeviceInfo.h"

namespace LQMeas {
    Device::~Device() {
        delete m_cfg;
        delete m_info;
    }

    const DeviceTypeInfo *Device::devTypeInfo() const {
        return m_info->type();
    }

    QString Device::name() const {
        return devTypeInfo()->deviceTypeName();
    }

    QString Device::modificationName() const {
        return devTypeInfo()->devceModificationName();
    }

    QString Device::serial() const {
        return devInfo() ? devInfo()->serial() : QString();
    }

    QString Device::devStr() const {
        QString ret = modificationName();
        if (!serial().isEmpty())
            ret.append(", ").append(serial());
        return ret;
    }

    QString Device::devStrFull() const {
        QString ret = devStr();
        if (!location().isEmpty())
            ret.append(" (").append(location()).append(")");
        return ret;
    }

    QString Device::ifaceName() const {
        DeviceInterface cur_iface = iface();
        if (cur_iface == InterfaceUSB)
            return "USB";
        if (cur_iface == InterfacePCI)
            return "PCI/PCIe";
        if (cur_iface == InterfaceEthernet)
            return "Ethernet";
        if (cur_iface == InterfaceLTR )
            return "LTR";
        return "Unknown";
    }

    QString Device::devKey() const {
        /* заменяем символ /, так это разделитель групп в
         * QSettings() */
        return QString(devStr() + ", " + ifaceName()).replace('/', '|');
    }



    Error Device::open() {
        Error err = protOpen();
        if (err.isSuccess()) {
            LQMeasLog->detail(tr("Device opened successfully"), this);
        } else {
            LQMeasLog->error(tr("Device open error"), err, this);
        }
        return err;
    }

    Error Device::close() {
        Error err = protClose();
        if (err.isSuccess()) {
            LQMeasLog->detail(tr("Device closed successfully"), this);
        } else {
            LQMeasLog->error(tr("Device close error"), err, this);
        }
        return err;
    }

	Error Device::configure(const x502Config *cfg) {
        Error err = Error::Success();
		x502Config *setCfg = cfg ? cfg->clone() : m_cfg;

        if (setCfg) {
            setCfg->updateDevTypeInfo(devTypeInfo());
            setCfg->updateConfig();
        }

        err = protConfigure(setCfg);
        if (setCfg != m_cfg) {
            if (err.isSuccess()) {
                delete m_cfg;
                m_cfg = setCfg;
            } else {
                /* при ошибке удаляем новую конфигурации и восстанавливаем старую */
                protConfigure(m_cfg);
            }
        }

        if (err.isSuccess()) {
            LQMeasLog->detail(tr("Device configuration done successfully"), this);
        } else {
            LQMeasLog->error(tr("Device configuration error"), err, this);
        }
        return err;
    }

    Error Device::loadConfig(QSettings &set, QString key) {
        Error err = Error::Success();
        if (m_cfg) {
			QScopedPointer<x502Config> cfg(m_cfg->clone());
            err = cfg->loadConfig(set, key);

            if (err.isSuccess()) {
                err = configure(cfg.data());
            }
        }
        return err;
    }

    Error Device::saveConfig(QSettings &set, QString key) {
        Error err = Error::Success();
        if (currentConfig()) {
            err = currentConfig()->saveConfig(set, key);
            if (key.isEmpty())
                key = currentConfig()->currentKey();
        }
        return err;
    }

    void Device::sendOpProgess(OpStatus status, QString descr, int done, int size, Error err) {
        Q_EMIT opProgress(status, descr, done, size, err);
    }

	Device::Device(x502Config *defaultCfg, const DeviceInfo *info, QObject *parent) :
        QObject(parent), m_cfg(defaultCfg), m_info(info) {
        if (m_cfg)
            m_cfg->updateConfig();
        qRegisterMetaType<LQMeas::Error>("LQMeas::Error");
    }

    void Device::setDevicInfo(const DeviceInfo *info)  {
        if (info && (info != m_info)) {
            const DeviceInfo *prev_info = m_info;
            m_info = info;
            delete prev_info;
            if (m_cfg)
                m_cfg->updateDevTypeInfo(info->type());
        }

    }

}
