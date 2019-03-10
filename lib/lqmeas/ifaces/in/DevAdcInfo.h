#ifndef LQMEAS_DEVADCINFO_H
#define LQMEAS_DEVADCINFO_H

#include "lqmeas/units/Units.h"

namespace LQMeas {
    class DevAdcInfo {
    public:
        enum AdcType {
            AdcTypeParallel,   /** Параллельные каналы */
            AdcTypeSequential  /** Последовательный опрос каналов (с коммутатором) */
        };

        virtual AdcType adcType() const = 0;
		virtual unsigned adcChannelsCnt() const = 0;
        virtual double adcFreqMax() const = 0;

        virtual bool adcIsChFreqConfigurable() const = 0;

        virtual unsigned adcRangesCnt(int unit_mode_num = 0) const = 0;
        virtual double adcRangeMaxVal(int range, int unit_mode_num = 0) const = 0;
        virtual double adcRangeMinVal(int range, int unit_mode_num = 0) const = 0;

        /* Единицы измерения величин, выдаваемых на данном канале ЦАП */
		virtual const Unit *adcChUnit(unsigned int ch_num, int unit_mode_num = 0) const {Q_UNUSED(ch_num); Q_UNUSED(unit_mode_num); return Units::V();}
        /* Количество режимов АЦП, влияющих на измеряемые величины и возможные диапазоны */
        virtual unsigned adcUnitModesCnt() const {return 1;}
    };
}

#endif // LQMEAS_DEVADCINFO_H
