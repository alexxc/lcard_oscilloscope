#include "E14_140.h"
#include "lqmeas/devs/lcomp/lcomp_headers.h"
#include "lqmeas/lqtdefs.h"
#include "E14_140Info.h"
#include "E14_140Config.h"

namespace LQMeas {
    const E14_140Config *E14_140::devspecConfig() const {
        return static_cast<const E14_140Config *>(currentConfig());
    }

    const E14_140Info *E14_140::devspecInfo() const {
        return static_cast<const E14_140Info*>(devInfo());
    }

    Error E14_140::lcompFillDescr(const void *descr) {
        PLATA_DESCR_E140 *pd = (PLATA_DESCR_E140 *)descr;

        setDevicInfo(new E14_140Info(E14_140TypeInfo::typeInfo(pd->Rev != 'A', pd->IsDacPresent != 0),
                                     QSTRING_FROM_CSTR(pd->SerNum)));

        for (unsigned range = 0; range < adc_ranges_cnt; range++) {
            m_cbr_b[range] = pd->KoefADC[range];
            m_cbr_k[range] = pd->KoefADC[4 + range];
        }

        return Error::Success();
    }

    Error E14_140::lcompFillAdcParams(void *par) const  {
        Error err = Error::Success();
        unsigned nch = 0;
        ADC_PAR *padcPar = (ADC_PAR *)par;
        const E14_140Config *cfg = devspecConfig();
        if (cfg) {
            padcPar->t1.s_Type = L_ADC_PARAM;
            padcPar->t1.AutoInit = 1;
            padcPar->t1.dScale = 0;
            padcPar->t1.SynchroType = 0; //3
            padcPar->t1.SynchroSensitivity = 0;
            padcPar->t1.SynchroMode = 0;
            padcPar->t1.AdChannel = 0;
            padcPar->t1.AdPorog = 0;

            unsigned ch_cnt = devTypeInfo()->adc()->adcChannelsCnt();
            for (unsigned ch=0; (ch < ch_cnt) && err.isSuccess(); ch++) {
                if (cfg->adcChEnabled(ch)) {
                    ULONG lch = 0;
                    lch |= (cfg->adcChRangeNum(ch) << 6);
                    lcompSeqAdcConfig::AdcChMode ch_mode = cfg->adcChMode(ch);
                    if (ch_mode == lcompSeqAdcConfig::AdcChModeZero) {
                        lch |= (1<<4);
                    } else if (ch_mode == lcompSeqAdcConfig::AdcChModeComm) {
                        lch |= (1<<5) | ch;
                    } else if (ch_mode == lcompSeqAdcConfig::AdcChModeDiff) {
                        if (ch < 16) {
                            lch |= ch;
                        } else {
                            err = StdErrors::InvalidAdcConfig();
                        }
                    } else {
                        err = StdErrors::InvalidAdcConfig();
                    }

                    if (err.isSuccess()) {
                        padcPar->t1.Chn[nch++] = lch;
                    }
                }
            }

            padcPar->t1.dRate = cfg->adcFreq()/1000;
            padcPar->t1.dKadr = 1000./cfg->adcChFreq() - 1000 *(nch-1)/cfg->adcFreq();
            padcPar->t1.NCh = nch;

            unsigned irq_step = cfg->adcFreq()/100;
            if (irq_step == 0) {
                irq_step = 64;
            } else {
                irq_step += 63;
                irq_step -= irq_step % 64;
            }


            padcPar->t1.FIFO = 4096;
            padcPar->t1.IrqStep = irq_step;
            padcPar->t1.Pages = 256;

            padcPar->t1.IrqEna = 1;
            padcPar->t1.AdcEna = 1;
        }

        return err;
    }

    Error E14_140::protConfigure(const DeviceConfig *cfg) {
        return lcompStreamDevice::protConfigure(cfg);
    }

    E14_140::E14_140(IDaqLDevice *dev, const __SLOT_PARAM *par, unsigned slot) :
        lcompStreamDevice(dev, par, slot, E14_140TypeInfo::defaultInfo()->adcChannelsCnt(),
                          new E14_140Config(), new E14_140Info()) {
    }


}
