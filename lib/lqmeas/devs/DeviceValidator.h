#ifndef LQMEAS_DEVICEVALIDATOR_H
#define LQMEAS_DEVICEVALIDATOR_H

namespace LQMeas {
    class Device;

    class DeviceValidator  {
    public:
        virtual bool deviceValid(const Device *device) const = 0;
    };    
}

#endif // LQMEAS_DEVICEVALIDATOR_H
