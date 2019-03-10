#include "DevOutAsyncDac.h"

namespace LQMeas {
    Error DevOutAsyncDac::outAsyncDac(int ch, double val) {
        return protOutAsyncDac(ch, val);
    }
}
