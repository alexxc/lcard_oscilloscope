#include "L502.h"
#include "L502Info.h"
#include "lqmeas/lqtdefs.h"

namespace LQMeas {
    L502::L502(const t_x502_devrec *info) : x502(info, new L502Info()) {

    }

    size_t L502::outCycleGenMaxSize(const OutRamSignalGenerator *generator) const {
        int chCnt = generator->dacSignalsCount() + (generator->digSignalsCount() == 0 ? 0 : 1);
        return chCnt == 0 ? 0 : 150*1024*1024/chCnt;
    }

    const x502Info *L502::createInfo(const t_x502_info *info) const {
        return new L502Info(new L502TypeInfo(info->devflags, true), QSTRING_FROM_CSTR(info->serial),
                            info->fpga_ver, info->plda_ver);
    }
}
