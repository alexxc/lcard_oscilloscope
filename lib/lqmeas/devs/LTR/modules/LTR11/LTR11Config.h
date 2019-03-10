#ifndef LTR11CONFIG_H
#define LTR11CONFIG_H

#include "lqmeas/devs/DeviceConfig.h"
#include "lqmeas/ifaces/in/DevAdcStdSeq32Config.h"

namespace LQMeas {
    class LTR11Config : public DeviceConfig, public DevAdcStdSeq32Config {
    public:
        static QString typeConfigName();
        QString configDevTypeName() const {return typeConfigName();}

        enum StartMode {
            StartModeInternal,
            StartModeExtRise,
            StartModeExtFall
        };

        enum ConvMode {
            ConvModeInternal,
            ConvModeExtRise,
            ConvModeExtFall
        };

        const DevAdcConfig *adcConfig() const {
            return static_cast<const DevAdcConfig*>(this);
        }

        DeviceConfig *clone() const {
            return new LTR11Config(this);
        }

        LTR11Config();
        LTR11Config(const LTR11Config *cfg);

        bool adcExternalStart() const {
            return (m_startMode != StartModeInternal) ||
                    (m_convMode != ConvModeInternal);
        }

        StartMode startMode() const {return m_startMode;}
        ConvMode  convMode() const {return m_convMode;}

        void setStartMode(StartMode mode);
        void setConvMode(ConvMode mode);

    protected:
        Error protLoadConfig(QSettings &set);
        Error protSaveConfig(QSettings &set) const;
        Error protUpdateConfig();

        void adcAdjustFreq(double *adcFreq, double *AdcChFreq) const;
    private:
        StartMode m_startMode;
        ConvMode m_convMode;
    };

}

class LTR11Config
{
public:
    LTR11Config();
};

#endif // LTR11CONFIG_H
