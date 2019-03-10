#include "e502NetBrowser.h"
#include "e502api.h"
#include "x502.h"
#include "e502SvcRecord.h"


namespace LQMeas {

    e502NetBrowser::e502NetBrowser() {

    }

    void e502NetBrowser::run() {
        t_e502_eth_svc_browse_hnd browse_hnd;
        int32_t err = E502_EthSvcBrowseStart(&browse_hnd, 0);
        if (err != X502_ERR_OK) {
            setError(x502::error(err), tr("Cannot start network browse for E502 modules"));
        } else {
            while ((err == X502_ERR_OK) && !stopIsRequested()) {
                t_e502_eth_svc_record_hnd svc_hnd;
                uint32_t event;
                err = E502_EthSvcBrowseGetEvent(browse_hnd, &svc_hnd, &event, NULL, 100);
                if (err != X502_ERR_OK) {
                    setError(x502::error(err), tr("Error occured during wait for new browse event for E502 modules"));
                } else {
                    if (event != E502_ETH_SVC_EVENT_NONE) {
                        QSharedPointer<DeviceSvcRecord> rec(new e502SvcRecord(svc_hnd));

                        if (event == E502_ETH_SVC_EVENT_ADD) {
                            addRecord(rec);
                        } else if (event == E502_ETH_SVC_EVENT_CHANGED) {
                            changeRecord(rec);
                        } else if (event == E502_ETH_SVC_EVENT_REMOVE) {
                            removeRecord(rec);
                        }
                    }
                }
            }

            E502_EthSvcBrowseStop(browse_hnd);
        }
    }
}

