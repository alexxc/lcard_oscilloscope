#ifndef SPECTRUMPLUGIN_H
#define SPECTRUMPLUGIN_H

#include "DataBlockProcessPlugin.h"

namespace LQMeasStudio {
    /* Плагин для рассчета спектра сигнала, а также спектральных параметров */
    class SpectrumPlugin : public DataBlockProcessPlugin {
        Q_OBJECT
    public:
		QString typePluginName() const {return "SpectrumPlugin";}
        static QString param_spectrum() {return "Spectrum";}
        static QString param_df()       {return "df";}
        static QString param_peakFreq() {return "PeakFreq";}
        static QString param_peakAmp()  {return "PeakAmp";}
        static QString param_snr()      {return "SNR";}
        static QString param_thd()      {return "THD";}
        static QString param_sinad()    {return "SINAD";}
        static QString param_sfdr()     {return "SFDR";}
        static QString param_enob()     {return "ENOB";}

		explicit SpectrumPlugin(QObject *parent = nullptr);

        virtual void blockProcess(QSharedPointer<DataBlock> block);
        virtual void blockShowResult(QSharedPointer<DataBlock> block);

    protected:
        void procProtSetEnabled(bool enabled);
    private:
        int m_win_type;

        int m_param_id_peak_freq;
        int m_param_id_peak_amp;
        int m_param_id_snr;
        int m_param_id_thd;
        int m_param_id_sinad;
        int m_param_id_sfdr;
        int m_param_id_enob;
    };
}

#endif // SPECTRUMPLUGIN_H
