/* определяем GUID один раз внутри данного файла */
#define INITGUID

#include "lcompResolver.h"
#include "lcomp_headers.h"
#include "lqmeas_config.h"
#include "lqmeas/log/Log.h"

#ifdef LQMEAS_DEV_E2010
    #include "modules/e2010.h"
#endif
#ifdef LQMEAS_DEV_E14_440
    #include "modules/e14_440.h"
#endif
#ifdef LQMEAS_DEV_E14_140
    #include "modules/E14_140/E14_140.h"
#endif

#include <QLibrary>

namespace LQMeas {
    lcompResolver::lcompResolver() : m_create_func(NULL) {

    }

    lcompResolver *lcompResolver::resolver() {
        static lcompResolver resolver;
        return &resolver;
    }

    QList<QSharedPointer<Device> > lcompResolver::getDevList() {
        if (!m_create_func) {
            QLibrary lib(
#ifdef Q_OS_WIN
                        sizeof(void*)==8 ? "lcomp64" :
#endif
                        "lcomp");
            m_create_func = (CREATEFUNCPTR)lib.resolve("CreateInstance");
            if (m_create_func==NULL) {
                LQMeasLog->warning(QObject::tr("Cannot load lcomp dynamic library"));
            }
        }

        if (m_create_func != NULL) {
            for (unsigned slot=0; slot < max_devs_cnt; slot++) {
                LUnknown *pIUnknown = (LUnknown *)m_create_func(slot);
                if (pIUnknown) {
                    IDaqLDevice* pI;
                    HRESULT hr = pIUnknown->QueryInterface(IID_ILDEV,(void**)&pI);
                    if (hr == S_OK) {
                        HANDLE hVxd;
                        pIUnknown->Release();
                        hVxd = pI->OpenLDevice(); // открываем устройство
                        if(hVxd != INVALID_HANDLE_VALUE) {
                            SLOT_PAR sl;
                            ULONG status = pI->GetSlotParam(&sl);
                            pI->CloseLDevice();
                            if(status == L_SUCCESS) {
                                QSharedPointer<Device> dev;
                                switch (sl.BoardType) {
    #ifdef LQMEAS_DEV_E2010
                                    case E2010:
                                    case E2010B:
                                        dev = QSharedPointer<Device>(new e2010(&sl, slot));
                                        break;

    #endif
    #ifdef LQMEAS_DEV_E14_440
                                    case E440:
                                        dev = QSharedPointer<Device>(new e14_440(pI, &sl, slot));
                                        break;
    #endif
    #ifdef LQMEAS_DEV_E14_140
                                    case E140:
                                        dev = QSharedPointer<Device>(new E14_140(pI, &sl, slot));
                                        break;
    #endif
                                    default:
                                        pI->Release();
                                        break;
                                }

                                if (dev) {
                                    m_devs[slot] = dev;
                                } else {
                                    m_devs.remove(slot);
                                }
                            }
                        }
                    }
                } else {
                    if (LCOMP_LAST_ERROR != L_ERROR_INUSE)  {
                        m_devs.remove(slot);
                    }
                }
            }
        }
        return m_devs.values();
    }

}
