#ifndef IPADDRCONFIG_H
#define IPADDRCONFIG_H

#include <QAbstractTableModel>
#include <QSettings>
#include <QHostAddress>
#include <QList>
#include "LQMeasStudio.h"

namespace LQMeas {
    class DevicesResolverIpAddr;
}

namespace LQMeasStudio {
    class IpAddrConfig : public QAbstractTableModel {
        Q_OBJECT
    public:
        struct Record {
            Record(QString devname, QHostAddress ip_addr, bool enabled) :
                m_enabled(enabled), m_devname(devname), m_ip_addr(ip_addr) {}

            bool enabled() const {return m_enabled;}
            QString devname() const {return m_devname;}
            QHostAddress ipAddr() const {return m_ip_addr;}

            void setEnabled(bool enabled) {m_enabled = enabled;}
        private:
            bool m_enabled;
            QString m_devname;
            QHostAddress m_ip_addr;
        } ;



        explicit IpAddrConfig(QObject *parent = 0);
        ~IpAddrConfig();
        QStringList supportedDevs() const;


        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QVariant headerData (int section, Qt::Orientation orientation,
                             int role = Qt::DisplayRole ) const;
        bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    public slots:
        void addRecord(const QString& devname, const QHostAddress& ip_addr, bool enabled = true);
		void remRecord(signed idx);
		void enableRecord(signed idx, bool enable);



        void loadSettings(QSettings &set);
        void saveSettings(QSettings &set) const;
    private:
        static const int COLUMN_DEVNAME = 0;
        static const int COLUMN_IP_ADDR = 1;
        static const int COLUMN_CNT = 2;



		Record *record(const QString &devname, const QHostAddress &ip_addr);

        QHash<QString, LQMeas::DevicesResolverIpAddr*> m_devs;
        QList<Record*> m_records;
    };
}

#endif // IPADDRCONFIG_H
