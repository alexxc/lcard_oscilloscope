#ifndef LQMEAS_DACSIGNAL_H
#define LQMEAS_DACSIGNAL_H

#include "lqmeas/ifaces/out/OutSignal.h"
#include <QtGlobal>

namespace LQMeas {
    class DacSignal : public OutSignal {
    public:
        virtual double signalGenNextPoint() = 0;
    protected:
        DacSignal(QString typeName, unsigned min_points, double freq = unspecifiedFreq(), bool periodStrictPoints = true) :
            OutSignal(typeName, min_points, freq, periodStrictPoints) {}
    };
}


#endif // LQMEAS_DACSIGNAL_H
