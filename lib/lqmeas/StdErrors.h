#ifndef LQMEAS_STDERRORS_H
#define LQMEAS_STDERRORS_H

#include "Error.h"
#include <QObject>

namespace LQMeas {
    class StdErrors : public QObject {
        Q_OBJECT
    public:
        static Error DeviceUnsupConfig()  {
            return Error(-1, tr("This configuration is not supported by device"));
        }

        static Error SendBusy() {
            return Error(-2, tr("Data sending in progress. No free space for new data"));
        }

        static Error StreamStopWait() {
            return Error(-3, tr("Stream completion timeout"));
        }
        static Error RecvInsufficientData() {
            return Error(-4, tr("Received less data than requested"));
        }
        static Error InvalidAdcConfig() {
            return Error(-5, tr("Invalid data acquisition parameters"));
        }
        static Error FeatureNotImplemeted() {
            return Error(-6, tr("Feature is not implemented!"));
        }
        static Error OutSignalPointCount() {
            return Error(-7, tr("Output signal requires too many points"));
        }
        static Error OutGenRunning() {
            return Error(-7, tr("Output generation already running"));
        }
        static Error InvalidConfigType() {
            return Error(-8, tr("Invalid configuration type"));
        }
        static Error DataProcess() {
            return Error(-9, tr("Data processing error"));
        }
        static Error UnsupOutSyncRamMode() {
            return Error(-10, tr("Unsupported synchronous output generation mode"));
        }
    };
}

#endif // LQMEAS_STDERRORS_H
