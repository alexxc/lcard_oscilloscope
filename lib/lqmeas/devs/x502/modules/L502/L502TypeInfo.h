#ifndef LQMEAS_L502TYPEINFO_H
#define LQMEAS_L502TYPEINFO_H

#include "lqmeas/devs/x502/x502TypeInfo.h"

namespace LQMeas {
    class L502TypeInfo : public x502TypeInfo {
    public:
        static QString name() {return "L502";}

        static const L502TypeInfo *defaultInfo();

        static const L502TypeInfo *typeInfoPGD();
        static const L502TypeInfo *typeInfoPG();
        static const L502TypeInfo *typeInfoXGD();
        static const L502TypeInfo *typeInfoXG();
        static const L502TypeInfo *typeInfoXXD();
        static const L502TypeInfo *typeInfoXX();

        static QList<const L502TypeInfo *> types();

        QString deviceTypeName() const {return name();}
        QString devceModificationName() const {return m_mod_string;}

        bool isDynamicAllocated() const {return m_dynamic_alloc;}
        const DeviceTypeInfo *clone() const;
    private:
        L502TypeInfo(unsigned devflags, bool dynamic_alloc);

        bool m_dynamic_alloc;
        unsigned m_devflags;

        QString m_mod_string;
        friend class L502;
    };
}

#endif // LQMEAS_L502TYPEINFO_H
