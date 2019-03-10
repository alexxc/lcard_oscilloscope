#ifndef LQMEAS_LTRCRATETYPEINFO_H
#define LQMEAS_LTRCRATETYPEINFO_H


#include "lqmeas/devs/DeviceTypeInfo.h"
#include "ltr/include/ltrapi.h"
#include <QList>

namespace LQMeas {
    class LTRCrateTypeInfo : public DeviceTypeInfo {
    public:
        static const LTRCrateTypeInfo *defaultInfo();

        static const LTRCrateTypeInfo *typeInfoU8_16();
        static const LTRCrateTypeInfo *typeInfoU1();
        static const LTRCrateTypeInfo *typeInfoEU8_16();
        static const LTRCrateTypeInfo *typeInfoEU2();
        static const LTRCrateTypeInfo *typeInfoE7_15();
        static const LTRCrateTypeInfo *typeInfoCU1();
        static const LTRCrateTypeInfo *typeInfoCEU1();

        static const LTRCrateTypeInfo *typeInfo(en_LTR_CrateTypes crate_type);

        static QList<const LTRCrateTypeInfo *> types();


        QString deviceTypeName() const {return m_crateTypeName;}
        unsigned slotCount() const {return m_slots_cnt;}
        unsigned maxChildDevicesCount() const {return slotCount();}
        en_LTR_CrateTypes crateType() const {return m_type;}



    private:
        LTRCrateTypeInfo(en_LTR_CrateTypes type, QString crateName, unsigned slot_cnt);

        QString m_crateTypeName;
        unsigned m_slots_cnt;
        en_LTR_CrateTypes m_type;
    };
}

#endif // LQMEAS_LTRCRATETYPEINFO_H
