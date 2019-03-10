#ifndef LQMEAS_LTRCRATEINFO_H
#define LQMEAS_LTRCRATEINFO_H

#include "lqmeas/devs/DeviceInfo.h"
#include "LTRCrateTypeInfo.h"

namespace LQMeas {
    class LTRCrateInfo : public DeviceInfo {
    public:
        LTRCrateInfo(const LTRCrateTypeInfo *type, QString serial = QString()) :
                     DeviceInfo(type, serial) {}

        const DeviceInfo *clone() const {return new LTRCrateInfo(*this);}

        unsigned slotCount() const {return devspecTypeInfo()->slotCount();}
        const LTRCrateTypeInfo *devspecTypeInfo() const {return static_cast<const LTRCrateTypeInfo *>(type());}
    protected:
        LTRCrateInfo(const LTRCrateInfo &info) : DeviceInfo(info) {}
    };
}

#endif // LTRCRATEINFO_H
