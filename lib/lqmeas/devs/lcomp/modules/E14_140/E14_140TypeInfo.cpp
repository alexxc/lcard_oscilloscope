#include "E14_140TypeInfo.h"

namespace LQMeas {
    static const double f_ranges[] = {10, 2.5, 0.625,  0.15625};

    const E14_140TypeInfo *E14_140TypeInfo::defaultInfo() {
        return typeInfoMD();
    }

    const E14_140TypeInfo *E14_140TypeInfo::typeInfoBase() {
        static const E14_140TypeInfo info(false, false);
        return &info;
    }

    const E14_140TypeInfo *E14_140TypeInfo::typeInfoD() {
        static const E14_140TypeInfo info(false, true);
        return &info;
    }

    const E14_140TypeInfo *E14_140TypeInfo::typeInfoM() {
        static const E14_140TypeInfo info(true, false);
        return &info;
    }

    const E14_140TypeInfo *E14_140TypeInfo::typeInfoMD() {
        static const E14_140TypeInfo info(true, true);
        return &info;
    }

    unsigned E14_140TypeInfo::adcRangesCnt(int unit_mode_num) const {
        return sizeof(f_ranges)/sizeof(f_ranges[0]);
    }

    double E14_140TypeInfo::adcRangeMaxVal(int range, int unit_mode_num) const {
        return f_ranges[range];
    }

    double E14_140TypeInfo::adcRangeMinVal(int range, int unit_mode_num) const {
        return -f_ranges[range];
    }

    E14_140TypeInfo::E14_140TypeInfo(bool modifiecated, bool dacPresent) :
        m_modificated(modifiecated), m_dacPresent(dacPresent) {

        m_modName = name();
        if (m_modificated) {
            m_modName += "-M";
            if (dacPresent)
                m_modName += "-D";
        } else {
            if (dacPresent)
                m_modName += "D";
        }
    }

    const E14_140TypeInfo *E14_140TypeInfo::typeInfo(bool modificated, bool dacPresent) {
        const E14_140TypeInfo *ret = 0;
        Q_FOREACH(const E14_140TypeInfo *info, types()) {
            if ((info->modificated() == modificated) && (info->dacPresent() == dacPresent)) {
                ret = info;
                break;
            }
        }
        return ret;
    }

    QList<const E14_140TypeInfo *> E14_140TypeInfo::types()  {
        QList<const E14_140TypeInfo *> ret;
        ret.append(typeInfoMD());
        ret.append(typeInfoM());
        ret.append(typeInfoD());
        ret.append(typeInfoBase());
        return ret;
    }

}
