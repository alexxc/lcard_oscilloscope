#ifndef LQMEAS_E14_440INFO_H
#define LQMEAS_E14_440INFO_H

#include "lqmeas/devs/DeviceInfo.h"
#include "E14_140TypeInfo.h"

namespace LQMeas {
    class E14_140Info : public DeviceInfo {
    public:
        E14_140Info(const E14_140TypeInfo *type = E14_140TypeInfo::defaultInfo(), QString serial = QString()) :
            DeviceInfo(type, serial) {}

        bool modificated() const {return devspecTypeInfo()->modificated();}
        bool dacPresent() const {return devspecTypeInfo()->dacPresent();}

        const E14_140TypeInfo *devspecTypeInfo() const {return static_cast<const E14_140TypeInfo *>(type());}
    };
}

#endif // LQMEAS_E14_440INFO_H
