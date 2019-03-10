#ifndef DACSIGNALRAMP_H
#define DACSIGNALRAMP_H

#include "DacSignal.h"
#include "../OutSignalFabric.h"

namespace LQMeas {
    class DacSignalRamp : public DacSignal {
    public:
        static QString signalTypeName() {return "DacSignalRamp";}
        static QString signalName() {return OutSignalFabric::tr("Ramp");}

        DacSignalRamp(double init_amp, double end_amp, double freq);
        double signalGenNextPoint();
        void signalGenInit() {m_cur_pt = 0;}

        virtual void setRealParams(double sigFreq, double ptFreq);

        double initAmplitude() const {return m_init_amp;}
        double endAmplitude()  const {return m_end_amp;}


    private:
        size_t m_cur_pt;

        double m_init_amp, m_end_amp;
        size_t m_pt_len;
    };
}


#endif // DACSIGNALRAMP_H
