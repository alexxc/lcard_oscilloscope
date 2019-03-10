#include "DacSignalRamp.h"

namespace LQMeas {
    DacSignalRamp::DacSignalRamp(double init_amp, double end_amp, double freq) :
        DacSignal(signalTypeName(), 2, freq, true), m_init_amp(init_amp), m_end_amp(end_amp) {

    }

    double DacSignalRamp::signalGenNextPoint() {
        double val = m_init_amp + (m_end_amp - m_init_amp) * m_cur_pt/(m_pt_len-1);
        if (++m_cur_pt == m_pt_len)
            m_cur_pt = 0;
        return val;
    }

    void DacSignalRamp::setRealParams(double sigFreq, double ptFreq) {
        m_pt_len = ptFreq/sigFreq + 0.5;
        Q_ASSERT(m_pt_len >= 2);
        return DacSignal::setRealParams(sigFreq, ptFreq);
    }

}


