#ifndef LQMEAS_DACSIGNALPULSE_H
#define LQMEAS_DACSIGNALPULSE_H

#include "DacSignal.h"
#include "../OutSignalFabric.h"

namespace LQMeas {
    class DacSignalPulse : public DacSignal {
    public:
        static QString signalTypeName() {return "DacSignalPulse";}
        static QString signalName() {return OutSignalFabric::tr("Pulse");}

        DacSignalPulse(double amp_pulse, double amp_pause, double freq, double dutyCycle);
        double signalGenNextPoint();
        void signalGenInit() {m_cur_pt = 0;}

        double realDuty() const {return (double)m_pt_duty/m_pt_len;}

        virtual void setRealParams(double sigFreq, double ptFreq);

        double pulseAmplitude() const {return m_amp_pulse;}
        double pauseAmplitude() const {return m_amp_pause;}
        double dutyCycle() const {return m_dutyCycle;}

    private:
        size_t m_cur_pt;

        double m_dutyCycle;
        double m_freq;
        double m_amp_pause, m_amp_pulse;

        size_t m_pt_len;
        size_t m_pt_duty;
    };
}

#endif // DACSIGNALPULSE_H
