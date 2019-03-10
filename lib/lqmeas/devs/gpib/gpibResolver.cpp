#include "gpibResolver.h"
#include "Agilent3458.h"

gpibResolver *gpibResolver::resolver() {
    static gpibResolver resolver;
    return &resolver;
}

QList<QSharedPointer<LQMeasDevice> > gpibResolver::getDevList() {
    if (m_dev.size()==0) {
        m_dev.append(QSharedPointer<LQMeasDevice>(new Agilent3458()));
    }
    return m_dev;
}
