#include "DacSignalParamUiPulse.h"
#include "lqmeas/ifaces/out/DacSignals/DacSignalPulse.h"
#include "lqmeas/ifaces/out/DevOutInfo.h"

#include <QDoubleSpinBox>
#include <QFormLayout>


DacSignalParamUiPulse::DacSignalParamUiPulse() {

}

QString DacSignalParamUiPulse::showName() const  {
    return LQMeas::DacSignalPulse::signalName();
}

QString DacSignalParamUiPulse::typeName() const {
    return LQMeas::DacSignalPulse::signalTypeName();
}

void DacSignalParamUiPulse::createUi(const LQMeas::DevOutInfo *devinfo, int ch, QFormLayout *lout) {
    m_freqBox = new QDoubleSpinBox();
    m_freqBox->setMaximum(devinfo->outSyncGenFreqMax());
    m_freqBox->setDecimals(3);
    m_freqBox->setSuffix(" " + LQMeas::Units::Hz()->name());
    m_freqBox->setValue(devinfo->outSyncGenFreqMax()/10);
    lout->addRow(tr("Frequency"), m_freqBox);

    m_dutyBox = new QDoubleSpinBox();
    m_dutyBox->setMaximum(100);
    m_dutyBox->setMinimum(0);
    m_dutyBox->setValue(50);
    m_dutyBox->setDecimals(3);
    m_dutyBox->setSuffix(" " + LQMeas::Units::percentage()->name());
    lout->addRow(tr("Duty Cycle"), m_dutyBox);

    m_pulseAmpBox = new QDoubleSpinBox();
    m_pulseAmpBox->setMaximum(devinfo->outDacMaxVal(ch));
    m_pulseAmpBox->setMinimum(devinfo->outDacMinVal(ch));
    m_pulseAmpBox->setDecimals(3);
    m_pulseAmpBox->setValue(m_pulseAmpBox->maximum());
    m_pulseAmpBox->setSuffix(" " + devinfo->outDacUnit(ch)->name());
    lout->addRow(tr("Pulse amplitude"), m_pulseAmpBox);

    m_pauseAmpBox = new QDoubleSpinBox();
    m_pauseAmpBox->setMaximum(devinfo->outDacMaxVal(ch));
    m_pauseAmpBox->setMinimum(devinfo->outDacMinVal(ch));
    m_pauseAmpBox->setDecimals(3);
    m_pauseAmpBox->setValue(m_pulseAmpBox->minimum());
    m_pauseAmpBox->setSuffix(" " + devinfo->outDacUnit(ch)->name());
    lout->addRow(tr("Pause amplitude"), m_pauseAmpBox);

    QSharedPointer<LQMeas::DacSignalPulse> pulse = qSharedPointerDynamicCast<LQMeas::DacSignalPulse>(m_curSignal);
    if (pulse) {
        m_freqBox->setValue(pulse->signalFreq());
        m_dutyBox->setValue(pulse->dutyCycle()*100);
        m_pulseAmpBox->setValue(pulse->pulseAmplitude());
        m_pauseAmpBox->setValue(pulse->pauseAmplitude());
    }
}

void DacSignalParamUiPulse::freeUi() {

}

QSharedPointer<LQMeas::DacSignal> DacSignalParamUiPulse::createSignal() {
    return QSharedPointer<LQMeas::DacSignalPulse>(new LQMeas::DacSignalPulse(
                                                      m_pulseAmpBox->value(),
                                                      m_pauseAmpBox->value(),
                                                      m_freqBox->value(),
                                                      m_dutyBox->value()/100));
}

