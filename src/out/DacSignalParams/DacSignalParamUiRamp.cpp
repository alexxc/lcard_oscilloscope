#include "DacSignalParamUiRamp.h"
#include "lqmeas/ifaces/out/DacSignals/DacSignalRamp.h"
#include "lqmeas/ifaces/out/DevOutInfo.h"

#include <QDoubleSpinBox>
#include <QFormLayout>


DacSignalParamUiRamp::DacSignalParamUiRamp() {

}

QString DacSignalParamUiRamp::showName() const {
    return LQMeas::DacSignalRamp::signalName();
}

QString DacSignalParamUiRamp::typeName() const {
    return LQMeas::DacSignalRamp::signalTypeName();
}

void DacSignalParamUiRamp::createUi(const LQMeas::DevOutInfo *devinfo, int ch, QFormLayout *lout) {
    m_freqBox = new QDoubleSpinBox();
    m_freqBox->setMaximum(devinfo->outSyncGenFreqMax());
    m_freqBox->setDecimals(3);
    m_freqBox->setSuffix(" " + LQMeas::Units::Hz()->name());
    m_freqBox->setValue(devinfo->outSyncGenFreqMax()/10);
    lout->addRow(tr("Frequency"), m_freqBox);

    m_initialAmpBox = new QDoubleSpinBox();
    m_initialAmpBox->setMaximum(devinfo->outDacMaxVal(ch));
    m_initialAmpBox->setMinimum(devinfo->outDacMinVal(ch));
    m_initialAmpBox->setDecimals(3);
    m_initialAmpBox->setSuffix(" " + devinfo->outDacUnit(ch)->name());
    m_initialAmpBox->setValue(m_initialAmpBox->maximum());
    lout->addRow(tr("Initial Amplitude"), m_initialAmpBox);

    m_endAmpBox = new QDoubleSpinBox();
    m_endAmpBox->setMaximum(devinfo->outDacMaxVal(ch));
    m_endAmpBox->setMinimum(devinfo->outDacMinVal(ch));
    m_endAmpBox->setDecimals(3);
    m_endAmpBox->setSuffix(" " + devinfo->outDacUnit(ch)->name());
    m_endAmpBox->setValue(m_endAmpBox->minimum());
    lout->addRow(tr("End Amplitude"), m_endAmpBox);

    QSharedPointer<LQMeas::DacSignalRamp> ramp = qSharedPointerDynamicCast<LQMeas::DacSignalRamp>(m_curSignal);
    if (ramp) {
        m_freqBox->setValue(ramp->signalFreq());
        m_initialAmpBox->setValue(ramp->initAmplitude());
        m_endAmpBox->setValue(ramp->endAmplitude());
    }
}

void DacSignalParamUiRamp::freeUi() {

}

QSharedPointer<LQMeas::DacSignal> DacSignalParamUiRamp::createSignal() {
    return QSharedPointer<LQMeas::DacSignal>(new LQMeas::DacSignalRamp(m_initialAmpBox->value(),
                                                              m_endAmpBox->value(),
                                                              m_freqBox->value()));
}

