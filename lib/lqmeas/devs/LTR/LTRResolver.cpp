#include "LTRResolver.h"
#include "lqmeas_config.h"
#include "lqmeas/devs/LTR/crates/LTRCrate.h"
#include "lqmeas/devs/DeviceValidator.h"
#include "lqmeas/devs/DeviceNameValidator.h"
#include "ltr/include/ltrapi.h"



#ifdef LQMEAS_DEV_LTR11
    #include "modules/LTR11/LTR11.h"
#endif
#ifdef LQMEAS_DEV_LTR24
    #include "modules/LTR24/LTR24.h"
#endif
#ifdef LQMEAS_DEV_LTR25
    #include "modules/ltr25.h"
#endif
#ifdef LQMEAS_DEV_LTR35
    #include "modules/ltr35.h"
#endif
#ifdef LQMEAS_DEV_LTR51
    #include "modules/ltr51.h"
#endif
#ifdef LQMEAS_DEV_LTR210
    #include "modules/ltr210.h"
#endif


namespace LQMeas {
    QList<QSharedPointer<LTRModule> > LTRResolver::getModuleList(QStringList nameFilter) {
        QScopedPointer<DeviceValidator> val(new DeviceNameValidator(nameFilter));
        return getModuleList(val.data());
    }

    QList<QSharedPointer<LTRModule> > LTRResolver::getModuleList(const DeviceValidator *validator) {
        QList<QSharedPointer<LTRModule> > fndList;

        QList<QSharedPointer<LTRCrate> > crates = getCratesList();
        Q_FOREACH (QSharedPointer<LTRCrate> crate, crates) {
            fndList.append(crate->getModuleList(validator));
        }
        return fndList;
    }

    QList<QSharedPointer<LTRCrate> > LTRResolver::getCratesList() {
        TLTR srv;
        INT err;

        QList<QSharedPointer<LTRCrate> >  ret;

        LTR_Init(&srv);
        srv.cc = LTR_CC_CHNUM_CONTROL;
        strcpy(srv.csn, LTR_CSN_SERVER_CONTROL);

        err = LTR_Open(&srv);
        if (err==LTR_OK) {
            char serials[LTR_CRATES_MAX][LTR_CRATE_SERIAL_SIZE];
            err = LTR_GetCrates(&srv, (BYTE*)serials);
            if (err==LTR_OK) {
                for (unsigned i=0; i < LTR_CRATES_MAX; i++) {
                    if (serials[i][0]!='\0') {
                        TLTR chnd;
                        INT crate_err = 0;
                        LTR_Init(&chnd);
                        chnd.cc = LTR_CC_CHNUM_CONTROL;
                        memcpy(chnd.csn, serials[i], LTR_CRATE_SERIAL_SIZE);
                        crate_err = LTR_Open(&chnd);
                        if (crate_err==LTR_OK) {
                            TLTR_CRATE_INFO crate_info;
                            QSharedPointer<LTRCrate> crate;
                            QString serial = QString::fromLatin1(serials[i]);
                            crate_err = LTR_GetCrateInfo(&chnd, &crate_info);

                            LTR_Close(&chnd);

                            if (crate_err == LTR_OK) {
                                crate = getCrate(serial);
                                if (!crate) {
                                    crate = QSharedPointer<LTRCrate>(
                                            new LTRCrate(chnd, serial, crate_info));
                                    m_crates.append(crate);
                                }
                                ret.append(crate);
                            }
                        }
                    }
                }
            }
            LTR_Close(&srv);
        }
        return ret;
    }


    QList<QSharedPointer<Device> > LTRResolver::getDevList() {
        QList<QSharedPointer<LTRCrate> > crates = getCratesList();
        QList<QSharedPointer<Device> > retlist;
        Q_FOREACH (QSharedPointer<LTRCrate> crate, crates) {
            retlist.append(crate);
        }
        return retlist;
    }

    QSharedPointer<LTRCrate> LTRResolver::getCrate(QString serial) {
        Q_FOREACH (QSharedPointer<LTRCrate> curCrate, m_crates) {
            if (curCrate->serial()==serial) {
                return curCrate;
            }
        }
        return QSharedPointer<LTRCrate>();
    }

    QSharedPointer<LTRCrate> LTRResolver::getCrate(const LTRCrate *crate) {
        Q_FOREACH (QSharedPointer<LTRCrate> curCrate, m_crates) {
            if (curCrate.data()==crate) {
                return curCrate;
            }
        }
        return QSharedPointer<LTRCrate>();
    }

    LTRResolver *LTRResolver::resolver() {
        static LTRResolver resolver;
        return &resolver;
    }

    LTRResolver::LTRResolver() {

    }

    QSharedPointer<LTRModule> LTRResolver::createModule(QSharedPointer<LTRCrate> crate, unsigned slot, quint16 mid) {
        switch (mid) {
    #ifdef LQMEAS_DEV_LTR11
            case LTR11::typeModuleID:
                return QSharedPointer<LTRModule>(new LTR11(crate, slot));
    #endif
    #ifdef LQMEAS_DEV_LTR24
            case LTR24::typeModuleID:
                return QSharedPointer<LTRModule>(new LTR24(crate, slot));
    #endif
    #ifdef LQMEAS_DEV_LTR25
            case LTR25::typeModuleID:
                return QSharedPointer<LtrModule>(new LTR25(crate, slot));
    #endif
    #ifdef LQMEAS_DEV_LTR35
            case LTR35::typeModuleID:
                return QSharedPointer<LtrModule>(new LTR35(crate, slot));
    #endif
    #ifdef LQMEAS_DEV_LTR51
            case LTR51::typeModuleID:
                return QSharedPointer<LtrModule>(new LTR51(crate, slot));
    #endif
    #ifdef LQMEAS_DEV_LTR210
            case LTR210::typeModuleID:
                return QSharedPointer<LtrModule>(new LTR210(crate, slot));
    #endif
        }

        return QSharedPointer<LTRModule>();
    }
}

