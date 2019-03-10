#include "e502SvcRecord.h"
#include "modules/E502/E502.h"
#include "modules/E502/E502TypeInfo.h"


namespace LQMeas {

    QString e502SvcRecord::instanceName() const {
        char name[X502_INSTANCE_NAME_SIZE];
        QString ret;
        if (E502_EthSvcRecordGetInstanceName(m_hnd, name) == X502_ERR_OK) {
            ret = QString::fromUtf8(name);
        }
        return ret;
    }

    QString e502SvcRecord::deviceName() const {
        return E502TypeInfo::name();
    }

    QString e502SvcRecord::deviceSerial() const {
        char serial[X502_SERIAL_SIZE];
        QString ret;
        if (E502_EthSvcRecordGetDevSerial(m_hnd, serial) == X502_ERR_OK) {
            ret = QString::fromLatin1(serial);
        }
        return ret;

    }

    QHostAddress e502SvcRecord::hostAddress() const {
        if (m_addr.isNull()) {
            uint32_t addr;
            if (E502_EthSvcRecordResolveIPv4Addr(m_hnd, &addr, 5000) == X502_ERR_OK) {
                m_addr = QHostAddress(addr);
            }
        }
        return m_addr;
    }

    bool e502SvcRecord::isEqual(DeviceSvcRecord *rec) const {
        e502SvcRecord *e502_rec = dynamic_cast<e502SvcRecord *>(rec);
        return (e502_rec == NULL) ? false : E502_EthSvcRecordIsSameInstance(m_hnd, e502_rec->m_hnd) == X502_ERR_OK ? true : false;
    }

    e502SvcRecord::e502SvcRecord(t_e502_eth_svc_record_hnd hnd) {
        m_hnd = hnd;
    }

    e502SvcRecord::~e502SvcRecord() {
        E502_EthSvcRecordFree(m_hnd);
    }


}

