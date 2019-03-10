#include "LTR24Config.h"
#include "LTR24.h"
#include "ltr/include/ltr24api.h"
#include <QSettings>

namespace LQMeas {
    QString LTR24Config::typeConfigName() {
        return LTR24TypeInfo::name();
    }

    LTR24Config::LTR24Config() : DevAdcConfig(LTR24TypeInfo::defaultInfo()) {
        memset(&params, 0, sizeof(params));
        for (int ch=0; ch < LTR24TypeInfo::adc_channels_cnt; ch++) {
            params.Ch[ch].enabled = ch==0;
            params.Ch[ch].range = LTR24_RANGE_10;
            params.Ch[ch].mode = ChModeDiff;
            params.Ch[ch].ac_mode = ChAcDc;
        }
    }

    LTR24Config::LTR24Config(const LTR24Config *cfg) : DevAdcConfig(cfg) {
        memcpy(&params, &cfg->params, sizeof (params));
    }

    double LTR24Config::adcFreq() const {
        return LTR24TypeInfo::typeAdcFreqVal(params.freq_code);
    }

    int LTR24Config::adcMaxEnabledChCnt() {
        int max_ch_cnt = 4;
        if (params.data_fmt == AdcDataFormat24) {
            if (params.freq_code == LTR24_FREQ_117K) {
                max_ch_cnt = 2;
            } else if (params.freq_code == LTR24_FREQ_78K) {
                max_ch_cnt = 3;
            }
        }
        return max_ch_cnt;
    }

    Error LTR24Config::protLoadConfig(QSettings &set) {
        int intval;
        bool ok;

        int ch_cnt = set.beginReadArray("Channels");
        if (ch_cnt == 0) {
            ch_cnt = 1;
            params.Ch[0].enabled = true;
            params.Ch[0].range = LTR24_RANGE_10;
            params.Ch[0].mode = ChModeDiff;
            params.Ch[0].ac_mode = ChAcDc;
        } else {
            for (int ch=0; (ch < ch_cnt) && (ch < LTR24TypeInfo::adc_channels_cnt); ch++) {
                set.setArrayIndex(ch);
                params.Ch[ch].enabled = set.value("Enabled").toBool();

                intval = set.value("Range", LTR24_RANGE_10).toInt(&ok);
                if (ok && (intval >= 0) && (intval < (int)adcInfo()->adcRangesCnt())) {
                    params.Ch[ch].range   = intval;
                }

                intval = set.value("Mode", ChModeDiff).toInt(&ok);
                if ((intval == ChModeDiff) || (intval == ChModeICP)) {
                    params.Ch[ch].mode    = (ChMode)intval;
                }

                intval = set.value("AcMode", ChAcDc).toInt(&ok);
                if ((intval == ChAcDc) || (intval == ChAcOnly)) {
                    params.Ch[ch].ac_mode = (ChAcMode)intval;
                }
            }
        }

        for (int ch=ch_cnt; ch < LTR24TypeInfo::adc_channels_cnt; ch++) {
            params.Ch[ch].enabled = false;
            params.Ch[ch].range = LTR24_RANGE_10;
            params.Ch[ch].mode = ChModeDiff;
            params.Ch[ch].ac_mode = ChAcDc;
        }
        set.endArray();



        intval = set.value("DataFmt", AdcDataFormat24).toInt(&ok);
        if (ok && ((intval==AdcDataFormat20) || (intval==AdcDataFormat24))) {
            params.data_fmt = (AdcDataFormat)intval;
        } else {
            reportLoadWarning(LTR24::tr("Invalid data format value"));
        }

        intval = set.value("AdcFreqCode", LTR24_FREQ_117K).toInt(&ok);
        if (ok && ((intval >= LTR24_FREQ_117K) && (intval <= LTR24_FREQ_610))) {
            params.freq_code = intval;
        } else {
            reportLoadWarning(LTR24::tr("Invalid frequency code value"));
        }

        intval = set.value("ISrcValue", ISrcValue_2_86mA).toInt(&ok);
        if (ok && ((intval==ISrcValue_2_86mA) || (intval==ISrcValue_10mA))) {
            params.i_src_value = (ISrcValue)intval;
        } else {
            reportLoadWarning(LTR24::tr("Invalid current source value"));
        }

        params.test_modes_en = set.value("TestMode").toBool();
        return Error::Success();
    }

    Error LTR24Config::protSaveConfig(QSettings &set) const {
        set.beginWriteArray("Channels", LTR24TypeInfo::adc_channels_cnt);
        for (int ch=0; ch < LTR24TypeInfo::adc_channels_cnt; ch++) {
            set.setArrayIndex(ch);
            set.setValue("Enabled", params.Ch[ch].enabled);
            set.setValue("Range", params.Ch[ch].range);
            set.setValue("Mode", params.Ch[ch].mode);
            set.setValue("AcMode", params.Ch[ch].ac_mode);
        }
        set.endArray();

        set.setValue("DataFmt", params.data_fmt);
        set.setValue("AdcFreqCode", params.freq_code);
        set.setValue("ISrcValue", params.i_src_value);
        set.setValue("TestMode", params.test_modes_en);
        return Error::Success();
    }
}
