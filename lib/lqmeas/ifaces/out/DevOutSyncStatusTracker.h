#ifndef LQMEAS_DEVOUTSYNCSTATUSTRACKER_H
#define LQMEAS_DEVOUTSYNCSTATUSTRACKER_H

#include <QObject>

#include "lqmeas/Error.h"

namespace LQMeas {
    class Device;

    /* Объект позволяет отслеживать ошибки при выполнении синхронной генерации
       сигналов */
    class DevOutSyncStatusTracker : public QObject {
        Q_OBJECT
    public:
        DevOutSyncStatusTracker(QObject *parent, Device* dev);

        Device *device() const {return m_dev;}

    public Q_SLOTS:
        void setError(LQMeas::Error err);
    Q_SIGNALS:
        /* сигнал, указывающий, что произошло опустошение буфера на вывод.
           если это сделано не преднамерено, то это привело к разрыву непрерыного
           сигнала */
        void underflowOccured();
        /* сигнал, указывающий, что произошла ошибка при выводе */
        void errorOccured(LQMeas::Error err);
    private:
        Device *m_dev;
        LQMeas::Error m_err;
    };
}

#endif // LQMEAS_DEVOUTSYNCSTATUSTRACKER_H
