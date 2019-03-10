#include "DigSignalPulse.h"

namespace LQMeas {
    DigSignalPulse::DigSignalPulse(double freq, double dutyCycle) :
        DigSignal(signalTypeName(), 2, freq), m_dutyCycle(dutyCycle) {

        Q_ASSERT(freq > 0);
        Q_ASSERT((dutyCycle >= 0.) && (dutyCycle <= 1.));
    }

    unsigned DigSignalPulse::signalGenNextPoint() {
        unsigned val = m_cur_pt < m_pt_duty ? 1 : 0;
        if (++m_cur_pt == m_pt_len)
            m_cur_pt = 0;
        return val;
    }

    void DigSignalPulse::setRealParams(double sigFreq, double ptFreq) {
        m_pt_len = ptFreq/sigFreq + 0.5;
        m_pt_duty = m_dutyCycle * m_pt_len + 0.5;

        return DigSignal::setRealParams(sigFreq, ptFreq);
    }
}
