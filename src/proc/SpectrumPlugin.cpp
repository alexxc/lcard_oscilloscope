#include "SpectrumPlugin.h"
#include "LQMeasStudio.h"
#include "ChannelSignalParamPanel.h"
#include "lmath.h"
#include <math.h>
#include <QMetaType>

Q_DECLARE_METATYPE (QSharedPointer<QVector<double> >)

namespace LQMeasStudio {
    SpectrumPlugin::SpectrumPlugin(QObject *parent) :
        DataBlockProcessPlugin(typePluginName(), true, parent),
        m_win_type(LMATH_WINTYPE_BH_4TERM) {

        qRegisterMetaType<QSharedPointer<QVector<double> > >("QSharedPointer<QVector<double> >");

        m_param_id_peak_freq    = LQMeasStudioEnv->chSignalParamsPanel()->addParam("Peak Freq");
        m_param_id_peak_amp     = LQMeasStudioEnv->chSignalParamsPanel()->addParam("Peak Amp");
        m_param_id_snr          = LQMeasStudioEnv->chSignalParamsPanel()->addParam("SNR");
        m_param_id_thd          = LQMeasStudioEnv->chSignalParamsPanel()->addParam("THD");
        m_param_id_sinad        = LQMeasStudioEnv->chSignalParamsPanel()->addParam("SINAD");
        m_param_id_sfdr         = LQMeasStudioEnv->chSignalParamsPanel()->addParam("SFDR");
        m_param_id_enob         = LQMeasStudioEnv->chSignalParamsPanel()->addParam("ENOB");
    }

    void SpectrumPlugin::blockProcess(QSharedPointer<DataBlock> block) {
        t_lmath_win_params win_par;
        unsigned size = block->values().size();
        QScopedArrayPointer<double> scaled_vals(new double[size]);
        int err = lmath_scaled_window(block->values().data(), size, m_win_type, 0,
                                  &win_par, scaled_vals.data());
        if (!err) {
            unsigned fft_size = size/2+1;
            QSharedPointer<QVector<double> > fft =
                    QSharedPointer<QVector<double> >(new QVector<double>(fft_size));
            double df;
            double peak_freq, peak_amp;

            err = lmath_amp_pha_spectrum(scaled_vals.data(), size,
                                         block->dt(), 0, fft->data(), NULL,
                                         &df);
            if (!err) {
                block->setParameter(param_spectrum(), QVariant::fromValue(fft));
                block->setParameter(param_df(), df);
            }

            if (!err) {
                err = lmath_find_peak_freq_spectrum(fft->data(), fft_size,
                                                    df, -1, win_par,
                                                    &peak_freq, &peak_amp);
                if (!err) {
                    peak_amp = sqrt(peak_amp);

                    block->setParameter(param_peakFreq(), peak_freq);
                    block->setParameter(param_peakAmp(), peak_amp);
                }
            }

            if (!err) {
                double snr, thd, sinad, sfdr, enob;
                err = lmath_calc_spectrum_params(fft->data(), fft_size, df, peak_freq,
                                                 6, win_par, &snr, &thd, &sinad,
                                                 &sfdr, &enob);
                if (!err) {
                    block->setParameter(param_snr(), snr);
                    block->setParameter(param_thd(), thd);
                    block->setParameter(param_sinad(), sinad);
                    block->setParameter(param_sfdr(), sfdr);
                    block->setParameter(param_enob(), enob);
                }
            }
        }
    }

    void SpectrumPlugin::blockShowResult(QSharedPointer<DataBlock> block) {
        if (block->hasParameter(param_peakFreq())) {
            LQMeasStudioEnv->chSignalParamsPanel()->setParam(block->channel(), m_param_id_peak_freq,
                                                             block->parameter(param_peakFreq()).toDouble(), 'f', 3);
        }
        if (block->hasParameter(param_peakAmp())) {
            LQMeasStudioEnv->chSignalParamsPanel()->setParam(block->channel(), m_param_id_peak_amp,
                                                             block->parameter(param_peakAmp()).toDouble(), 'f', 6);
        }
        if (block->hasParameter(param_snr())) {
            LQMeasStudioEnv->chSignalParamsPanel()->setParam(block->channel(), m_param_id_snr,
                                                             block->parameter(param_snr()).toDouble(), 'f', 2);
        }
        if (block->hasParameter(param_thd())) {
            LQMeasStudioEnv->chSignalParamsPanel()->setParam(block->channel(), m_param_id_thd,
                                                             block->parameter(param_thd()).toDouble(), 'f', 2);
        }
        if (block->hasParameter(param_sinad())) {
           LQMeasStudioEnv->chSignalParamsPanel()->setParam(block->channel(), m_param_id_sinad,
                                                            block->parameter(param_sinad()).toDouble(), 'f', 2);
        }
        if (block->hasParameter(param_sfdr())) {
           LQMeasStudioEnv->chSignalParamsPanel()->setParam(block->channel(), m_param_id_sfdr,
                                                            block->parameter(param_sfdr()).toDouble(), 'f', 2);
        }
        if (block->hasParameter(param_enob())) {
           LQMeasStudioEnv->chSignalParamsPanel()->setParam(block->channel(), m_param_id_enob,
                                                            block->parameter(param_enob()).toDouble(), 'f', 2);
        }
    }

    void SpectrumPlugin::procProtSetEnabled(bool enabled) {
        LQMeasStudioEnv->chSignalParamsPanel()->enableParam(m_param_id_peak_freq, enabled);
        LQMeasStudioEnv->chSignalParamsPanel()->enableParam(m_param_id_peak_amp, enabled);
        LQMeasStudioEnv->chSignalParamsPanel()->enableParam(m_param_id_snr, enabled);
        LQMeasStudioEnv->chSignalParamsPanel()->enableParam(m_param_id_thd, enabled);
        LQMeasStudioEnv->chSignalParamsPanel()->enableParam(m_param_id_sinad, enabled);
        LQMeasStudioEnv->chSignalParamsPanel()->enableParam(m_param_id_sfdr, enabled);
        LQMeasStudioEnv->chSignalParamsPanel()->enableParam(m_param_id_enob, enabled);
    }
}
