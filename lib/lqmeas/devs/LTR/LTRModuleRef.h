#ifndef LTRMODULEREF_H
#define LTRMODULEREF_H


#include "lqmeas/devs/DeviceRef.h"

namespace LQMeas {
    class LTRModuleRef : public DeviceRef {
    public:
        LTRModuleRef(const LQMeas::DeviceInfo *info, const DeviceRef *crate, unsigned slot_num);

        DeviceRef *clone() const {return new LTRModuleRef(*this);}
        bool sameDevice(const DeviceRef *ref) const;

        int parentPos() const {return moduleSlot() - 1;}
        unsigned moduleSlot() const {return m_slot;}
    protected:
        LTRModuleRef(const LTRModuleRef& ref);
    private:
        unsigned m_slot;
    };
}

#endif // LTRMODULEREF_H
