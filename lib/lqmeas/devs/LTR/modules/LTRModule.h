#ifndef LQMEAS_LTRMODULE_H
#define LQMEAS_LTRMODULE_H


#include <QSharedPointer>

#include "lqmeas/devs/Device.h"
#include "lqmeas/devs/LTR/crates/LTRCrate.h"
#include "ltr/include/ltrapi.h"

namespace LQMeas {
    class LTRResolver;
    class LtrFrameReceiver;

    class LTRModule : public Device {
        Q_OBJECT
    public:

        ~LTRModule();

        unsigned mid() const {return m_mid;}
        int slot() const {return m_slot; }

        DeviceInterface iface() const {return InterfaceLTR;}
        QString location() const {return tr("Slot %1").arg(QString::number(slot()));}

        QSharedPointer<LTRCrate> crate() const {return m_crate;}


        Error error(int err) const;
    protected:
        explicit LTRModule(QSharedPointer<LTRCrate> crate, unsigned slot,
                           unsigned mid, DeviceConfig *defaultCfg, const DeviceInfo *info, QObject *parent = 0);


        virtual QString errorString(int err) const;
        virtual TLTR* channel() const=0;

        virtual Error rawWordsReceive(DWORD *wrds, unsigned size, unsigned tout, unsigned *recvd_size);
        virtual Error rawWordsSend(const DWORD *wrds, unsigned size, unsigned tout, unsigned *sent_size);
    private:
        QSharedPointer<LTRCrate> m_crate;
        unsigned m_slot;
        unsigned m_mid;

        friend class LTRResolver;
        friend class LtrFrameReceiver;
    };
}

#endif // LTRMODULE_H
