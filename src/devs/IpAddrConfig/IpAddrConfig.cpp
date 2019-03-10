#include "IpAddrConfig.h"
#include "lqmeas/devs/resolver/DevicesResolverIpAddr.h"
#include <QHostAddress>

namespace LQMeasStudio {
    IpAddrConfig::IpAddrConfig(QObject *parent) :
        QAbstractTableModel(parent) {

        foreach (LQDevPlugin *plugin, LQMeasStudioEnv->deviceTree()->devPlugins()) {
            auto *ipResolver = dynamic_cast<LQMeas::DevicesResolverIpAddr *>(plugin->resolver());
            if (ipResolver) {
                foreach (QString devname, ipResolver->ipSupportedDevs()) {
                    m_devs[devname] = ipResolver;
                }
            }
        }
    }

    IpAddrConfig::~IpAddrConfig() {
        qDeleteAll(m_records);
    }

    QStringList IpAddrConfig::supportedDevs() const {
        return m_devs.keys();
    }

    int IpAddrConfig::rowCount(const QModelIndex &parent) const {
		// CHECKME: why is `const QModelIndex &parent` not used?
		Q_UNUSED(parent);
		return m_records.size();
    }

    int IpAddrConfig::columnCount(const QModelIndex &parent) const {
		// CHECKME: why is `const QModelIndex &parent` not used?
		Q_UNUSED(parent);
        return COLUMN_CNT;
    }

    QVariant IpAddrConfig::data(const QModelIndex &index, int role) const {
        QVariant ret;
        if (index.isValid()) {
            Record *rec = m_records.at(index.row());
            if ((role == Qt::CheckStateRole) && (index.column()==0))  {
                ret = rec->enabled() ? Qt::Checked : Qt::Unchecked;
            } else {
                switch (index.column()) {
                    case COLUMN_IP_ADDR:
                        if (role == Qt::DisplayRole)
                            ret = rec->ipAddr().toString();
                        break;
                    case COLUMN_DEVNAME:
                        if (role == Qt::DisplayRole)
                            ret = rec->devname();
                        break;
                    default:
                        break;
                }
            }

        }
        return ret;
    }

    Qt::ItemFlags IpAddrConfig::flags(const QModelIndex &index) const {
        if (!index.isValid())
			return nullptr;

        Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

        if (index.column()==0)
            flags |= Qt::ItemIsUserCheckable;
        return flags;
    }

    QVariant IpAddrConfig::headerData(int section, Qt::Orientation orientation, int role) const {
        QVariant ret;
        if ((role == Qt::DisplayRole) && (orientation==Qt::Horizontal)) {
            switch (section) {
                case COLUMN_IP_ADDR:
                    ret = tr("IP address");
                    break;
                case COLUMN_DEVNAME:
                    ret = tr("Device type");
                    break;
                default:
                    break;
            }
        }
        return ret;
    }

    bool IpAddrConfig::setData(const QModelIndex &index, const QVariant &value, int role) {
        bool ret = false;
        if (index.isValid()) {
            if ((index.column()== 0)  && (role == Qt::CheckStateRole)) {
                enableRecord(index.row(), value == Qt::Checked);
            }
        }
        return ret;
    }

    void IpAddrConfig::addRecord(const QString& devname, const QHostAddress& ip_addr, bool enabled) {
        LQMeas::DevicesResolverIpAddr *ipResolver = m_devs[devname];
        if (ipResolver) {
            beginInsertRows(QModelIndex(), m_records.size(), m_records.size());
            m_records.append(new Record(devname, ip_addr, enabled));
            if (enabled)
                ipResolver->addIpAddr(ip_addr);
            endInsertRows();
        }
    }

	void IpAddrConfig::remRecord(signed idx) {
		if (idx < m_records.size()) {
            LQMeas::DevicesResolverIpAddr *ipResolver = m_devs[m_records[idx]->devname()];

            beginRemoveRows(QModelIndex(), idx, idx);
            if (ipResolver) {
                ipResolver->remIpAddr(m_records[idx]->ipAddr());
            }
            m_records.removeAt(idx);
            endRemoveRows();
        }
    }

	void IpAddrConfig::enableRecord(signed idx, bool enable) {
		if (idx < m_records.size()) {
            if (m_records[idx]->enabled()!=enable) {
                LQMeas::DevicesResolverIpAddr *ipResolver = m_devs[m_records[idx]->devname()];
                if (ipResolver) {
                    if (enable) {
                        ipResolver->addIpAddr(m_records[idx]->ipAddr());
                     } else {
                        ipResolver->remIpAddr(m_records[idx]->ipAddr());
                    }
                }
                m_records[idx]->setEnabled(enable);
				emit dataChanged(createIndex(idx, 0, static_cast<void*>(nullptr)), createIndex(idx, COLUMN_CNT-1, static_cast<void*>(nullptr)));
            }
        }
    }

    void IpAddrConfig::loadSettings(QSettings &set) {
        set.beginGroup("IpAddrConfig");
        int size = set.beginReadArray("AddrList");
        for (int i=0; i < size; i++) {
            bool addrOk;
            QHostAddress addr;

            set.setArrayIndex(i);

            addrOk = addr.setAddress(set.value("IpAddr").toString());
            QVariant val = set.value("DevName");
            if (val.isValid() && addrOk) {
                addRecord(val.toString(), addr, set.value("Enabled").toBool());
            }
        }
        set.endArray();
        set.endGroup();
    }

    void IpAddrConfig::saveSettings(QSettings &set) const {
        set.beginGroup("IpAddrConfig");
        set.beginWriteArray("AddrList", m_records.size());
        for (int i=0; i < m_records.size(); i++) {
            set.setArrayIndex(i);
            set.setValue("IpAddr", m_records[i]->ipAddr().toString());
            set.setValue("DevName", m_records[i]->devname());
            set.setValue("Enabled", m_records[i]->enabled());
        }
        set.endArray();
        set.endGroup();
    }

	IpAddrConfig::Record *IpAddrConfig::record(const QString &devname, const QHostAddress &ip_addr) {
		Record *rec = nullptr;
        for (int i=0; (i < m_records.size()) && !rec; i++) {
            if ((m_records.at(i)->devname()==devname) && (m_records.at(i)->ipAddr()==ip_addr)) {
                rec = m_records[i];
            }
        }
        return rec;
    }
}


