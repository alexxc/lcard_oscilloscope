#include "LTR24TypeInfo.h"


static double f_ranges[] = {2., 10.};

static const double f_adc_freqs[] = {
        117.1875e3,  78.125e3,   58.59375e3,   39.0625e3,
        29.296875e3, 19.53125e3, 14.6484375e3, 9.765625e3,
        7.32421875e3, 4.8828125e3, 3.662109375e3, 2.44140625e3,
        1.8310546875e3, 1.220703125e3, 0.91552734375e3, 0.6103515625e3
};

namespace LQMeas {
    class LTR24TypeInfoMod1 : public LTR24TypeInfo {
    public:
        bool supportICP() const {return false;}
    };

    class LTR24TypeInfoMod2 : public LTR24TypeInfo {
    public:
        bool supportICP() const {return true;}
    };


    double LTR24TypeInfo::adcFreqMax() const {
        return f_adc_freqs[0];
    }

    unsigned LTR24TypeInfo::adcRangesCnt(int unit_mode_num) const {
        return sizeof(f_ranges)/sizeof(f_ranges[0]);
    }

    double LTR24TypeInfo::adcRangeMaxVal(int range, int unit_mode_num) const {
         return f_ranges[range];
    }

    double LTR24TypeInfo::adcRangeMinVal(int range, int unit_mode_num) const {
        return -f_ranges[range];
    }

    const LTR24TypeInfo *LTR24TypeInfo::typeInfoMod1()     {
        static const LTR24TypeInfo info(false);
        return &info;
    }

    const LTR24TypeInfo *LTR24TypeInfo::typeInfoMod2() {
        static const LTR24TypeInfo info(true);
        return &info;
    }

    const LTR24TypeInfo *LTR24TypeInfo::defaultInfo() {
        return typeInfoMod1();
    }

    const QList<const LTR24TypeInfo *> LTR24TypeInfo::types() {
        return QList<const LTR24TypeInfo *>() << typeInfoMod1() << typeInfoMod2();
    }

    QString LTR24TypeInfo::devceModificationName() const {
       return  QString("%1-%2").arg(deviceTypeName()).arg(m_supportICP ? "2" : "1");
    }

    double LTR24TypeInfo::typeAdcFreqVal(int idx) {
        return f_adc_freqs[idx];
    }

    unsigned LTR24TypeInfo::typeAdcFreqsCnt() {
        return sizeof(f_adc_freqs)/sizeof(f_adc_freqs[0]);
    }
}
