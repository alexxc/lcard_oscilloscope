#ifndef LQMEAS_DEVICESRESOLVER_H
#define LQMEAS_DEVICESRESOLVER_H

#include <QObject>
#include <QSharedPointer>
#include "lqmeas/devs/Device.h"

namespace LQMeas {
    class DeviceValidator;

    class DevicesResolver {
    public:
        virtual QList<QSharedPointer<Device> > getDevList() = 0;
        virtual QList<QSharedPointer<Device> > getDevList(const DeviceValidator *validator);
        virtual QList<QSharedPointer<Device> > getDevList(QStringList deviceNames);
    };
}

#endif // LQMEAS_DEVICESRESOLVER_H
