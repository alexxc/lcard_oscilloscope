#include "DevOutAsyncDig.h"

namespace LQMeas {
    Error DevOutAsyncDig::outAsyncDigAll(unsigned val, unsigned mask) {
        return protOutAsyncDig(val, mask);
    }
}
