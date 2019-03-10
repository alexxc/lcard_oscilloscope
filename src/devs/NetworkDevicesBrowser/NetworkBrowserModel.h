#ifndef NETWORKDEVICESBROWSER_H
#define NETWORKDEVICESBROWSER_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QSettings>
#include "lqmeas/devs/resolver/DeviceNetworkBrowser.h"

namespace LQMeas {
    class DevicesResolverIpAddr;
}

namespace LQMeasStudio {
    /* Модель в виде таблицы со списком присутствующих устройств в локальной
       сети. Находит все нужные плагины и, если разрешена, запускает их
       поиск */
    class NetworkBrowserModel : public QAbstractTableModel {
        Q_OBJECT

    public:
        explicit NetworkBrowserModel(QObject *parent = 0);
        ~NetworkBrowserModel();

        void browseStart();
        void browseStop();
        void browseClearResult();

        bool browseIsEnabled() const {return m_enabled;}
        bool browseIsRunning() const {return m_running;}

        void browseEnable(bool en);

        /* Признак, найдены ли все разрешенные устройства в локальной сети */
        bool allEnabledDevsPresent() const;
        /* Удаление всех не найденных разрешенных устройств в локальной сети */
        void removeNotPresentDevs();

        void loadSettings(QSettings &set);
        void saveSettings(QSettings &set) const;


        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QVariant headerData (int section, Qt::Orientation orientation,
                             int role = Qt::DisplayRole ) const;
        bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    signals:
         void error(LQMeas::Error err);
    private slots:
        void onStateChanged(const QSharedPointer<LQMeas::DeviceSvcRecord>& svcRec,
                            LQMeas::DeviceNetworkBrowser::Event event);

    private:
        static const int COLUMN_INSTANCE  = 0;
        static const int COLUMN_DEVNAME   = 1;
        static const int COLUMN_DEVSERIAL = 2;
        static const int COLUMN_IPADDR    = 3;
        static const int COLUMN_CNT       = 4;

        struct svcRecState {
            QSharedPointer <LQMeas::DeviceSvcRecord> rec;
            bool en;
        } ;
        QStringList notPresentDevs() const;

        QList<svcRecState> m_devrecs;
        QList<LQMeas::DeviceNetworkBrowser *> m_browsers;
        QStringList m_enabledDevices;
        bool m_enabled;
        bool m_running;

        QHash<QString, LQMeas::DevicesResolverIpAddr*> m_devsReolvers;        
    };
}

#endif // NETWORKDEVICESBROWSER_H
