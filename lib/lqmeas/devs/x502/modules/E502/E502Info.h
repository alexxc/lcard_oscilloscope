#ifndef LQMEAS_E502INFO_H
#define LQMEAS_E502INFO_H


#include "lqmeas/devs/x502/x502Info.h"
#include "E502TypeInfo.h"

namespace LQMeas {
    class E502Info : public x502Info {
    public:
        E502Info(const E502TypeInfo *type = E502TypeInfo::defaultInfo(), QString serial = "",
                 unsigned fpga_ver = 0, unsigned plda_ver = 0, unsigned mcu_ver = 0) :
                x502Info(type, serial, fpga_ver, plda_ver, mcu_ver) {}

        E502Info(const E502Info &info) : x502Info(info) {}
        const DeviceInfo *clone() const {return new E502Info(*this);}
        bool mcuPresent() const {return true;}
    };
}


#endif // LQMEAS_E502INFO_H
