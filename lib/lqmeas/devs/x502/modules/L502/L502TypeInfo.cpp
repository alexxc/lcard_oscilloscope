#include "L502TypeInfo.h"
#include "x502api.h"


namespace LQMeas {
    L502TypeInfo::L502TypeInfo(unsigned devflags, bool dynamic_alloc) : x502TypeInfo(devflags),
        m_devflags(devflags), m_dynamic_alloc(dynamic_alloc) {
        m_mod_string = name() + QString("-%1-%2").arg(bfPresent() ? "P" : "X")
                .arg(galPresent() ? "G" : "X");
        if (dacPresent())
            m_mod_string += "-D";
    }

    const L502TypeInfo *L502TypeInfo::defaultInfo() {
        return typeInfoPGD();
    }

    const L502TypeInfo *L502TypeInfo::typeInfoPGD() {
        static const L502TypeInfo info(X502_DEVFLAGS_IFACE_SUPPORT_PCI |
                                        X502_DEVFLAGS_BF_PRESENT | X502_DEVFLAGS_GAL_PRESENT |
                                        X502_DEVFLAGS_DAC_PRESENT, false);
        return &info;
    }

    const L502TypeInfo *L502TypeInfo::typeInfoPG() {
        static const L502TypeInfo info(X502_DEVFLAGS_IFACE_SUPPORT_PCI |
                                        X502_DEVFLAGS_BF_PRESENT | X502_DEVFLAGS_GAL_PRESENT, false);
        return &info;
    }

    const L502TypeInfo *L502TypeInfo::typeInfoXGD() {
        static const L502TypeInfo info(X502_DEVFLAGS_IFACE_SUPPORT_PCI |
                                       X502_DEVFLAGS_GAL_PRESENT |
                                       X502_DEVFLAGS_DAC_PRESENT, false);
        return &info;
    }

    const L502TypeInfo *L502TypeInfo::typeInfoXG() {
        static const L502TypeInfo info(X502_DEVFLAGS_IFACE_SUPPORT_PCI |
                                       X502_DEVFLAGS_GAL_PRESENT, false);
        return &info;
    }

    const L502TypeInfo *L502TypeInfo::typeInfoXXD() {
        static const L502TypeInfo info(X502_DEVFLAGS_IFACE_SUPPORT_PCI |
                                       X502_DEVFLAGS_DAC_PRESENT, false);
        return &info;
    }

    const L502TypeInfo *L502TypeInfo::typeInfoXX() {
        static const L502TypeInfo info(X502_DEVFLAGS_IFACE_SUPPORT_PCI, false);
        return &info;
    }

    QList<const L502TypeInfo *> L502TypeInfo::types() {
        QList<const L502TypeInfo *> ret;
        ret.append(typeInfoPGD());
        ret.append(typeInfoPG());
        ret.append(typeInfoXGD());
        ret.append(typeInfoXG());
        ret.append(typeInfoXXD());
        ret.append(typeInfoXX());
        return ret;
    }

    const DeviceTypeInfo *L502TypeInfo::clone() const {
        return m_dynamic_alloc ? new L502TypeInfo(m_devflags, true) : DeviceTypeInfo::clone();
    }

}
