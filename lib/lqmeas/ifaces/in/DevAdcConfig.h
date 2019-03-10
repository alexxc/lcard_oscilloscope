#ifndef LQMEAS_DEVADCCONFIG_H
#define LQMEAS_DEVADCCONFIG_H

#include "DevAdcInfo.h"
#include <QVector>
#include "lqmeas/units/Units.h"

namespace LQMeas {
    class DevAdcConfig {
    public:
        DevAdcConfig(const DevAdcInfo *info);
        DevAdcConfig(const DevAdcConfig *cfg);

        const DevAdcInfo *adcInfo() const {return m_adc_info;}

        virtual double adcFreq() const = 0;
        virtual double adcChFreq() const;
        /* начальное смещение по времени заданного канала АЦП относительно первого канала */
		virtual double adcChTimeShift(unsigned int ch) const;

		virtual bool adcChEnabled(unsigned int ch) const = 0;
        virtual bool adcExternalStart() const {return false;}

		virtual unsigned adcEnabledChCnt() const {return static_cast<unsigned>(m_ch_indexes.size());}

		virtual int adcChRangeNum(unsigned int ch) const = 0;
		virtual double adcChRangeMaxVal(unsigned int ch) const {
            return adcInfo()->adcRangeMaxVal(adcChRangeNum(ch), adcUnitMode(ch));
        }
		virtual double adcChRangeMinVal(unsigned int ch) const {
            return adcInfo()->adcRangeMinVal(adcChRangeNum(ch), adcUnitMode(ch));
        }

		virtual int adcUnitMode(unsigned int ch) const {Q_UNUSED(ch); return 0;}
		virtual const Unit *adcChUnit(unsigned int ch) const {
            return m_adc_info->adcChUnit(ch, adcUnitMode(ch));
        }


        /* получить номер физ. канала по индексу при приеме */
		virtual int adcChNum(unsigned int idx) const;
		virtual int adcChIdx(unsigned int ch) const;
    private:
        void adcUpdateConfig() const;
        void adcUpdateDevInfo(const DevAdcInfo *info);
        const DevAdcInfo *m_adc_info;
        mutable QVector<int> m_ch_indexes;

        friend class DeviceConfig;
    };
}

#endif // LQMEAS_DEVADCCONFIG_H
