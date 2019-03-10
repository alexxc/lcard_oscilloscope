#include "DacSignalPulse.h"

namespace LQMeas {

    DacSignalPulse::DacSignalPulse(double amp_pulse, double amp_pause, double freq, double dutyCycle) :
        DacSignal(signalTypeName(), 2, freq), m_dutyCycle(dutyCycle),
        m_amp_pulse(amp_pulse), m_amp_pause(amp_pause) {

        Q_ASSERT(freq > 0);
        Q_ASSERT((dutyCycle >= 0.) && (dutyCycle <= 1.));
    }

    double DacSignalPulse::signalGenNextPoint() {
        double val = m_cur_pt < m_pt_duty ? m_amp_pulse : m_amp_pause;
        if (++m_cur_pt == m_pt_len)
            m_cur_pt = 0;        
        return val;
    }

    void DacSignalPulse::setRealParams(double sigFreq, double ptFreq) {
        m_pt_len = ptFreq/sigFreq + 0.5;
        m_pt_duty = m_dutyCycle * m_pt_len + 0.5;

        return DacSignal::setRealParams(sigFreq, ptFreq);
    }

}

