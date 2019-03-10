#include "E14_140Config.h"
#include "E14_140TypeInfo.h"

namespace LQMeas {
    E14_140Config::E14_140Config() : lcompSeqAdcConfig(E14_140TypeInfo::defaultInfo()) {

    }

    E14_140Config::E14_140Config(const E14_140Config *cfg) : lcompSeqAdcConfig(cfg) {

    }

    QString E14_140Config::typeConfigName() {
        return E14_140TypeInfo::name();
    }

    Error E14_140Config::protLoadConfig(QSettings &set) {
        return protAdcLoadConfig(set);
    }

    Error E14_140Config::protSaveConfig(QSettings &set) const {
        return protAdcSaveConfig(set);
    }

    Error E14_140Config::protUpdateConfig() {
        return protAdcUpdateConfig();
    }

    void E14_140Config::adcAdjustFreq(double *adcFreq, double *adcChFreq) const {
        double QF = quartz/1000.0;
        double DSP_CLOCK_OUT_PLX=QF;

        unsigned nch = adcEnabledChCnt();
        double dRate = *adcFreq/1000;
        double dKadr = 1000./(*adcChFreq) - 1000 *(nch-1)/(*adcFreq);

        // частота сбора в единицах SCLOCK_DIV SPORT DSP
        double SCLOCK_DIV=DSP_CLOCK_OUT_PLX/(2.0*(dRate))+0.5;
        if(SCLOCK_DIV > 65536.0)
            SCLOCK_DIV=65536.0;

        int llimit = 40; // проверка на верхний предел частоты 100 для А и 200 для В
        /** @todo сделать через devInfo как-то определение ревизии */
        //if(pdu.t5.Rev=='A') llimit=80;
        if(SCLOCK_DIV < llimit)
            SCLOCK_DIV=llimit;

        unsigned Rate = (unsigned)SCLOCK_DIV;
        dRate=DSP_CLOCK_OUT_PLX/(2.0*(Rate));

          //
       int kadrdelay=(int)((dKadr)*(dRate)+0.5);
       if(kadrdelay > 256) kadrdelay=256;
       if(kadrdelay == 0) kadrdelay=1;

       dKadr=(kadrdelay)/(dRate);


       *adcFreq = 1000. * dRate;
       *adcChFreq = 1000./(1000.*(nch-1)/ *adcFreq + dKadr);
    }
}
