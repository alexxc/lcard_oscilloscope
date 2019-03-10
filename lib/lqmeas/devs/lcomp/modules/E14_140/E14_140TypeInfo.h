#ifndef LQMEAS_E14_140TYPEINFO_H
#define LQMEAS_E14_140TYPEINFO_H

#include "lqmeas/devs/DeviceTypeInfo.h"
#include "lqmeas/ifaces/in/DevAdcInfo.h"
#include <QList>

namespace LQMeas {
    class E14_140TypeInfo : public DeviceTypeInfo, public DevAdcInfo {
    public:
        static QString name() {return "E14-440";}
        static const E14_140TypeInfo *defaultInfo();

        static const E14_140TypeInfo *typeInfoBase();
        static const E14_140TypeInfo *typeInfoD();
        static const E14_140TypeInfo *typeInfoM();
        static const E14_140TypeInfo *typeInfoMD();


        static const E14_140TypeInfo *typeInfo(bool modificated, bool dacPresent);

        static QList<const E14_140TypeInfo *> types();


        const DevAdcInfo *adc() const {return static_cast<const DevAdcInfo *>(this);}

        QString deviceTypeName() const {return name();}
        QString devceModificationName() const {return m_modName;}

        AdcType adcType() const {return AdcTypeSequential;}
        unsigned adcChannelsCnt() const {return 32;}

        double adcFreqMax() const {return 200000;}

        bool adcIsChFreqConfigurable() const {return true;}

        unsigned adcRangesCnt(int unit_mode_num = 0) const;
        double adcRangeMaxVal(int range, int unit_mode_num = 0) const;
        double adcRangeMinVal(int range, int unit_mode_num = 0) const;

        bool modificated() const {return m_modificated;}
        bool dacPresent() const {return m_dacPresent;}
    private:
        E14_140TypeInfo(bool modifiecated, bool has_dac);

        QString m_modName;
        bool m_modificated;
        bool m_dacPresent;
    };
}

#endif // LQMEAS_E14_140TYPEINFO_H
