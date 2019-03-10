#include "E502TypeInfo.h"
#include "x502api.h"

namespace LQMeas {
    E502TypeInfo::E502TypeInfo(unsigned devflags, bool dynamic_alloc) : x502TypeInfo(devflags),
        m_devflags(devflags), m_dynamic_alloc(dynamic_alloc) {

        m_mod_string = name() + QString("-%1-%2-%3").arg(bfPresent() ? "P" : "X")
                .arg(ethPresent() ? "EU" : "U")
                .arg(dacPresent() ? "D" : "X");
        if (industrial())
            m_mod_string += "-I";
    }

    const LQMeas::E502TypeInfo *E502TypeInfo::defaultInfo() {
        return typeInfoPEUD();
    }

    const E502TypeInfo *E502TypeInfo::typeInfoPEUDI() {
        static const E502TypeInfo info(X502_DEVFLAGS_IFACE_SUPPORT_USB | X502_DEVFLAGS_GAL_PRESENT |
                                        X502_DEVFLAGS_BF_PRESENT | X502_DEVFLAGS_IFACE_SUPPORT_ETH |
                                        X502_DEVFLAGS_DAC_PRESENT | X502_DEVFLAGS_INDUSTRIAL, false);
        return &info;
    }

    const E502TypeInfo *E502TypeInfo::typeInfoPEUD() {
        static const E502TypeInfo info(X502_DEVFLAGS_IFACE_SUPPORT_USB | X502_DEVFLAGS_GAL_PRESENT |
                                       X502_DEVFLAGS_BF_PRESENT | X502_DEVFLAGS_IFACE_SUPPORT_ETH |
                                       X502_DEVFLAGS_DAC_PRESENT, false);
        return &info;
    }

    const E502TypeInfo *E502TypeInfo::typeInfoXUD() {
        static const E502TypeInfo info(X502_DEVFLAGS_IFACE_SUPPORT_USB | X502_DEVFLAGS_GAL_PRESENT |
                                       X502_DEVFLAGS_DAC_PRESENT, false);
        return &info;
    }

    const E502TypeInfo *E502TypeInfo::typeInfoXEUX() {
        static const E502TypeInfo info(X502_DEVFLAGS_IFACE_SUPPORT_USB | X502_DEVFLAGS_GAL_PRESENT |
                                       X502_DEVFLAGS_IFACE_SUPPORT_ETH, false);
        return &info;
    }

    const E502TypeInfo *E502TypeInfo::typeInfoXUX() {
        static const E502TypeInfo info(X502_DEVFLAGS_IFACE_SUPPORT_USB | X502_DEVFLAGS_GAL_PRESENT, false);
        return &info;
    }

    QList<const E502TypeInfo *> E502TypeInfo::types() {
        QList<const E502TypeInfo *> ret;
        ret.append(typeInfoPEUD());
        ret.append(typeInfoPEUDI());
        ret.append(typeInfoXUD());
        ret.append(typeInfoXEUX());
        ret.append(typeInfoXUX());
        return ret;
    }

    const DeviceTypeInfo *E502TypeInfo::clone() const {
        return m_dynamic_alloc ? new E502TypeInfo(m_devflags, true) : DeviceTypeInfo::clone();
    }

}
