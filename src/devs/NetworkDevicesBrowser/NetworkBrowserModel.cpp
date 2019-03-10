#include "NetworkBrowserModel.h"
#include "LQMeasStudio.h"
#include "lqmeas/devs/resolver/DevicesResolverIpAddr.h"

namespace LQMeasStudio {

	NetworkBrowserModel::NetworkBrowserModel(QObject *parent) :  m_enabled(false), m_running(false) {
		// CHECKME: why is `QObject *parent` not used?
		Q_UNUSED(parent);
        foreach (LQDevPlugin *plugin, LQMeasStudioEnv->deviceTree()->devPlugins()) {
            auto *ipResolver = dynamic_cast<LQMeas::DevicesResolverIpAddr *>(plugin->resolver());
            if (ipResolver) {
                LQMeas::DeviceNetworkBrowser *browser = ipResolver->netBrowser();
                if (browser) {
                    connect(browser, SIGNAL(stateChanged(QSharedPointer<LQMeas::DeviceSvcRecord>,LQMeas::DeviceNetworkBrowser::Event)),
                            SLOT(onStateChanged(QSharedPointer<LQMeas::DeviceSvcRecord>,LQMeas::DeviceNetworkBrowser::Event)));
                    connect(browser, SIGNAL(error(LQMeas::Error,QString)), SIGNAL(error(LQMeas::Error)));
                    m_browsers.append(browser);

                    foreach (QString devname, ipResolver->ipSupportedDevs()) {
                        m_devsReolvers[devname] = ipResolver;
                    }
                }
            }
        }
    }

    NetworkBrowserModel::~NetworkBrowserModel() = default;

    void NetworkBrowserModel::browseStart() {
        if (m_enabled) {
            foreach (LQMeas::DeviceNetworkBrowser *browser, m_browsers)
                browser->start();

            m_running = true;
        }
    }

    void NetworkBrowserModel::browseStop() {
        foreach (LQMeas::DeviceNetworkBrowser *browser, m_browsers) {
            browser->stopRequest();
        }

        foreach (LQMeas::DeviceNetworkBrowser *browser, m_browsers) {
            browser->stopWiat();
        }
        m_running = false;
    }

    void NetworkBrowserModel::browseClearResult() {
        if (!m_devrecs.isEmpty()) {
            beginRemoveRows(QModelIndex(), 0, m_devrecs.size()-1);
            QList<svcRecState> recCopy = m_devrecs;
            m_devrecs.clear();
            endRemoveRows();

            /* удаляем все записи из списка для открытия */
            foreach (svcRecState state, recCopy) {
                if (state.en) {
                    LQMeas::DevicesResolverIpAddr *ipResolver = m_devsReolvers[state.rec->deviceName()];
                    if (ipResolver) {
                        ipResolver->remSvcRec(state.rec);
                    }
                }
            }
        }
    }

    void NetworkBrowserModel::browseEnable(bool en) {
        m_enabled = en;
        if (en) {
            if (!m_running)
                browseStart();
        } else {
            if (m_running)
                browseStop();
            browseClearResult();
        }

    }

    bool NetworkBrowserModel::allEnabledDevsPresent() const {
        return notPresentDevs().empty();
    }

    void NetworkBrowserModel::removeNotPresentDevs() {
        QStringList remDevices = notPresentDevs();
        foreach (QString name, remDevices) {
            m_enabledDevices.removeAll(name);
        }
    }

    QStringList NetworkBrowserModel::notPresentDevs() const {
        QStringList remDevices = m_enabledDevices;

        for (int i=0; (i < m_devrecs.size()) && !remDevices.isEmpty(); i++) {
            remDevices.removeAll(m_devrecs[i].rec->instanceName());
        }
        return remDevices;
    }

    void NetworkBrowserModel::loadSettings(QSettings &set) {
        if (m_running) {
            browseStop();
        }
        browseClearResult();

        set.beginGroup("NetworkBrowserModel");
        bool en = set.value("Enabled", true).toBool();
        m_enabledDevices.clear();
        int size = set.beginReadArray("EnabledDevs");
        for (int i=0; i < size; i++) {
            set.setArrayIndex(i);
            QString name = set.value("InstanceName").toString();
            if (!name.isEmpty())
                m_enabledDevices.append(name);
        }
        set.endArray();
        set.endGroup();

        browseEnable(en);
    }

    void NetworkBrowserModel::saveSettings(QSettings &set) const {
        set.beginGroup("NetworkBrowserModel");
        set.setValue("Enabled", m_enabled);
        set.beginWriteArray("EnabledDevs", m_enabledDevices.size());
        for (int i=0; i < m_enabledDevices.size(); i++) {
            set.setArrayIndex(i);
            set.setValue("InstanceName", m_enabledDevices[i]);
        }
        set.endArray();
        set.endGroup();
    }

    int NetworkBrowserModel::rowCount(const QModelIndex &parent) const {
		// CHECKME: why is `const QModelIndex &parent` not used?
		Q_UNUSED(parent);
        return m_devrecs.size();
    }

    int NetworkBrowserModel::columnCount(const QModelIndex &parent) const {
		// CHECKME: why is `const QModelIndex &parent` not used?
		Q_UNUSED(parent);
		return COLUMN_CNT;
    }

    QVariant NetworkBrowserModel::data(const QModelIndex &index, int role) const {
        QVariant ret;
        if (index.isValid()) {
            svcRecState recState = m_devrecs.at(index.row());
            switch (index.column()) {
                case COLUMN_INSTANCE:
                    if (role == Qt::CheckStateRole) {
                        ret = recState.en ? Qt::Checked : Qt::Unchecked;
                    } else if (role == Qt::DisplayRole) {
                        ret = recState.rec->instanceName();
                    }
                    break;
                case COLUMN_DEVNAME:
                    if (role == Qt::DisplayRole)
                        ret = recState.rec->deviceName();
                    break;
                case COLUMN_DEVSERIAL:
                    if (role == Qt::DisplayRole)
                        ret = recState.rec->deviceSerial();
                    break;
                case COLUMN_IPADDR:
                    if (role == Qt::DisplayRole)
                        ret = recState.rec->hostAddress().toString();
                    break;
                default:
                    break;
            }
        }
        return ret;
    }

    Qt::ItemFlags NetworkBrowserModel::flags(const QModelIndex &index) const {
        if (!index.isValid())
			return nullptr;

        Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

        switch (index.column()) {
            case 0:
                flags |= Qt::ItemIsUserCheckable;
                break;
            default:
                break;
        }
        return flags;
    }

    QVariant NetworkBrowserModel::headerData(int section, Qt::Orientation orientation, int role) const {
        QVariant ret;
        if ((role == Qt::DisplayRole) && (orientation==Qt::Horizontal)) {
            switch (section) {
                case COLUMN_INSTANCE:
                    ret = tr("Instance name");
                    break;
                case COLUMN_DEVNAME:
                    ret = tr("Device name");
                    break;
                case COLUMN_DEVSERIAL:
                    ret = tr("Device serial");
                    break;
                case COLUMN_IPADDR:
                    ret = tr("IP address");
                    break;
                default:
                    break;
            }
        }
        return ret;
    }

    bool NetworkBrowserModel::setData(const QModelIndex &index, const QVariant &value, int role) {
        bool ret = false;
        if (index.isValid()) {
            switch (index.column()) {
                case 0:
                    if (role == Qt::CheckStateRole) {
                        bool en = value == Qt::Checked;
                        if (en != m_devrecs[index.row()].en) {
                            m_devrecs[index.row()].en = en;

                            QSharedPointer <LQMeas::DeviceSvcRecord> rec = m_devrecs[index.row()].rec;
                            LQMeas::DevicesResolverIpAddr *ipResolver = m_devsReolvers[rec->deviceName()];
                            if (ipResolver) {
                                if (en) {
                                    ipResolver->addSvcRec(rec);
                                    m_enabledDevices.append(rec->instanceName());
                                } else {
                                    ipResolver->remSvcRec(rec);
                                    m_enabledDevices.removeAll(rec->instanceName());
                                }
                            }
                        }
                    }
                    ret = true;
                    break;
            }
        }
        return ret;
    }

    void NetworkBrowserModel::onStateChanged(const QSharedPointer<LQMeas::DeviceSvcRecord>&  svcRec,
                                               LQMeas::DeviceNetworkBrowser::Event event) {
        if (event == LQMeas::DeviceNetworkBrowser::EventAdd) {
            svcRecState state;
            beginInsertRows(QModelIndex(), m_devrecs.size(), m_devrecs.size());
            state.rec = svcRec;
            state.en = m_enabledDevices.contains(svcRec->instanceName());
            m_devrecs.append(state);            
            endInsertRows();
            if (state.en) {
                LQMeas::DevicesResolverIpAddr *ipResolver = m_devsReolvers[state.rec->deviceName()];
                if (ipResolver) {
                    ipResolver->addSvcRec(state.rec);
                }
            }
        } else if ((event == LQMeas::DeviceNetworkBrowser::EventChange) ||
            (event == LQMeas::DeviceNetworkBrowser::EventRemoved)) {

            int fnd_idx = -1;
            for (int i=0; (i < m_devrecs.size()) && (fnd_idx < 0); i++) {
                if (m_devrecs[i].rec->isEqual(svcRec.data()))
                    fnd_idx = i;
            }
            if (fnd_idx >= 0) {
                QSharedPointer <LQMeas::DeviceSvcRecord> rec = m_devrecs[fnd_idx].rec;
                LQMeas::DevicesResolverIpAddr *ipResolver = m_devsReolvers[rec->deviceName()];
                if (ipResolver) {
                    if (event == LQMeas::DeviceNetworkBrowser::EventRemoved) {
                        beginRemoveRows(QModelIndex(), fnd_idx, fnd_idx);
                        m_devrecs.removeAt(fnd_idx);
                        endRemoveRows();
                        ipResolver->remSvcRec(svcRec);
                    } else {
                        m_devrecs[fnd_idx].rec = svcRec;
						emit dataChanged(createIndex(fnd_idx, 0, static_cast<void*>(nullptr)), createIndex(fnd_idx, COLUMN_CNT-1, static_cast<void*>(nullptr)));
                        ipResolver->updateSvcRec(svcRec);
                    }
                }
            }
        }
    }


}
