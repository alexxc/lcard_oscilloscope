#include "DevAdcStdSeq32Config.h"
#include <QSettings>

namespace LQMeas {
    DevAdcStdSeq32Config::DevAdcStdSeq32Config(const DevAdcInfo *info) : DevAdcConfig(info) {
        memset(&adc_params, 0, sizeof(adc_params));
        adc_params.FreqAdc = info->adcFreqMax();
        adc_params.UseMaxChFreq = true;
        adc_params.Ch[0].Enabled = true;
        results.AdcFreq = results.AdcChFreq = adc_params.FreqAdc;
    }

    DevAdcStdSeq32Config::DevAdcStdSeq32Config(const DevAdcStdSeq32Config *cfg) : DevAdcConfig(cfg) {
        memcpy(&adc_params, &cfg->adc_params, sizeof(adc_params));
        memcpy(&results, &cfg->results, sizeof(results));
    }

    Error DevAdcStdSeq32Config::protAdcLoadConfig(QSettings &set) {
        bool ok;
        double dval;
        int intval;
        set.beginGroup("Adc");

        int ch_cnt = set.beginReadArray("Channels");
        if (ch_cnt == 0) {
            ch_cnt = 1;
            adc_params.Ch[0].Enabled = true;
            adc_params.Ch[0].Mode = AdcChModeComm;
            adc_params.Ch[0].Range = 0;
        } else {
            for (int ch=0; (ch < ch_cnt) && (ch < adc_channels_cnt); ch++) {
                set.setArrayIndex(ch);
                adc_params.Ch[ch].Enabled = set.value("Enabled").toBool();

                intval = set.value("Range", 0).toInt(&ok);
                if (ok && (intval >= 0) && (intval <= (int)adcInfo()->adcRangesCnt())) {
                    adc_params.Ch[ch].Range = intval;
                } else {
                    adc_params.Ch[ch].Range = 0;
                }

                intval = set.value("Mode", AdcChModeDiff).toInt(&ok);
                if (ok && ((intval == AdcChModeComm) || (intval == AdcChModeZero) ||
                           ((intval == AdcChModeDiff) && (ch < adc_diff_channels_cnt)))) {
                    adc_params.Ch[ch].Mode    = (AdcChMode)intval;
                } else {
                    adc_params.Ch[ch].Mode = AdcChModeComm;
                }
            }
        }

        for (int ch=ch_cnt; ch < adc_channels_cnt; ch++) {
            adc_params.Ch[ch].Enabled = false;
            adc_params.Ch[ch].Range = 0;
            adc_params.Ch[ch].Mode = AdcChModeComm;
        }
        set.endArray();

        double max_freq = adcInfo()->adcFreqMax();
        dval = set.value("FreqAdc", max_freq).toDouble(&ok);
        if (ok && (dval > 0) && (dval <= max_freq)) {
            adc_params.FreqAdc = dval;
        } else {
            adc_params.FreqAdc = max_freq;
        }

        dval = set.value("FreqCh").toDouble(&ok);
        if (ok && (dval >= 0) && (dval <= adc_params.FreqAdc)) {
            adc_params.FreqCh = dval;
        } else {
            adc_params.FreqCh = 0;
        }

        adc_params.UseMaxChFreq = set.value("UseMaxFreqCh", true).toBool();


        set.endGroup();

        return Error::Success();
    }

    Error DevAdcStdSeq32Config::protAdcSaveConfig(QSettings &set) const {
        set.beginGroup("Adc");
        set.beginWriteArray("Channels", adc_channels_cnt);

        for (int ch=0; ch < adc_channels_cnt; ch++) {
            set.setArrayIndex(ch);
            set.setValue("Enabled", adc_params.Ch[ch].Enabled);
            set.setValue("Range", adc_params.Ch[ch].Range);
            set.setValue("Mode", adc_params.Ch[ch].Mode);
        }
        set.endArray();

        set.setValue("FreqAdc", adc_params.FreqAdc);
        set.setValue("FreqCh",  adc_params.FreqCh);
        set.setValue("UseMaxFreqCh", adc_params.UseMaxChFreq);

        set.endGroup();

        return Error::Success();
    }

    Error DevAdcStdSeq32Config::protAdcUpdateConfig() {

        if ((adc_params.FreqAdc <= 0) || (adc_params.FreqAdc > adcInfo()->adcFreqMax())) {
            results.AdcFreq = adcInfo()->adcFreqMax();
        } else {
            results.AdcFreq = adc_params.FreqAdc;
        }

        if (adc_params.UseMaxChFreq || (adc_params.FreqCh <= 0) ||
                (adc_params.FreqCh > results.AdcFreq/adcEnabledChCnt())) {
            results.AdcChFreq = adcInfo()->adcFreqMax();
        } else {
            results.AdcChFreq = adc_params.FreqCh;
        }

        adcAdjustFreq(&results.AdcFreq, &results.AdcChFreq);
        return Error::Success();
    }
}
