#include "lcompSeqAdcConfig.h"

namespace LQMeas {
    lcompSeqAdcConfig::lcompSeqAdcConfig(const DevAdcInfo *info) : DevAdcStdSeq32Config(info) {
    }

    lcompSeqAdcConfig::lcompSeqAdcConfig(const lcompSeqAdcConfig *cfg) : DevAdcStdSeq32Config(cfg) {

    }
}
