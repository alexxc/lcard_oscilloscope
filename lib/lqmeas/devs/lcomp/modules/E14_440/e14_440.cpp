#include "e14_440.h"
#include "lqmeas/devs/lcomp/lcomp_headers.h"
#include "lqmeas/lqtdefs.h"

static double f_adc_ranges[] = {10, 2.5, 0.625,  0.156};

namespace LQMeas {
    QString e14_440::modificationName() const {
        return m_dac_present ? name() + "D" : name();
    }



    Error e14_440::lcompFillDescr(const void *descr) {
        PLATA_DESCR_E440 *pd = (PLATA_DESCR_E440 *)descr;
        m_serial = QSTRING_FROM_CSTR(pd->SerNum);
        m_dac_present = pd->IsDacPresent != 0;
        return Error::Success();
    }

    e14_440::e14_440(IDaqLDevice *dev, const __SLOT_PARAM *par, unsigned slot) :
        lcompStreamDevice(dev, par, slot) {
    }
}
