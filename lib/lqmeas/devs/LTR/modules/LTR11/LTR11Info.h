#ifndef LQMEAS_LTR11INFO_H
#define LQMEAS_LTR11INFO_H

#include "lqmeas/devs/DeviceInfo.h"
#include "LTR11TypeInfo.h"

namespace LQMeas {
    class LTR11Info : public DeviceInfo {
    public:
        LTR11Info(const LTR11TypeInfo *type = LTR11TypeInfo::defaultInfo(), QString serial = QString(), int mcu_ver = 0) :
            DeviceInfo(type, serial), m_mcu_ver(mcu_ver) {}

        const DeviceInfo *clone() const {return new LTR11Info(*this);}
        int mcuVer() const {return m_mcu_ver;}


        const LTR11TypeInfo *devspecTypeInfo() const {return static_cast<const LTR11TypeInfo *>(type());}
    private:
        LTR11Info(const LTR11Info &info) : DeviceInfo(info), m_mcu_ver(info.m_mcu_ver) {
        }

        int m_mcu_ver;
    };
}

#endif // LTR11INFO_H
