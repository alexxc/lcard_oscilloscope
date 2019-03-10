#ifndef LQMEAS_DIGSIGNAL_H
#define LQMEAS_DIGSIGNAL_H

#include "lqmeas/ifaces/out/OutSignal.h"
#include <QtGlobal>

namespace LQMeas {
    class DigSignal : public OutSignal {
    public:
        virtual unsigned signalGenNextPoint() = 0;

    protected:
        DigSignal(QString typeName, unsigned min_points, double freq = unspecifiedFreq()) :
            OutSignal(typeName, min_points, freq, true) {}
    };
}

#endif // LQMEAS_DIGSIGNAL_H
