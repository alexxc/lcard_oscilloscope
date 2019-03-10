#include "DevOutSyncModeImplCycle.h"
#include "DevOutSyncIfaceCycle.h"
#include "lqmeas/StdErrors.h"
#include "lqmeas/devs/Device.h"
#include "lqmeas/ifaces/out/DevOutSync.h"
#include "lqmeas/log/Log.h"
#include <qmath.h>

// функция вычисляет НОД
static int gcd( int a, int b ) {
    return b ? gcd( b, a % b ) : a;
}

// функция вычисляет НОК, используя НОД
static int lcm( int a, int b ){
    return a / gcd( a, b ) * b;
}


// source:
// http://www.isi.edu/~johnh/BLOG/1999/0728_RATIONAL_PI/fast_convergence.txt
// theory:
// https://en.wikipedia.org/wiki/Continued_fraction#Best_rational_approximations

static int rat_approx(double val, long* out_n, long* out_d, long max_d, double rel_err) {
    const size_t max_iter = 20;
    long coeff[max_iter];
    int sign = 1;
    double x = val;
	for (int i = 0; i < max_iter; ++i) {
        if (x < 0.0) {
            sign = -sign;
            x = -x;
        }
		long ipart = (long)x + (x - floor(x) > 0.5);
        coeff[i] = ipart * sign;
        x = 1.0 / (x - ipart);

        // compute numerator and denominator
        long num = coeff[i], den = 1;
		for (int j = i - 1; j >= 0; --j) {
            long t = num;
            num = coeff[j] * num + den;
            den = t;
        }
        if (den < 0) {
            num = -num;
            den = -den;
        }
        if (den > max_d)
            break;
        *out_n = num;
        *out_d = den;
		double approx_val = (double)num / den;
        double err = fabs(approx_val / val - 1.0);
        if (err <= rel_err)
            return 1;
    }
    return 0;
}


namespace LQMeas {
    DevOutSyncModeImplCycle::DevOutSyncModeImplCycle(LQMeas::Device *dev, LQMeas::DevOutSyncIfaceCycle *dev_cycle) :
        m_dev(dev), m_iface(dev_cycle), m_cycle_freq_rel_err(0.000001) {

    }

    Error DevOutSyncModeImplCycle::outSyncModeImplSigSet(QSharedPointer<OutRamSignalGenerator> ramSigGenerator) {
        Error err;


        err = outCalcCycleSigsParams(ramSigGenerator.data(), m_dev->devOutSync()->outSyncConfig()->outSyncGenFreq(),
                                     m_iface->outCycleGenMaxSize(ramSigGenerator.data()),
                                     &m_sig_size);
        if (err.isSuccess())
            m_generator = ramSigGenerator;
        return err;

    }

    Error DevOutSyncModeImplCycle::outSyncModeImplStartPrepare() {
        Error err = m_iface->privOutCycleLoadStart(m_generator.data(), m_sig_size);
        if (err.isSuccess()) {
            QVector<double> dac_data;
            QVector<unsigned> dig_data;

            err = m_generator->genNextData(m_sig_size, dac_data, dig_data);
            if (err.isSuccess()) {
                err = m_iface->privOutSyncSendData(dac_data.data(), dac_data.size(),
                                                   dig_data.data(), dig_data.size(), 0);
            }
            if (err.isSuccess())
                err = m_iface->privOutSyncFlushData(0);
            if (err.isSuccess())
                err = m_iface->privOutCycleLoadFinish();
        }

        if (err.isSuccess()) {
            LQMeasLog->detail(Device::tr("Output cycle generation setup done successfully"), m_dev);
        } else {
            LQMeasLog->error(Device::tr("Output cycle generation setup error"), err, m_dev);
        }

        return err;
    }

    Error DevOutSyncModeImplCycle::outSyncModeImplStart() {
        Error err = m_iface->privOutCycleGenStart();
        if (err.isSuccess()) {
            LQMeasLog->detail(Device::tr("Output cycle generation started successfully"), m_dev);
        } else {
            LQMeasLog->error(Device::tr("Output cycle generation start error"), err, m_dev);
        }
        return err;
    }

    Error DevOutSyncModeImplCycle::outSyncModeImplStopRequest(unsigned tout) {
        return m_iface->privOutCycleGenStopRequest(tout);
    }

    Error DevOutSyncModeImplCycle::outSyncModeImplStopWait() {
        Error err = m_iface->privOutCycleGenStop();
        if (err.isSuccess()) {
            LQMeasLog->detail(Device::tr("Output cycle generation stopped successfully"), m_dev);
        } else {
            LQMeasLog->error(Device::tr("Output cycle generation stop error"), err, m_dev);
        }
        return err;
    }

    Error DevOutSyncModeImplCycle::outSyncModeImplSigUpdate() {
        return outSyncModeImplStartPrepare();
    }

    Error DevOutSyncModeImplCycle::outCalcCycleSigsParams(OutRamSignalGenerator *ramSigGenerator, double out_freq, unsigned max_sig_size, size_t *res_size) {
        Error err = Error::Success();
        unsigned sig_size = 1;
        QList<QSharedPointer<OutSignal> > ram_sigs = ramSigGenerator->allSignals();

        Q_FOREACH (QSharedPointer<OutSignal> sig, ram_sigs) {
            double sig_freq = sig->signalFreq();
            long n, d;

            if (sig_freq <= 0) {
                /* если сигнал имеет явно заданное кол-во точек, а не
                   определяемое по частоте, то нужно установить именно это
                   кол-во отсчетов - как НОК. Если не хватает места - то сигнал
                   не устанавливаем */
                unsigned cur_size = lcm(sig_size, sig->minPoints());
                if (max_sig_size && (cur_size > max_sig_size)) {
                    err = StdErrors::OutSignalPointCount();
                    break;
                }
            } else {
                /* если сигнал задан частотой, то пытаемся
                   подобрать ближайшую к отношению текущей частоты и частоты
                   сигнала дробь */


                /* если задана силшком большая частота - обрезаем до макс. возможной */
                if (out_freq < sig->minPoints()*sig_freq)
                    sig_freq = out_freq/sig->minPoints();


                rat_approx(sig_freq/(out_freq/sig_size), &n, &d, max_sig_size/sig_size,
                           m_cycle_freq_rel_err);
                /* qDebug() << QString("sig_f = %0, sig_size = %1, n = %2, d = %3")
                            .arg(QString::number(sig_freq))
                            .arg(QString::number(sig_size))
                            .arg(QString::number(n))
                            .arg(QString::number(d)); */
                /* нулевой числитель => доступного буфера не достаточно даже для одного
                   периода */
                if (n==0) {
                    err = StdErrors::OutSignalPointCount();
                    break;
                }
                sig_size*=d;
            }
        }

        if (err.isSuccess()) {
            Q_FOREACH (QSharedPointer<OutSignal> sig, ram_sigs) {
                if (sig->signalFreq() > 0) {
                    if (!sig->periodStrictPoints()) {
                        unsigned periods = sig->signalFreq()*sig_size/out_freq + 0.5;
                        if (periods==0)
                            periods = 1;
                        sig->setRealParams(out_freq*periods/sig_size, out_freq);
                    } else {
                        unsigned req_size = (unsigned)(out_freq/sig->signalFreq()+0.5);
                        unsigned periods = (double)sig_size/req_size + 0.5;
                        if (periods==0)
                            periods = 1;
                        req_size = sig_size/periods;
                        sig->setRealParams(out_freq/req_size, out_freq);
                    }
				} else {
                    sig->setRealParams(OutSignal::unspecifiedFreq(), out_freq);
                }
            }
        }

        if (err.isSuccess())
            *res_size = sig_size;

        return err;
    }
}
