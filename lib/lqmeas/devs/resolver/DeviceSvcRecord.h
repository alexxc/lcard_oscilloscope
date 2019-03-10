#ifndef LQMEAS_DEVICESVCRECORD_H
#define LQMEAS_DEVICESVCRECORD_H

#include <QString>
#include <QHostAddress>

namespace LQMeas {
    class DeviceSvcRecord {
    public:
        virtual QString instanceName() const = 0;
        virtual QString deviceName() const = 0;
        virtual QString deviceSerial() const = 0;
        virtual QHostAddress hostAddress() const = 0;
        virtual bool isEqual(DeviceSvcRecord *rec) const = 0;
        virtual ~DeviceSvcRecord() {}
    };
}

#endif // LQMEAS_DEVICESVCRECORD_H

