#ifndef LTR24TYPEINFO_H
#define LTR24TYPEINFO_H

#include "lqmeas/devs/DeviceTypeInfo.h"
#include "lqmeas/ifaces/in/DevAdcInfo.h"
#include <QList>

namespace LQMeas {
    class LTR24TypeInfo : public DeviceTypeInfo, public DevAdcInfo {
    public:
        static QString name() {return "LTR24";}
        static const LTR24TypeInfo *typeInfoMod1();
        static const LTR24TypeInfo *typeInfoMod2();
        static const LTR24TypeInfo *defaultInfo();

        const QList<const LTR24TypeInfo*> types();

        static const unsigned adc_channels_cnt = 4;

        const DevAdcInfo *adc() const {return static_cast<const DevAdcInfo *>(this);}

        QString deviceTypeName() const {return name();}
        QString devceModificationName() const;

        AdcType adcType() const {return AdcTypeParallel;}
        unsigned adcChannelsCnt() const {return adc_channels_cnt;}
        double adcFreqMax() const;

        bool adcIsChFreqConfigurable() const {return false;}

        unsigned adcRangesCnt(int unit_mode_num = 0) const;
        double adcRangeMaxVal(int range, int unit_mode_num = 0) const;
        double adcRangeMinVal(int range, int unit_mode_num = 0) const;

        static double typeAdcFreqVal(int idx);
        static unsigned typeAdcFreqsCnt();

        bool supportICP() const {return m_supportICP;}

    private:
        LTR24TypeInfo(bool supportICP) : m_supportICP(supportICP) {
        }

        bool m_supportICP;
    };
}


#endif // LTR24TYPEINFO_H
