#ifndef LQMEAS_DACSIGNALCONST_H
#define LQMEAS_DACSIGNALCONST_H

#include "DacSignal.h"
#include "../OutSignalFabric.h"
#include <QString>

namespace LQMeas {
    class DacSignalConst : public DacSignal {
    public:
        static QString signalTypeName() {return "DacSignalConst";}
        static QString signalName() {return OutSignalFabric::tr("Const");}

        static int signalMinPoints() {return 1;}

        void signalGenInit() {}
        double signalGenNextPoint() {return m_amp;}


        DacSignalConst(double amp);

        double amplitude() const {return m_amp;}
    private:
        double m_amp;
    };
}

#endif // LQMEAS_DACSIGNALCONST_H
