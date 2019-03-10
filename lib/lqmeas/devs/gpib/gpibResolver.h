#ifndef GPIBRESOLVER_H
#define GPIBRESOLVER_H

#include "lqmeas/devs/resolver/LQDevResolver.h"

class gpibResolver : public LQDevResolver {
    Q_OBJECT
public:
    static gpibResolver* resolver();

    QList<QSharedPointer<LQMeasDevice> > getDevList();
private:


    QList<QSharedPointer<LQMeasDevice> >  m_dev;
};

#endif // GPIBRESOLVER_H
