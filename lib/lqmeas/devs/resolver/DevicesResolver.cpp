#include "DevicesResolver.h"
#include "lqmeas/devs/DeviceValidator.h"
#include "lqmeas/devs/DeviceNameValidator.h"

namespace LQMeas {
    QList<QSharedPointer<Device> > DevicesResolver::getDevList(const DeviceValidator *validator) {
        QList<QSharedPointer<Device> > devlist, retlist;
        devlist = getDevList();
        Q_FOREACH (QSharedPointer<Device> dev, devlist) {
            if (dev && validator->deviceValid(dev.data()))
                retlist.append(dev);
        }
        return retlist;
    }

    QList<QSharedPointer<Device> > DevicesResolver::getDevList(QStringList deviceNames) {
        DeviceNameValidator val(deviceNames);
        return getDevList(&val);
    }
}
