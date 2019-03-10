#ifndef LQMEAS_LCOMPRESOLVER_H
#define LQMEAS_LCOMPRESOLVER_H

#include "lqmeas/devs/resolver/DevicesResolver.h"


namespace LQMeas {
    class lcompResolver : public DevicesResolver {
    public:

        static lcompResolver* resolver();

        QList<QSharedPointer<Device> > getDevList();

    private:
        static const unsigned max_devs_cnt = 255;

        explicit lcompResolver();

        typedef void *(*CREATEFUNCPTR)(quint32 Slot);


        CREATEFUNCPTR m_create_func;
        QHash<unsigned, QSharedPointer<Device> >  m_devs;
    };
}

#endif // LCOMPRESOLVER_H
