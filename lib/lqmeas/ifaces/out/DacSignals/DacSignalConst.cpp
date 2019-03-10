#include "DacSignalConst.h"

namespace LQMeas {
    DacSignalConst::DacSignalConst(double amp) :
        DacSignal(signalTypeName(), signalMinPoints()), m_amp(amp) {
    }
}
