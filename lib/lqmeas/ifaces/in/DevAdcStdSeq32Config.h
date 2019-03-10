#ifndef DEVADCSTDSEQ32CONFIG_H
#define DEVADCSTDSEQ32CONFIG_H

#include "DevAdcConfig.h"
#include "lqmeas/Error.h"
class QSettings;

namespace LQMeas {
    /* Вспомогательный класс, реализующих установку стандартных параметров
     * для АЦП с коммутацией каналов на 32 канала с общей землей/16 дифференциальных
     * каналов с настраиваемым диапазоном для каждого канала */
    class DevAdcStdSeq32Config : public DevAdcConfig {
    public:
        static const unsigned adc_channels_cnt = 32;
        static const unsigned adc_diff_channels_cnt = 16;

        enum AdcChMode {
            AdcChModeComm  = 0,
            AdcChModeDiff = 1,
            AdcChModeZero = 2
        };


        DevAdcStdSeq32Config(const DevAdcInfo *info);
        DevAdcStdSeq32Config(const DevAdcStdSeq32Config *cfg);


        double adcFreq() const {return results.AdcFreq;}
        double adcChFreq() const {return results.AdcChFreq;}
		bool adcChEnabled(unsigned int ch) const { return adc_params.Ch[ch].Enabled; }

		int adcChRangeNum(unsigned int ch) const {return adc_params.Ch[ch].Range;}
		AdcChMode adcChMode(unsigned int ch) const {return adc_params.Ch[ch].Mode;}


        bool adcUseMaxChFreq() const {return adc_params.UseMaxChFreq;}

		void adcSetChEnabled(unsigned int ch, bool en) {adc_params.Ch[ch].Enabled = en;}
		void adcSetChMode(unsigned int ch, AdcChMode mode) {adc_params.Ch[ch].Mode = mode;}
		void adcSetChRangeNum(unsigned int ch, int range_num) {adc_params.Ch[ch].Range = range_num;}
        void adcSetFreq(double freq) {adc_params.FreqAdc = freq;}
        void adcSetChFreq(double ch_freq) {adc_params.FreqCh = ch_freq;}
        void adcSetUseMaxChFreq(bool en) {adc_params.UseMaxChFreq = en;}

    protected:
        Error protAdcLoadConfig(QSettings &set);
        Error protAdcSaveConfig(QSettings &set) const;
        Error protAdcUpdateConfig();
        virtual void adcAdjustFreq(double *adcFreq, double *AdcChFreq) const = 0;
    private:
        struct {
            struct {
                bool Enabled;
                AdcChMode Mode;
                int Range;
            } Ch[adc_channels_cnt];
            double FreqAdc;
            double FreqCh;
            bool   UseMaxChFreq;
        } adc_params;

        struct {
            double AdcFreq;
            double AdcChFreq;
        } results;
    };
}


#endif // DEVADCSTDSEQ32CONFIG_H
