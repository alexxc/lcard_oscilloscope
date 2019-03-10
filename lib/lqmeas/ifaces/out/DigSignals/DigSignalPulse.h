#ifndef LQMEAS_DIGSIGNALPULSE_H
#define LQMEAS_DIGSIGNALPULSE_H

#include "DigSignal.h"

namespace LQMeas {
    class DigSignalPulse : public DigSignal {
    public:
        static QString signalTypeName() {return "DigSignalPulse";}

        DigSignalPulse(double freq, double dutyCycle);
        unsigned signalGenNextPoint();
        void signalGenInit() {m_cur_pt = 0;}

        double realDuty() const {return (double)m_pt_duty/m_pt_len;}

        virtual void setRealParams(double sigFreq, double ptFreq);
    private:
        unsigned m_cur_pt;

        double m_dutyCycle;
        double m_freq;

        unsigned m_pt_len;
        unsigned m_pt_duty;
    };
}


#endif // LQMEAS_DIGSIGNALPULSE_H
