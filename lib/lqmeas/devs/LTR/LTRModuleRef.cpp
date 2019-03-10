#include "LTRModuleRef.h"

namespace LQMeas {
    LTRModuleRef::LTRModuleRef(const LQMeas::DeviceInfo *info, const DeviceRef *crate, unsigned slot_num) :
        DeviceRef(info, crate), m_slot(slot_num) {
    }

    bool LTRModuleRef::sameDevice(const DeviceRef *ref) const {
        const LTRModuleRef *ltrRef = dynamic_cast<const LTRModuleRef *>(ref);
        return ltrRef && DeviceRef::sameDevice(ref) && (m_slot == ltrRef->m_slot);
    }

    LTRModuleRef::LTRModuleRef(const LTRModuleRef &ref) : DeviceRef(ref), m_slot(ref.m_slot) {

    }
}
