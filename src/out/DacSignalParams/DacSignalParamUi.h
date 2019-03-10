#ifndef DACSIGNALPARAMUI_H
#define DACSIGNALPARAMUI_H

#include <QObject>
#include "lqmeas/ifaces/out/DacSignals/DacSignal.h"
#include <QSharedPointer>

class QFormLayout;
namespace LQMeas {
    class DevOutInfo;
}


class DacSignalParamUi : public QObject {
    Q_OBJECT
public:
    virtual QString showName() const = 0;
    virtual QString typeName() const = 0;

    virtual void createUi(const LQMeas::DevOutInfo *dac, int ch, QFormLayout *lout) = 0;
    virtual void freeUi(void) = 0;
    virtual void setSignal(QSharedPointer<LQMeas::DacSignal>  sig) { m_curSignal = sig;}

    virtual QSharedPointer<LQMeas::DacSignal> createSignal() = 0;

protected:
    QSharedPointer<LQMeas::DacSignal> m_curSignal;
};

#endif // DACSIGNALPARAMUI_H
