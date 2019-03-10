#include "DevOutSyncStatusTracker.h"

namespace LQMeas {
    DevOutSyncStatusTracker::DevOutSyncStatusTracker(QObject *parent, Device *dev) :
        QObject(parent), m_dev(dev) {

    }

    void DevOutSyncStatusTracker::setError(LQMeas::Error err) {
        m_err = err;
        if (!m_err.isSuccess())
            Q_EMIT errorOccured(err);
    }
}
