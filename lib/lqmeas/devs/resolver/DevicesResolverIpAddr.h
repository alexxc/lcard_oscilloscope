#ifndef LQMEAS_DEVICESRESOLVERIPADDR_H
#define LQMEAS_DEVICESRESOLVERIPADDR_H

#include <QStringList>
#include <QHostAddress>

#include "DeviceNetworkBrowser.h"
#include "lqmeas/StdErrors.h"
#include "lqmeas_config.h"

namespace LQMeas {
    class DevicesResolverIpAddr {
    public:
        virtual Error addIpAddr(QHostAddress addr) = 0;
        virtual void remIpAddr(QHostAddress addr) = 0;

        virtual QStringList ipSupportedDevs() = 0;

#ifdef LQMEAS_USE_NETWORK_BROWSER
        virtual DeviceNetworkBrowser *netBrowser() {return 0;}
#endif
        virtual Error addSvcRec(  QSharedPointer<DeviceSvcRecord> svc) = 0;
        virtual void updateSvcRec(  QSharedPointer<DeviceSvcRecord> svc) = 0;
        virtual void remSvcRec(  QSharedPointer<DeviceSvcRecord> svc) = 0;

    };
}

#endif // LQMEAS_DEVICESRESOLVERIPADDR_H
