#ifndef LQMEAS_LCOMPRECVPROCESSORIFACE_H
#define LQMEAS_LCOMPRECVPROCESSORIFACE_H

#include "lqmeas/Error.h"

namespace LQMeas {
    class lcompStreamReceiver;

    class lcompRecvProcessorIface {
        virtual Error lcompProcessRecvdData(const void *src_buf, unsigned size, double *dst_buf, unsigned flags, unsigned offs) = 0;

        friend class lcompStreamReceiver;
    };
}

#endif // LQMEAS_LCOMPRECVPROCESSORIFACE_H
