#ifndef LQMEAS_X502RESOLVER_H
#define LQMEAS_X502RESOLVER_H

#include <QObject>
#include <QSharedPointer>
#include <QHash>
#include "x502.h"

#include "lqmeas/devs/resolver/DevicesResolver.h"
#include "lqmeas/devs/resolver/DevicesResolverIpAddr.h"


namespace LQMeas {
    class x502Resolver : public DevicesResolver, public DevicesResolverIpAddr {
    public:
        Error addIpAddr(QHostAddress addr);
        void remIpAddr(QHostAddress addr);

        QStringList ipSupportedDevs();

#ifdef LQMEAS_USE_NETWORK_BROWSER
        DeviceNetworkBrowser *netBrowser();
#endif
        Error addSvcRec(  QSharedPointer<DeviceSvcRecord> svc);
        void remSvcRec(  QSharedPointer<DeviceSvcRecord> svc);
        void updateSvcRec(  QSharedPointer<DeviceSvcRecord> svc);

        QList<QSharedPointer<Device> > getDevList();

        static x502Resolver* resolver();
    private:
        static const int m_con_tout = 5000;

        explicit x502Resolver();

        QList< QSharedPointer<Device> > m_devs;

        QHash<quint32, QSharedPointer<Device> > m_tcpDevs;

        QList<QSharedPointer<Device> > m_svcDevs;
        QList<QSharedPointer<DeviceSvcRecord> > m_svcList;

    };
}

#endif // X502RESOLVER_H
