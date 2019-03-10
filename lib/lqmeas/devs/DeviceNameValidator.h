#ifndef LQMEAS_DEVICENAMEVALIDATOR_H
#define LQMEAS_DEVICENAMEVALIDATOR_H

#include "DeviceValidator.h"
#include "Device.h"
#include <QStringList>


namespace LQMeas {
    class DeviceNameValidator : public DeviceValidator {
    public:
        DeviceNameValidator(QStringList nameList) : m_devnames(nameList) {}

        bool deviceValid(const Device *device) const {
            return device && (m_devnames.contains(device->name()) || m_devnames.empty());
        }

    private:
        QStringList m_devnames;
    };
}

#endif // LQMEAS_DEVICENAMEVALIDATOR_H
