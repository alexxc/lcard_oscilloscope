#ifndef LQMEAS_LTRCRATE_H
#define LQMEAS_LTRCRATE_H


#include <QStringList>
#include <QSharedPointer>
#include <QVector>
#include "ltr/include/ltrapi.h"
#include "lqmeas/devs/Device.h"
#include "lqmeas/devs/resolver/DevicesResolver.h"


namespace LQMeas {
    class LTRModule;
    class LTRResolver;
    class DeviceValidator;
    class LTRCrateInfo;


    class LTRCrate : public Device, public DevicesResolver {
        Q_OBJECT
    public:
        ~LTRCrate();

        unsigned slotCount() const;
        virtual bool isOpened() const;
        /* адрес ltrd с которым установлено соединение */
        quint32 ltrdIpAddr();
        /* порт TCP соединения с ltrd */
        quint16 ltrdTcpPort();


        DeviceInterface iface() const {return m_iface;}

        QList<QSharedPointer<LTRModule> > getModuleList(QStringList nameFilter);
        QList<QSharedPointer<LTRModule> > getModuleList(const DeviceValidator* validator);
        QList<QSharedPointer<Device> > getDevList();

        DevicesResolver *childResolver() {return dynamic_cast<DevicesResolver *>(this);}

        const LTRCrateInfo *devspecInfo() const;

        Error error(int err) const;
    protected:
        Error protOpen();
        Error protClose();
        Error protConfigure(const DeviceConfig *cfg);

    private:
        explicit LTRCrate(TLTR hnd, QString serial, TLTR_CRATE_INFO info);

        DeviceInterface m_iface;
        QVector<QSharedPointer<LTRModule> > m_modules;

        mutable TLTR m_hnd;

        friend class LTRResolver;
    };
}

#endif // LQMEAS_LTRCRATE_H
