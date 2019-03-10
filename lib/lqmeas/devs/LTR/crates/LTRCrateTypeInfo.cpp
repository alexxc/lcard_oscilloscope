#include "LTRCrateTypeInfo.h"

namespace LQMeas {
    const LTRCrateTypeInfo *LTRCrateTypeInfo::typeInfoU8_16() {
        static const LTRCrateTypeInfo info(LTR_CRATE_TYPE_LTR010, "LTR-U-8/16", 16);
        return &info;
    }

    const LTRCrateTypeInfo *LTRCrateTypeInfo::typeInfoU1() {
        static const LTRCrateTypeInfo info(LTR_CRATE_TYPE_LTR021, "LTR-U-1", 1);
        return &info;
    }

    const LTRCrateTypeInfo *LTRCrateTypeInfo::typeInfoEU8_16() {
        static const LTRCrateTypeInfo info(LTR_CRATE_TYPE_LTR030, "LTR-EU-8/16", 16);
        return &info;
    }

    const LTRCrateTypeInfo *LTRCrateTypeInfo::typeInfoEU2() {
        static const LTRCrateTypeInfo info(LTR_CRATE_TYPE_LTR031, "LTR-EU-2", 2);
        return &info;
    }

    const LTRCrateTypeInfo *LTRCrateTypeInfo::typeInfoE7_15() {
        static const LTRCrateTypeInfo info(LTR_CRATE_TYPE_LTR032, "LTR-E-7/15", 16);
        return &info;
    }

    const LTRCrateTypeInfo *LTRCrateTypeInfo::typeInfoCU1() {
        static const LTRCrateTypeInfo info(LTR_CRATE_TYPE_LTR_CU_1, "LTR-CU-1", 1);
        return &info;
    }

    const LTRCrateTypeInfo *LTRCrateTypeInfo::typeInfoCEU1() {
        static const LTRCrateTypeInfo info(LTR_CRATE_TYPE_LTR_CEU_1, "LTR-CEU-1", 1);
        return &info;
    }

    const LTRCrateTypeInfo *LTRCrateTypeInfo::typeInfo(en_LTR_CrateTypes crate_type) {
        const LTRCrateTypeInfo *ret = 0;
        QList<const LTRCrateTypeInfo *> crateTypes = LTRCrateTypeInfo::types();
        Q_FOREACH(const LTRCrateTypeInfo *type, crateTypes) {
            if (type->crateType() == crate_type) {
                ret = type;
                break;
            }
        }

        if (!ret) {
            ret = LTRCrateTypeInfo::defaultInfo();
        }
        return ret;
    }

    QList<const LTRCrateTypeInfo*> LTRCrateTypeInfo::types() {
        QList <const LTRCrateTypeInfo*> ret;
        ret.append(typeInfoEU8_16());
        ret.append(typeInfoEU2());
        ret.append(typeInfoCEU1());
        ret.append(typeInfoCU1());
        ret.append(typeInfoU8_16());
        ret.append(typeInfoU1());
        ret.append(typeInfoE7_15());
        return ret;
    }

    const LTRCrateTypeInfo *LTRCrateTypeInfo::defaultInfo() {
        static const LTRCrateTypeInfo info(LTR_CRATE_TYPE_UNKNOWN, "Unknown", 16);
        return &info;
    }

    LTRCrateTypeInfo::LTRCrateTypeInfo(en_LTR_CrateTypes type, QString crateName, unsigned slot_cnt) :
        m_type(type), m_crateTypeName(crateName), m_slots_cnt(slot_cnt) {

    }
}
