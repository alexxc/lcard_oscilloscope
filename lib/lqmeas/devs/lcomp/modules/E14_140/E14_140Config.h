#ifndef LQMEAS_E14_140CONFIG_H
#define LQMEAS_E14_140CONFIG_H

#include "lqmeas/devs/lcomp/lcompSeqAdcConfig.h"
#include "lqmeas/devs/DeviceConfig.h"

namespace LQMeas {
    class E14_140Config : public DeviceConfig, public lcompSeqAdcConfig {
    public:
        enum AdcRange {
            AdcRange_10V = 0,
            AdcRange_2_5V,
            AdcRange_0_625V,
            AdcRange_0_156V
        };


        static QString typeConfigName();
        QString configDevTypeName() const {return typeConfigName();}

        const DevAdcConfig *adcConfig() const {
            return static_cast<const DevAdcConfig*>(this);
        }

        DeviceConfig *clone() const {
            return new E14_140Config(this);
        }

        E14_140Config();
        E14_140Config(const E14_140Config *cfg);

        unsigned adcRangesCnt() const;
    protected:
        Error protLoadConfig(QSettings &set);
        Error protSaveConfig(QSettings &set) const;
        Error protUpdateConfig();
        void adcAdjustFreq(double *adcFreq, double *adcChFreq) const;
    private:
        static const unsigned quartz = 16000000;

    };
}

#endif // LQMEAS_E14_140CONFIG_H
