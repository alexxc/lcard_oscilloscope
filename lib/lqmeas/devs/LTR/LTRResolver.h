#ifndef LQMEAS_LTRRESOLVER_H
#define LQMEAS_LTRRESOLVER_H

#include <QList>
#include <QStringList>
#include <QSharedPointer>

#include "lqmeas/devs/resolver/DevicesResolver.h"

namespace LQMeas {
    class LTRModule;
    class LTRCrate;
    class Device;
    class DeviceValidator;

    class LTRResolver : public DevicesResolver {
    public:
        QList<QSharedPointer<LTRModule> > getModuleList(QStringList nameFilter);
        QList<QSharedPointer<LTRModule> > getModuleList(const DeviceValidator* validator);

        QList<QSharedPointer<LTRCrate> > getCratesList();

        QList<QSharedPointer<Device> > getDevList();

        static LTRResolver* resolver();

    private:
        LTRResolver();
        QList< QSharedPointer<LTRCrate> > m_crates;

        QSharedPointer<LTRCrate> getCrate(QString serial);
        QSharedPointer<LTRCrate> getCrate(const LTRCrate *crate);

        static QSharedPointer<LTRModule> createModule(QSharedPointer<LTRCrate> getCrate, unsigned slot, quint16 mid);

        friend class LTRCrate;
    };
}

#endif // LTRRESOLVER_H
