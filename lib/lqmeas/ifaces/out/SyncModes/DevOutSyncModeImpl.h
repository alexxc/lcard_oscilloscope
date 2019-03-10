#ifndef LQMEAS_DEVOUTSYNCMODEIMPL_H
#define LQMEAS_DEVOUTSYNCMODEIMPL_H

#include <QObject>
#include <QSharedPointer>
#include "lqmeas/ifaces/out/DevOutSyncConfig.h"
#include "lqmeas/ifaces/out/OutRamSignalGenerator.h"

namespace LQMeas {
    class DevOutSyncModeImpl : public QObject {
        Q_OBJECT
    public:
        virtual ~DevOutSyncModeImpl() {}

        virtual DevOutSyncConfig::OutSyncRamMode outSyncMode() = 0;
        /* Установка генератора для генерации новых точек сигнала.
           В данной функции класс реализации должен поправить сигналы, если это
           требуется в данном режиме и запомнить генератор для дальнейших функций. */
        virtual Error outSyncModeImplSigSet(QSharedPointer<OutRamSignalGenerator> ramSigGenerator) = 0;
        virtual Error outSyncModeImplStartPrepare() = 0;
        virtual Error outSyncModeImplStart() = 0;
        virtual Error outSyncModeImplStopRequest(unsigned tout) = 0;
        virtual Error outSyncModeImplStopWait() = 0;
        virtual Error outSyncModeImplSigUpdate() = 0;
    Q_SIGNALS:
        void underflowOccured();
        void errorOccured(LQMeas::Error err);
    };
}

#endif // LQMEAS_DEVOUTSYNCMODEIMPL_H
