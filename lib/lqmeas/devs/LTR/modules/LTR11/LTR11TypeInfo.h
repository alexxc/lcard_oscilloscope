#ifndef LTR11TYPEINFO_H
#define LTR11TYPEINFO_H

#include "lqmeas/devs/DeviceTypeInfo.h"
#include "lqmeas/ifaces/in/DevAdcInfo.h"

namespace LQMeas {
    class LTR11TypeInfo : public DeviceTypeInfo, public DevAdcInfo {
    public:
        static QString name() {return "LTR11";}
        static const LTR11TypeInfo *defaultInfo();

        const DevAdcInfo *adc() const {return static_cast<const DevAdcInfo *>(this);}

        QString deviceTypeName() const {return name();}

        AdcType adcType() const {return AdcTypeSequential;}
        unsigned adcChannelsCnt() const {return 32;}
        double adcFreqMax() const {return 400000;}

        bool adcIsChFreqConfigurable() const {return false;}

        unsigned adcRangesCnt(int unit_mode_num = 0) const;
        double adcRangeMaxVal(int range, int unit_mode_num = 0) const;
        double adcRangeMinVal(int range, int unit_mode_num = 0) const;
    };
}

#endif // LTR11TYPEINFO_H
