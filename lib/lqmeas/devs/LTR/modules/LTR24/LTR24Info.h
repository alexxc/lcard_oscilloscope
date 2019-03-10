#ifndef LQMEAS_LTR24INFO_H
#define LQMEAS_LTR24INFO_H

#include "lqmeas/devs/DeviceInfo.h"
#include "LTR24TypeInfo.h"

namespace LQMeas {
    class LTR24Info : public DeviceInfo {
    public:
        LTR24Info(const LTR24TypeInfo *type = LTR24TypeInfo::defaultInfo(), QString serial = "", int pld_ver = 0) :
            DeviceInfo(type, serial), m_pld_ver(pld_ver) {}


        const DeviceInfo *clone() const {return new LTR24Info(*this);}
        int pldVer() const {return m_pld_ver;}
        bool supportICP() const {return devspecTypeInfo()->supportICP();}

        const LTR24TypeInfo *devspecTypeInfo() const {return static_cast<const LTR24TypeInfo *>(type());}
    private:
        LTR24Info(const LTR24Info &info) : DeviceInfo(info), m_pld_ver(info.m_pld_ver) {
        }

        int m_pld_ver;
    };
}


#endif // LTR24INFO_H
