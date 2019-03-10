#include "DeviceRef.h"
#include "lqmeas/devs/DeviceInfo.h"
#include "lqmeas/devs/DeviceTypeInfo.h"
#include "lqmeas/devs/Device.h"

namespace LQMeas {

    DeviceRef::DeviceRef(const DeviceInfo *info, const DeviceRef *parent) : m_info(info), m_parent(parent) {

    }

    DeviceRef::DeviceRef(const DeviceRef &ref) {
        m_info = ref.m_info->clone();
        m_parent = ref.m_parent ? ref.m_parent->clone() : 0;
    }

    DeviceRef::~DeviceRef() {
        delete m_info;
        delete m_parent;
    }

    bool DeviceRef::checkDevice(const Device *device) const  {
        return (device->devTypeInfo()->deviceTypeName() == m_info->type()->deviceTypeName()) &&
                (m_info->serial().isEmpty() || (device->serial() == m_info->serial()));
    }

    bool DeviceRef::sameDevice(const DeviceRef *ref) const {
        bool ok = m_info->sameDevice(ref->devInfo());
        if (ok && ((m_parent == 0) != (ref->m_parent == 0)))
            ok = false;
        if (ok && m_parent)
            ok = m_parent->sameDevice(ref->m_parent);
        return ok;
    }
}




