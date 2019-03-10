#include "DacSignalParamUiSin.h"
#include "qmath.h"

#include "lqmeas/ifaces/out/DacSignals/DacSignalSin.h"
#include "lqmeas/ifaces/out/DevOutInfo.h"

#include <QFormLayout>
#include <QDoubleSpinBox>


DacSignalParamUiSin::DacSignalParamUiSin() {

}

QString DacSignalParamUiSin::showName() const {
    return LQMeas::DacSignalSin::signalName();
}

QString DacSignalParamUiSin::typeName() const {
    return LQMeas::DacSignalSin::signalTypeName();
}

void DacSignalParamUiSin::createUi(const LQMeas::DevOutInfo *devinfo, int ch, QFormLayout *lout) {
    m_freqBox = new QDoubleSpinBox();
    m_freqBox->setMaximum(devinfo->outSyncGenFreqMax());
    m_freqBox->setDecimals(3);
    m_freqBox->setSuffix(" " + LQMeas::Units::Hz()->name());
    m_freqBox->setValue(devinfo->outSyncGenFreqMax()/10);
    lout->addRow(tr("Frequency"), m_freqBox);

    m_ampBox = new QDoubleSpinBox();
    m_ampBox->setMaximum((devinfo->outDacMaxVal(ch) - devinfo->outDacMinVal(ch))/2);
    m_ampBox->setMinimum(0);
    m_ampBox->setValue(m_ampBox->maximum());
    m_ampBox->setSuffix(" " + devinfo->outDacUnit(ch)->name());
    m_ampBox->setDecimals(6);
    lout->addRow(tr("Amplitude"), m_ampBox);

    m_offsBox = new QDoubleSpinBox();
    m_offsBox->setMaximum(devinfo->outDacMaxVal(ch));
    m_offsBox->setMinimum(devinfo->outDacMinVal(ch));    
    m_offsBox->setSuffix(" " + devinfo->outDacUnit(ch)->name());
    m_offsBox->setDecimals(6);
    m_offsBox->setValue((m_offsBox->maximum() + m_offsBox->minimum())/2);
    lout->addRow(tr("Offset"), m_offsBox);

    m_phaBox = new QDoubleSpinBox();
    m_phaBox->setMaximum(360);
    m_phaBox->setMinimum(0);
    m_phaBox->setSuffix(LQMeas::Units::degree()->name());
    lout->addRow(tr("Phase"), m_phaBox);

    QSharedPointer<LQMeas::DacSignalSin> sin = qSharedPointerDynamicCast<LQMeas::DacSignalSin>(m_curSignal);
    if (sin) {
        m_freqBox->setValue(sin->signalFreq());
        m_ampBox->setValue(sin->amplitude());
        m_offsBox->setValue(sin->offset());
        m_phaBox->setValue(sin->phase()*180/M_PI);
    }
}

void DacSignalParamUiSin::freeUi() {

}

QSharedPointer<LQMeas::DacSignal> DacSignalParamUiSin::createSignal() {
    return QSharedPointer<LQMeas::DacSignal>(new LQMeas::DacSignalSin(m_ampBox->value(),
                                                              m_offsBox->value(),
                                                              m_freqBox->value(),
                                                              m_phaBox->value()*M_PI/180));
}
