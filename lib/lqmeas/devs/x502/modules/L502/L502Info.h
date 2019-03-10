#ifndef LQMEAS_L502INFO_H
#define LQMEAS_L502INFO_H

#include "lqmeas/devs/x502/x502Info.h"
#include "L502TypeInfo.h"

namespace LQMeas {
    class L502Info : public x502Info {
    public:
        L502Info(const L502TypeInfo *type = L502TypeInfo::defaultInfo(), QString serial = "",
                 unsigned fpga_ver = 0, unsigned plda_ver = 0) :
                x502Info(type, serial, fpga_ver, plda_ver, 0) {}

        L502Info(const L502Info &info) : x502Info(info) {}
        const DeviceInfo *clone() const {return new L502Info(*this);}

        bool mcuPresent() const {return false;}
    };
}

#endif // LQMEAS_L502INFO_H
