#include "LTRModule.h"
#include "lqmeas/lqtdefs.h"

#include <QCoreApplication>

namespace LQMeas {
    LTRModule::LTRModule(QSharedPointer<LTRCrate> crate,
                         unsigned slot, unsigned mid, DeviceConfig *defaultCfg,
                         const DeviceInfo *info, QObject *parent) :
        Device(defaultCfg, info, parent), m_crate(crate), m_slot(slot), m_mid(mid) {
    }

    LTRModule::~LTRModule() {

    }


    Error LTRModule::error(int err) const {
        return err == LTR_OK ? Error::Success() : Error(err, errorString(err),
                                                        Error::ERRTYPE_DEVNATIVE_LTR);
    }

    QString LTRModule::errorString(int err) const {
        return  QSTRING_FROM_CSTR(LTR_GetErrorString(err));
    }

    Error LTRModule::rawWordsReceive(DWORD *wrds, unsigned size, unsigned tout, unsigned *recvd_size) {
        Error err = Error::Success();
        INT recv_res = LTR_Recv(channel(), wrds, 0, size, tout);
        if (recv_res >= 0) {
            *recvd_size = recv_res;
        } else {
            err = error(recv_res);
        }
        return err;
    }

    Error LTRModule::rawWordsSend(const DWORD *wrds, unsigned size, unsigned tout, unsigned *sent_size) {
        Error err = Error::Success();
        int send_res = LTR_Send(channel(), wrds, size, tout);
        if (send_res >= 0) {
            *sent_size = send_res;
        } else {
            err = error(send_res);
        }
        return err;
    }
}


