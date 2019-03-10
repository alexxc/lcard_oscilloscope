#ifndef LTR24CONFIG_H
#define LTR24CONFIG_H

#include "lqmeas/devs/DeviceConfig.h"
#include "lqmeas/ifaces/in/DevAdcConfig.h"
#include "LTR24TypeInfo.h"

namespace LQMeas {
    class LTR24Config : public DeviceConfig, public DevAdcConfig {
    public:

        static QString typeConfigName();
        QString configDevTypeName() const {return typeConfigName();}
        const DevAdcConfig *adcConfig() const {
            return static_cast<const DevAdcConfig *>(this);
        }

        DeviceConfig *clone() const {
            return new LTR24Config(this);
        }

        LTR24Config();
        LTR24Config(const LTR24Config *cfg);

        enum ChMode {
            ChModeDiff,
            ChModeICP
        };

        enum ChAcMode {
            ChAcDc,
            ChAcOnly
        };

        enum AdcDataFormat {
            AdcDataFormat20,
            AdcDataFormat24
        };

        enum ISrcValue {
            ISrcValue_2_86mA,
            ISrcValue_10mA,
        };

        double adcFreq() const;
        bool adcChEnabled(int ch) const {return params.Ch[ch].enabled;}
        int adcChRangeNum(int ch) const { return params.Ch[ch].range; }
        ChMode adcChMode(int ch) const {return params.Ch[ch].mode;}
        ChAcMode adcChAcMode(int ch) const {return params.Ch[ch].ac_mode;}
        int adcFreqCode() const {return params.freq_code;}
        AdcDataFormat adcDataFormat() const {return params.data_fmt;}
        ISrcValue  adcISrcValue() const {return params.i_src_value;}
        bool adcTestModesEnabled() const {return params.test_modes_en;}

        void adcSetChEnabled(int ch, bool en) {params.Ch[ch].enabled = en;}
        void adcSetChRangeNum(int ch, int range_num) {params.Ch[ch].range = range_num;}
        void adcSetChMode(int ch, ChMode mode) {params.Ch[ch].mode = mode;}
        void adcSetChAcMode(int ch, ChAcMode mode) {params.Ch[ch].ac_mode = mode;}
        void adcSetFreqCode(int freq_code) {params.freq_code = freq_code;}
        void adcSetDataFormat(AdcDataFormat format) {params.data_fmt = format;}
        void adcSetISrcValue(ISrcValue val) {params.i_src_value = val;}
        void adcSetTestModesEnabled(bool en) {params.test_modes_en = en;}

        /* возвращает максимально количество каналов, которое можно разрешить
         * для текущих настроек модуля */
        int adcMaxEnabledChCnt();

    protected:
        Error protLoadConfig(QSettings &set);
        Error protSaveConfig(QSettings &set) const;
        Error protUpdateConfig() {return Error::Success();}
    private:
        struct {
            int           freq_code;
            AdcDataFormat data_fmt;
            ISrcValue     i_src_value;
            bool          test_modes_en;
            struct {
                bool      enabled;
                int       range;
                ChMode    mode;
                ChAcMode  ac_mode;
            } Ch[LTR24TypeInfo::adc_channels_cnt];
        } params;
    };
}

#endif // LTR24CONFIG_H
