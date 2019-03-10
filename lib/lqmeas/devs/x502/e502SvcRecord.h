#ifndef E502SVCRECORD_H
#define E502SVCRECORD_H


#include "lqmeas/devs/resolver/DeviceSvcRecord.h"
#include "e502api.h"

namespace LQMeas {

    class e502SvcRecord : public DeviceSvcRecord {
    public:
        QString instanceName() const;
        QString deviceName() const;
        QString deviceSerial() const;
        QHostAddress hostAddress() const;
        bool isEqual(DeviceSvcRecord *rec) const;
        ~e502SvcRecord();
    private:
        e502SvcRecord(t_e502_eth_svc_record_hnd hnd);

        t_e502_eth_svc_record_hnd m_hnd;
        mutable QHostAddress m_addr;

        friend class e502NetBrowser;
        friend class x502Resolver;
    };
}

#endif // E502SVCRECORD_H
