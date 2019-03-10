#include "DacSignalParamUiConst.h"


#include "lqmeas/ifaces/out/DacSignals/DacSignalConst.h"
#include "lqmeas/ifaces/out/DevOutInfo.h"

#include <QFormLayout>
#include <QDoubleSpinBox>





QString DacSignalParamUiConst::showName() const {
    return LQMeas::DacSignalConst::signalName();
}

QString DacSignalParamUiConst::typeName() const {
    return LQMeas::DacSignalConst::signalTypeName();
}

void DacSignalParamUiConst::createUi(const LQMeas::DevOutInfo *devinfo, int ch, QFormLayout *lout) {

    m_val = new QDoubleSpinBox();
    m_val->setMinimum(devinfo->outDacMinVal(ch));
    m_val->setMaximum(devinfo->outDacMaxVal(ch));
    m_val->setDecimals(6);
    m_val->setSuffix(" " + devinfo->outDacUnit(ch)->name());
    lout->addRow(tr("Value"), m_val);



    QSharedPointer<LQMeas::DacSignalConst> constSig =
            qSharedPointerDynamicCast<LQMeas::DacSignalConst>(m_curSignal);
    if (constSig) {
        m_val->setValue(constSig->amplitude());
    }
}

void DacSignalParamUiConst::freeUi() {

}

QSharedPointer<LQMeas::DacSignal> DacSignalParamUiConst::createSignal() {
    return QSharedPointer<LQMeas::DacSignal>(new LQMeas::DacSignalConst(m_val->value()));
}
