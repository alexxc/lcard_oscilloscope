#include "LTRCrate.h"
#include "LTRCrateInfo.h"
#include <QScopedPointer>
#include "lqmeas/devs/LTR/modules/LTRModule.h"
#include "lqmeas/devs/LTR/LTRResolver.h"
#include "lqmeas/devs/DeviceValidator.h"
#include "lqmeas/devs/DeviceNameValidator.h"
#include "lqmeas/log/Log.h"
#include "lqmeas/lqtdefs.h"


namespace LQMeas {
    LTRCrate::LTRCrate(TLTR hnd, QString serial, TLTR_CRATE_INFO info) :
        Device(0, new LTRCrateInfo(LTRCrateTypeInfo::typeInfo((en_LTR_CrateTypes)info.CrateType), serial)),
        m_hnd(hnd) {

        m_iface = info.CrateInterface == LTR_CRATE_IFACE_USB ? InterfaceUSB :
                  info.CrateInterface == LTR_CRATE_IFACE_TCPIP ? InterfaceEthernet :
                                                                 InterfaceUnknown;
        m_modules.resize(slotCount());
    }

    LTRCrate::~LTRCrate() {
        if (isOpened())
            close();
    }


    unsigned LTRCrate::slotCount() const {
        return devspecInfo()->slotCount();
    }

    bool LTRCrate::isOpened() const {
        return LTR_IsOpened(&m_hnd)==LTR_OK;
    }



    quint32 LTRCrate::ltrdIpAddr() {
        return m_hnd.saddr;
    }

    quint16 LTRCrate::ltrdTcpPort() {
        return m_hnd.sport;
    }

    QList<QSharedPointer<LTRModule> > LTRCrate::getModuleList(QStringList nameFilter) {
        QScopedPointer<DeviceValidator> val(new DeviceNameValidator(nameFilter));
        return getModuleList(val.data());
    }



    QList<QSharedPointer<LTRModule> > LTRCrate::getModuleList(const DeviceValidator *validator) {
        QList<QSharedPointer<LTRModule> > ret;
        bool close_req = false;
        if (!isOpened()) {
            if (open().isSuccess()) {
                close_req = true;
            }
        }

        if (isOpened()) {
            WORD mids[LTR_MODULES_PER_CRATE_MAX];
            int err = LTR_GetCrateModules(&m_hnd, mids);
            if (err==LTR_OK) {
                for (int slot=0; (slot < LTR_MODULES_PER_CRATE_MAX) && (slot < m_modules.size()); slot++) {
                    if ((mids[slot]!=LTR_MID_EMPTY) && (mids[slot]!=LTR_MID_IDENTIFYING)) {
                        QSharedPointer<LTRModule> module = m_modules[slot];
                        if (!module || (module->mid()!= mids[slot])) {
                            m_modules[slot] = module = LTRResolver::createModule(
                                        LTRResolver::resolver()->getCrate(this),
                                        slot+LTR_CC_CHNUM_MODULE1, mids[slot]);
                        }

                        if (module && ((validator == 0) || validator->deviceValid(module.data()))) {
                            ret.append(module);
                        }

                    } else {
                        /* если пуст - то удаляем старую информацию о модуле */
                        m_modules[slot] = QSharedPointer<LTRModule>();
                    }
                }
            }


            if (close_req)
                close();
        }
        return ret;
    }

    QList<QSharedPointer<Device> > LTRCrate::getDevList() {
		QList<QSharedPointer<LTRModule> > ltrlist = getModuleList(nullptr);
        QList<QSharedPointer<Device> > retlist;
        Q_FOREACH (QSharedPointer<LTRModule> ltr, ltrlist) {
            retlist.append(ltr);
        }
        return retlist;
    }

    const LTRCrateInfo *LTRCrate::devspecInfo() const {
        return static_cast<const LTRCrateInfo *>(devInfo());
    }

    Error LTRCrate::error(int err) const {
        return err == LTR_OK ? Error::Success() :
                               Error(err, QSTRING_FROM_CSTR(LTR_GetErrorString(err)),
                                     Error::ERRTYPE_DEVNATIVE_LTR);
    }


    Error LTRCrate::protOpen() {
        return error(LTR_Open(&m_hnd));
    }

    Error LTRCrate::protClose() {
        return error(LTR_Close(&m_hnd));
    }

    Error LTRCrate::protConfigure(const DeviceConfig *cfg) {
        return Error::Success();
    }
}
