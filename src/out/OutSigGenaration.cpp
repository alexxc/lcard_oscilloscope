#include "OutSigGenaration.h"
#include "LQMeasStudio.h"
#include "lqmeas/ifaces/out/DevOutSync.h"
#include "lqmeas/ifaces/out/DevOutSyncStatusTracker.h"
#include <QThread>

namespace LQMeasStudio {
    void OutSigGenaration::procStartPrepare() {
        m_devs.clear();
        Q_FOREACH (QSharedPointer<LQMeas::Device> dev, LQMeasStudioEnv->deviceTree()->selectedList()) {
            LQMeas::DevOutSync *out_sync = dev->devOutSync();
            if (out_sync && out_sync->outSyncHasEnabledChannels()) {
                LQMeas::Error err = out_sync->outSyncGenPrepare();
                if (!err.isSuccess()) {
                    emit deviceError(dev, err, tr("Data setup error"), tr("Error occured during output generation initialization"));
                } else {
                    m_devs.append(dev);
                }
            }
        }
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        /* дополнительная задержка, чтобы записанные данные уже были бы подгружены
           к моменту запуска */
        /** @note Возможно следует подумать, как сделать более корректно */
        QThread::msleep(500);
#endif
    }

    void OutSigGenaration::procStart() {
        Q_FOREACH (QSharedPointer<LQMeas::Device> dev, m_devs) {
            LQMeas::DevOutSyncStatusTracker *tracker = dev->devOutSync()->outSyncStatusTracker();
            connect(tracker, SIGNAL(errorOccured(LQMeas::Error)), this, SLOT(onGenError(LQMeas::Error)));
            connect(tracker, SIGNAL(underflowOccured()), this, SLOT(onGenUnderflow()));
            LQMeas::Error err = dev->devOutSync()->outSyncGenStart();
            if (!err.isSuccess()) {
                emit deviceError(dev, err, tr("Data generation error"), tr("Error occured during output generation start"));
            }
        }
    }

    void OutSigGenaration::procStopRequest() {
        Q_FOREACH (QSharedPointer<LQMeas::Device> dev, m_devs) {
            LQMeas::DevOutSync *devout = dev->devOutSync();
            devout->outSyncGenStopRequest();

            LQMeas::DevOutSyncStatusTracker *tracker = devout->outSyncStatusTracker();
            disconnect(tracker, SIGNAL(errorOccured(LQMeas::Error)), this, SLOT(onGenError(LQMeas::Error)));
            disconnect(tracker, SIGNAL(underflowOccured()), this, SLOT(onGenUnderflow()));
        }
    }

    void OutSigGenaration::procStop() {
        Q_FOREACH (QSharedPointer<LQMeas::Device> dev, m_devs) {
            dev->devOutSync()->outSyncGenStop();
        }
    }

    void OutSigGenaration::procClear() {
        m_devs.clear();
    }

	unsigned int OutSigGenaration::runningDevs() {
		unsigned int cnt = 0;
        Q_FOREACH (QSharedPointer<LQMeas::Device> dev, m_devs) {
            if (dev->devOutSync()->outSyncGenRunning())
                cnt++;
        }
        return cnt;
    }

    void OutSigGenaration::onGenError(LQMeas::Error err) {
        LQMeas::DevOutSyncStatusTracker *tracker = qobject_cast<LQMeas::DevOutSyncStatusTracker *>(sender());
        if (tracker) {
            QSharedPointer<LQMeas::Device> dev = getDev(tracker);
            if (dev) {
                emit deviceError(dev, err, tr("Data generation error"), tr("Error occured during data generation"));
            }
        }
    }


    void OutSigGenaration::onGenUnderflow() {
        LQMeas::DevOutSyncStatusTracker *tracker = qobject_cast<LQMeas::DevOutSyncStatusTracker *>(sender());
        if (tracker) {
            QSharedPointer<LQMeas::Device> dev = getDev(tracker);
            if (dev) {
                emit deviceGenUnderflow(dev);
            }
        }
    }

    QSharedPointer<LQMeas::Device> OutSigGenaration::getDev(LQMeas::DevOutSyncStatusTracker *tracker) {
        QSharedPointer<LQMeas::Device> ret_dev;
        Q_FOREACH (QSharedPointer<LQMeas::Device> dev, m_devs) {
            if (dev->devOutSync()->outSyncStatusTracker() == tracker) {
                ret_dev = dev;
                break;
            }
        }
        return ret_dev;
    }
}
