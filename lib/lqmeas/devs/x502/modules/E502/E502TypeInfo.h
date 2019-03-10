#ifndef LQMEAS_E502TYPEINFO_H
#define LQMEAS_E502TYPEINFO_H

#include "lqmeas/devs/x502/x502TypeInfo.h"

namespace LQMeas {
    class E502TypeInfo : public x502TypeInfo {
    public:
        static QString name() {return "E502";}

        static const E502TypeInfo *defaultInfo();

        static const E502TypeInfo *typeInfoPEUDI();
        static const E502TypeInfo *typeInfoPEUD();
        static const E502TypeInfo *typeInfoXUD();
        static const E502TypeInfo *typeInfoXEUX();
        static const E502TypeInfo *typeInfoXUX();

        static QList<const E502TypeInfo *> types();

        QString deviceTypeName() const {return name();}
        QString devceModificationName() const {return m_mod_string;}

        bool isDynamicAllocated() const {return m_dynamic_alloc;}
        const DeviceTypeInfo *clone() const;
    private:
        E502TypeInfo(unsigned devflags, bool dynamic_alloc);

        QString m_mod_string;

        bool m_dynamic_alloc;
        unsigned m_devflags;
        friend class E502;
    };
}


#endif // LQMEAS_E502TYPEINFO_H
