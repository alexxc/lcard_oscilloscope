#include "DacSignalSin.h"
#include <math.h>

#ifndef M_PI
    #define M_PI       3.14159265358979323846
#endif

namespace LQMeas {

    double DacSignalSin::signalGenNextPoint() {
        double val = m_amp * sin(m_cur_pha) + m_offs;
        m_cur_pha += 2.*M_PI * signalRealFreq() / signalGenFreq();
        return val;
    }

    DacSignalSin::DacSignalSin(double amp, double offs, double freq, double pha,
                               bool periodStrictPoints):
                                DacSignal(signalTypeName(), signalMinPoints(),
                                          freq, periodStrictPoints), m_amp(amp),
                                m_offs(offs), m_pha(pha) {
        Q_ASSERT(freq > 0);
    }
}


