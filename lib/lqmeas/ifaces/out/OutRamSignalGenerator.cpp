#include "OutRamSignalGenerator.h"

namespace LQMeas {
    OutRamSignalGenerator::OutRamSignalGenerator(QList<QSharedPointer<DacSignal> > dac_signals, QList<int> dac_chs,
                                                      QList<QSharedPointer<DigSignal> > dig_signals, QList<int> dig_chs) {
        m_dac_signals = dac_signals;
        m_dig_signals = dig_signals;
        m_dig_ch_nums = dig_chs;
        m_dac_ch_nums = dac_chs;

        Q_FOREACH(const QSharedPointer<DacSignal> dac_sig, dac_signals) {
            m_all_signals.append(dac_sig);
        }
        Q_FOREACH(const QSharedPointer<DigSignal> dig_sig, dig_signals) {
            m_all_signals.append(dig_sig);
        }
    }

    void OutRamSignalGenerator::genSignalsInit() {
        Q_FOREACH(QSharedPointer <DacSignal> sig, m_dac_signals) {
            sig->signalGenInit();
        }
        Q_FOREACH(QSharedPointer <DigSignal> sig, m_dig_signals) {
            sig->signalGenInit();
        }
    }

    Error OutRamSignalGenerator::genNextData(size_t points, QVector<double> &dac_data, QVector<unsigned> &dig_data) {
        int dac_ch_cnt = dacSignalsCount();
        int dig_ch_cnt = digSignalsCount();

        if (dac_ch_cnt > 0) {
            dac_data.resize(points*dac_ch_cnt);
            for (int ch_idx = 0; ch_idx < dac_ch_cnt; ch_idx++) {
                QSharedPointer<DacSignal> sig = m_dac_signals[ch_idx];
                for (unsigned pt = 0; pt < points; pt++) {
                    dac_data[pt * dac_ch_cnt + ch_idx] = sig->signalGenNextPoint();
                }
            }
        } else {
            dac_data.clear();
        }

        if (dig_ch_cnt > 0) {
            dig_data.resize(points);
            memset(dig_data.data(), 0, dig_data.size()*sizeof(dig_data[0]));
            for (int ch_idx = 0; ch_idx < dig_ch_cnt; ch_idx++) {
                QSharedPointer<DigSignal> sig = m_dig_signals[ch_idx];

                for (unsigned pt = 0; pt < points; pt++) {
                    if (sig->signalGenNextPoint()) {
                        dig_data[pt] |= (1 << m_dig_ch_nums[ch_idx]);
                    }
                }
            }
        } else {
            dig_data.clear();
        }

        return Error::Success();
    }

}
