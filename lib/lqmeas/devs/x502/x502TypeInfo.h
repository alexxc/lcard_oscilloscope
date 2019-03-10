#ifndef LQMEAS_X502TYPEINFO_H
#define LQMEAS_X502TYPEINFO_H

#include "lqmeas/devs/DeviceTypeInfo.h"
#include "lqmeas/ifaces/in/DevAdcInfo.h"
#include "lqmeas/ifaces/out/DevOutInfo.h"

namespace LQMeas {
    class x502TypeInfo : public DeviceTypeInfo, public DevAdcInfo, public DevOutInfo {
public:
        QString deviceTypeName() const {return "x502";}
        static const x502TypeInfo *defaultInfo();

        bool galPresent() const {return m_gal_present;}
        bool dacPresent() const {return m_dac_present;}
        bool bfPresent() const {return m_bf_present;}
        bool ethPresent() const {return m_eth_present;}
        bool industrial() const {return m_industrial;}


        /* ----------------- DevAdcInfo -------------------------------------*/
        const DevAdcInfo *adc() const {return static_cast<const DevAdcInfo *>(this);}

        static const unsigned adc_channels_cnt = 32;
        AdcType adcType() const {return AdcTypeSequential;}
		unsigned adcChannelsCnt() const {return adc_channels_cnt;}

        double adcFreqMax() const {return 2000000;}
        bool adcIsChFreqConfigurable() const {return true;}

        unsigned adcRangesCnt(int unit_mode_num = 0) const;
        double adcRangeMaxVal(int range, int unit_mode_num = 0) const;
        double adcRangeMinVal(int range, int unit_mode_num = 0) const;

        /* ----------------- DevOutInfo -------------------------------------*/
        const DevOutInfo *out() const {return static_cast<const DevOutInfo *>(this);}

        bool outDacSyncSupport() const {return m_dac_present;}
        bool outDigSyncSupport() const {return true;}

        bool outDacAsyncSupport() const {return m_dac_present;}
        bool outDigAsyncSupport() const {return true;}

        bool outDacSyncModeCfgPerCh() const {return true;}

        bool outSyncRamModeIsConfigurable() const {return true;}
        bool outSyncChGenModeIsConfigurable() const {return false;}


        virtual double outSyncGenFreqMax() const;
        virtual bool outSyncPresetSupport() const {return true;}

        int outDacChannelsCnt() const;
        int outDigChannelsCnt() const;


        double outDacMaxVal(int ch) const;
        double outDacMinVal(int ch) const;

    protected:
        x502TypeInfo(unsigned flags);
    private:
        bool m_gal_present;
        bool m_dac_present;
        bool m_bf_present;
        bool m_eth_present;
        bool m_industrial;
    };
}

#endif // LQMEAS_X502TYPEINFO_H
