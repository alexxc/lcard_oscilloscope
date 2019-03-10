#include "MultimeterProcessor.h"
#include "lqmeas/log/Log.h"
#include "lmath.h"



namespace LQMeas {
    void MultimeterProcessor::processData(DevAdc * adc, QVector<double> vals,
                                      MultimeterThread::MultimeterData data) {
        int enabled_ch_cnt = adc->adcEnabledChCnt();
        int ch_size = vals.size()/enabled_ch_cnt;
        int fft_size = ch_size/2 + 1;

        QScopedArrayPointer<double> ch_vals(new double[ch_size]);
    #ifndef LQMEAS_SERVICE_MULTIMETER_DC_ONLY
        QScopedArrayPointer<double> fft(new double[fft_size]);
        QScopedArrayPointer<double> scaled(new double[ch_size]);

        double df;
        double dt = 1./adc->adcFreq();
    #endif

        for (int ch=0; ch < enabled_ch_cnt; ch++) {
    #ifndef LQMEAS_SERVICE_MULTIMETER_DC_ONLY
            t_lmath_win_params win_par;
    #endif
            unsigned ch_num = adc->adcChNum(ch);


			if ((data.ch.at(ch_num).status == DevAdc::ChStatusOk) ||
				(data.ch.at(ch_num).status == DevAdc::ChStatusOverload)) {
                for (int i=0; i < ch_size; i++) {
					ch_vals[i] = vals.at(enabled_ch_cnt*i + ch);
                }

                int ch_err=0;
    #ifdef LQMEAS_SERVICE_MULTIMETER_DC_ONLY
                if (ch_size==1) {
                    data.ch[ch_num].DC = ch_vals[0];
                } else {
                    ch_err = lmath_acdc_estimation(ch_vals.data(), ch_size,
												  &data.ch.at(ch_num).DC, 0);
                }
    #else
				ch_err = lmath_acdc_estimation(ch_vals.data(), ch_size, &data.ch.at(ch_num).DC,
											   &data.ch.at(ch_num).AC);
                if (!ch_err) {
                    ch_err = lmath_scaled_window(ch_vals.data(), ch_size,
                                                 LMATH_WINTYPE_BH_4TERM,
                                                 0, &win_par, scaled.data());
                }
                if (!ch_err) {
                    ch_err = lmath_amp_pha_spectrum(scaled.data(), ch_size,
                                                    dt, 0, fft.data(), 0, &df);
                }
                if (!ch_err) {
                    ch_err = lmath_find_peak_freq_spectrum(fft.data(), fft_size,
                                                           df, -1, win_par,
														   &data.ch.at(ch_num).freq, 0);
                }
                if (!ch_err) {
                    if (lmath_calc_spectrum_params(fft.data(), fft_size,
												   df, data.ch.at(ch_num).freq,
												   6, win_par, &data.ch.at(ch_num).SNR,
												   0, &data.ch.at(ch_num).SINAD, 0, 0) != 0) {
                        data.ch[ch_num].SNR = 0;
                        data.ch[ch_num].SINAD = 0;
                    }
                }
    #endif

                if (ch_err) {
                    data.ch[ch_num].status = DevAdc::ChStatusError;
                    LQMeasLog->error(tr("Cannot process data block"), StdErrors::DataProcess());
                }
            }
        }

        emit dataProcessed(data);

    }
}
