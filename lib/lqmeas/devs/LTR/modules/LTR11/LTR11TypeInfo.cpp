#include "LTR11TypeInfo.h"


namespace LQMeas {

    static double f_ranges[] = {10., 2.5, 0.625, 0.156};

    unsigned LTR11TypeInfo::adcRangesCnt(int unit_mode_num) const {
        return sizeof(f_ranges)/sizeof(f_ranges[0]);
    }

    double LTR11TypeInfo::adcRangeMaxVal(int range, int unit_mode_num) const {
         return f_ranges[range];
    }

    double LTR11TypeInfo::adcRangeMinVal(int range, int unit_mode_num) const {
        return -f_ranges[range];
    }

    const LTR11TypeInfo *LTR11TypeInfo::defaultInfo() {
        static const LTR11TypeInfo info;
        return &info;
    }
}
