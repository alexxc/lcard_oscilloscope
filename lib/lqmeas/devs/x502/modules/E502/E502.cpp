#include "E502.h"
#include "e502api.h"
#include "E502Info.h"
#include "lqmeas/lqtdefs.h"

namespace LQMeas {

    QHostAddress E502::ipAddr() const {
        uint32_t addr;
        QHostAddress ret;
        Error err = error(E502_GetIpAddr(devHandle(), &addr));
        if (err.isSuccess()) {
            ret = QHostAddress(addr);
        }
        return ret;
    }

    size_t E502::outCycleGenMaxSize(const OutRamSignalGenerator *generator) const {
        int chCnt = generator->dacSignalsCount() + (generator->digSignalsCount() == 0 ? 0 : 1);
        return  chCnt == 0 ? 0 : 3.*1024*1024/chCnt;
    }

    const x502Info *E502::createInfo(const t_x502_info *info) const {
        return new E502Info(new E502TypeInfo(info->devflags, true), QSTRING_FROM_CSTR(info->serial),
                            info->fpga_ver, info->plda_ver, info->mcu_firmware_ver);
    }

    E502::E502(const t_x502_devrec *info) : x502(info, new E502Info()) {

    }
}
