#include "x502TypeInfo.h"
#include "x502api.h"

static double f_ranges[] = {10.,5.,2.,1.,0.5, 0.2};

namespace LQMeas {

    const x502TypeInfo *x502TypeInfo::defaultInfo() {
        static const x502TypeInfo info(X502_DEVFLAGS_BF_PRESENT | X502_DEVFLAGS_DAC_PRESENT | X502_DEVFLAGS_GAL_PRESENT);
        return &info;
    }

    unsigned x502TypeInfo::adcRangesCnt(int unit_mode_num) const {
        return sizeof(f_ranges)/sizeof(f_ranges[0]);
    }

    double x502TypeInfo::adcRangeMaxVal(int range, int unit_mode_num) const {
        return f_ranges[range];
    }

    double x502TypeInfo::adcRangeMinVal(int range, int unit_mode_num) const {
        return -f_ranges[range];
    }

    double x502TypeInfo::outSyncGenFreqMax() const {
        return X502_REF_FREQ_2000KHZ/X502_OUT_FREQ_DIV_MIN;
    }

    int x502TypeInfo::outDacChannelsCnt() const {
        return m_dac_present ? X502_DAC_CH_CNT : 0;
    }

    int x502TypeInfo::outDigChannelsCnt() const {
        return X502_DOUT_LINES_CNT;
    }

    double x502TypeInfo::outDacMaxVal(int ch) const {
        return X502_DAC_RANGE;
    }

    double x502TypeInfo::outDacMinVal(int ch) const {
        return -X502_DAC_RANGE;
    }

    x502TypeInfo::x502TypeInfo(unsigned devflags) {
        m_gal_present = devflags & X502_DEVFLAGS_GAL_PRESENT ? true : false;
        m_dac_present = devflags & X502_DEVFLAGS_DAC_PRESENT ? true : false;
        m_bf_present  = devflags & X502_DEVFLAGS_BF_PRESENT ? true : false;
        m_eth_present = devflags & X502_DEVFLAGS_IFACE_SUPPORT_ETH ? true : false;
        m_industrial  = devflags & X502_DEVFLAGS_INDUSTRIAL ? true : false;
    }
}
