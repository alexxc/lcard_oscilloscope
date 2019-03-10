#ifndef LQMEAS_DACSIGNALSIN_H
#define LQMEAS_DACSIGNALSIN_H

#include "DacSignal.h"
#include "../OutSignalFabric.h"
#include <QString>

namespace LQMeas {
    class DacSignalSin : public DacSignal {
    public:
        static QString signalTypeName() {return "DacSignalSin";}
        static QString signalName() {return OutSignalFabric::tr("Sinus");}


        static int signalMinPoints() {return 2;}
        void signalGenInit() {m_cur_pha = m_pha;}
        double signalGenNextPoint();

        DacSignalSin(double amp, double offs, double freq, double pha = 0, bool periodStrictPoints = false);

        double amplitude() const {return m_amp;}
        double offset() const {return m_offs;}
        double phase() const {return m_pha;}

        QString description();
    private:
        double m_cur_pha;
        double m_amp;
        double m_offs;
        double m_pha;
    };
}


#endif // LQMEAS_DACSIGNALSIN_H
