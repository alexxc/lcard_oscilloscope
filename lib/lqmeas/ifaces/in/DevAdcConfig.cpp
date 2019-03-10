#include "DevAdcConfig.h"

namespace LQMeas {

    DevAdcConfig::DevAdcConfig(const DevAdcInfo *info) : m_adc_info(info) {

    }

    DevAdcConfig::DevAdcConfig(const DevAdcConfig *cfg) {
        m_adc_info = cfg->adcInfo();
        m_ch_indexes = cfg->m_ch_indexes;
    }

    double DevAdcConfig::adcChFreq() const {
        double ch_freq  = adcFreq();
        if (m_adc_info->adcType() == DevAdcInfo::AdcTypeSequential) {
            unsigned enChCnt = adcEnabledChCnt();
            if (enChCnt) {
                ch_freq/=enChCnt;
            }
        }
        return ch_freq;
    }

	double DevAdcConfig::adcChTimeShift(unsigned int ch) const {
        return m_adc_info->adcType() == DevAdcInfo::AdcTypeSequential ? (double)adcChIdx(ch)/adcFreq() : 0;
    }

	int DevAdcConfig::adcChNum(unsigned int idx) const {
		return idx < m_ch_indexes.size() ? m_ch_indexes.at(idx) : -1;
    }

	int DevAdcConfig::adcChIdx(unsigned int ch) const {
        return m_ch_indexes.indexOf(ch);
    }

    void DevAdcConfig::adcUpdateConfig() const {
        m_ch_indexes.clear();
        for (unsigned ch =0; ch < m_adc_info->adcChannelsCnt(); ch++) {
            if (adcChEnabled(ch)) {
                m_ch_indexes.append(ch);
            }
        }
    }

    void DevAdcConfig::adcUpdateDevInfo(const DevAdcInfo *info)  {
        m_adc_info = info;
        adcUpdateConfig();
    }

}
