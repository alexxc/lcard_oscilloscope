#include "x502Resolver.h"
#include "e502SvcRecord.h"
#ifdef LQMEAS_USE_NETWORK_BROWSER
#include "e502NetBrowser.h"
#endif
#include "../../lqtdefs.h"
#include "lqmeas_config.h"
#include <QScopedArrayPointer>
#include "x502.h"

#ifdef LQMEAS_DEV_E502
    #include "e502api.h"
    #include "modules/E502/E502.h"
    #include "modules/E502/E502TypeInfo.h"
#endif
#ifdef LQMEAS_DEV_L502
    #include "l502api.h"
    #include "modules/L502/L502.h"
    #include "modules/L502/L502TypeInfo.h"
#endif

namespace LQMeas {
    x502Resolver::x502Resolver() {
    }


    Error x502Resolver::addIpAddr(QHostAddress addr) {
        Error err = Error::Success();
        if (!m_tcpDevs.keys().contains(addr.toIPv4Address())) {
            t_x502_devrec devrec;
            err = x502::error(E502_MakeDevRecordByIpAddr(&devrec, addr.toIPv4Address(), 0, m_con_tout));
            if (err.isSuccess())
                m_tcpDevs[addr.toIPv4Address()] = QSharedPointer<Device>(new E502(&devrec));
        }
        return err;
    }

    void x502Resolver::remIpAddr(QHostAddress addr) {
        m_tcpDevs.remove(addr.toIPv4Address());
    }

    QStringList x502Resolver::ipSupportedDevs() {
#ifdef LQMEAS_DEV_E502
        return QStringList() << E502TypeInfo::name();
#else
        return QStringList();
#endif
    }

    QList<QSharedPointer<Device> > x502Resolver::getDevList() {
		int32_t devcnt_usb=0, devcnt_pci=0;
		int32_t devcnt;
        int32_t res = X502_ERR_OK;

    #ifdef LQMEAS_DEV_E502
        if (res >= 0)
			res = E502_UsbGetDevRecordsList(nullptr, 0, 0, reinterpret_cast<unsigned int*>(&devcnt_usb));
    #endif
    #ifdef LQMEAS_DEV_L502
        if (res >= 0)
			res = L502_GetDevRecordsList(nullptr, 0, 0, reinterpret_cast<unsigned int*>(&devcnt_pci));
    #endif
        devcnt = devcnt_usb + devcnt_pci;

        if (res >= 0) {
            QScopedArrayPointer<t_x502_devrec> info_list(new t_x502_devrec[devcnt]);
            /* получаем информацию по всем устройствам  */
    #ifdef LQMEAS_DEV_E502
			res = E502_UsbGetDevRecordsList(info_list.data(), static_cast<unsigned int>(devcnt_usb), 0, nullptr);
    #endif
    #ifdef LQMEAS_DEV_L502
            if (res >= 0) {
				int32_t cur_res = L502_GetDevRecordsList(&info_list.data()[res], static_cast<unsigned int>(devcnt_pci), 0, nullptr);
                if (cur_res >= 0) {
                    res += cur_res;
                }
            }
    #endif
            if (res >= 0) {
                /* удаляем все не найденные устройства */
                for (int d=0; d < m_devs.size(); d++) {
                    bool fnd = false;
					for (signed i=0; !fnd && (i < devcnt); i++) {
                        QString cur_ser = QSTRING_FROM_CSTR(info_list[i].serial);
                        QString cur_name = QSTRING_FROM_CSTR(info_list[i].devname);
                        if ((m_devs.at(d)->serial() == cur_ser) &&
                                (m_devs.at(d)->name() == cur_name)) {
                            fnd = true;

                            QSharedPointer<x502> x502dev = qSharedPointerObjectCast<x502>(m_devs[d]);
                            x502dev->setNewDevrecord(&info_list[i]);
                            memset(info_list[i].devname, 0, X502_DEVNAME_SIZE);
                        }
                    }

                    if (!fnd) {
                        m_devs.removeAt(d);
                        d--;
                    }
                }

                /* Добавляем новые устройства, которые не были до этого в
                 * списках */
				for (signed i=0; i < devcnt; i++) {
                    QString cur_name = QSTRING_FROM_CSTR(info_list[i].devname);
    #ifdef LQMEAS_DEV_E502
                    if (cur_name == E502TypeInfo::name()) {
                        m_devs.append(QSharedPointer<Device>(new E502(&info_list[i])));
                    }
    #endif
    #ifdef LQMEAS_DEV_L502
                    if (cur_name == L502TypeInfo::name()) {
                        m_devs.append(QSharedPointer<Device>(new L502(&info_list[i])));
                    }
    #endif
                }
            }
        }

        return m_devs + m_tcpDevs.values() + m_svcDevs;
    }

    x502Resolver *x502Resolver::resolver() {
        static x502Resolver resolver;
        return &resolver;
    }

#ifdef LQMEAS_USE_NETWORK_BROWSER
    DeviceNetworkBrowser *x502Resolver::netBrowser() {
        static e502NetBrowser browser;
        return &browser;
    }
#endif

    Error x502Resolver::addSvcRec(QSharedPointer<DeviceSvcRecord> svc)   {
        Error err = Error::Success();

        e502SvcRecord *e502_svc = dynamic_cast<e502SvcRecord*>(svc.data());
        if (e502_svc) {
            t_x502_devrec devrec;
            err = x502::error(E502_MakeDevRecordByEthSvc(&devrec, e502_svc->m_hnd, 0, m_con_tout));
            if (err.isSuccess()) {
                m_svcDevs.append(QSharedPointer<Device>(new E502(&devrec)));
                m_svcList.append(svc);
            }
        }
        return err;
    }

    void x502Resolver::remSvcRec(QSharedPointer<DeviceSvcRecord> svc) {
        for (int i=0; i < m_svcList.size(); i++) {
            if (svc->isEqual(m_svcList[i].data())) {
                m_svcDevs.removeAt(i);
                m_svcList.removeAt(i);
                break;
            }
        }
    }

    void x502Resolver::updateSvcRec(QSharedPointer<DeviceSvcRecord> svc) {
        /* не требуется, т.к. можем открыть устройство и по старой записи */
		Q_UNUSED(svc);
    }
}
