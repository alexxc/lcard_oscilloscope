#ifndef LQMEAS_OUTSIGNAL_H
#define LQMEAS_OUTSIGNAL_H

#include <QString>

namespace LQMeas {
    class OutSignal {
    public:
        static double unspecifiedFreq() {return -1.;}

        virtual void setRealParams(double sigFreq, double ptFreq)
            {m_real_sig_freq = sigFreq; m_real_pt_freq = ptFreq;}

        double signalRealFreq() const {return m_real_sig_freq;}
        double signalGenFreq() const {return m_real_pt_freq;}

        double signalFreq() const {return m_freq;}
        unsigned minPoints() const {return m_min_points;}

        bool periodStrictPoints() const {return m_period_strict_pts;}

        QString typeName() const {return m_typeName;}

        virtual void signalGenInit() = 0;

    protected:
        OutSignal(QString typeName, unsigned min_points, double freq = unspecifiedFreq(), bool periodStrictPoints=true) :
            m_typeName(typeName), m_min_points(min_points), m_freq(freq), m_period_strict_pts(periodStrictPoints)
        {}


    private:        
        double m_real_sig_freq, m_real_pt_freq;
        unsigned m_min_points;
        double m_freq;
        bool m_period_strict_pts;
        QString m_typeName;
    };
}


#endif // LQMEAS_OUTSIGNAL_H
