#include "OutSignalFabric.h"
#include "DacSignals/DacSignalSin.h"
#include "DacSignals/DacSignalConst.h"
#include "DacSignals/DacSignalPulse.h"
#include "DacSignals/DacSignalRamp.h"
#include "lqmeas/units/Units.h"
#include <qmath.h>
#include <QSettings>

namespace LQMeas {

    OutSignalFabric::OutSignalFabric() {
    }

    const OutSignalFabric *OutSignalFabric::instance() {
        static OutSignalFabric instance;
        return &instance;
    }

    QSharedPointer<DacSignal> OutSignalFabric::createDacSignal(QSettings &set, QString key) const {
        QSharedPointer<DacSignal> ret;
        if (!key.isEmpty())
            set.beginGroup(key);

        QString sigType = set.value("type").toString();
        if (sigType == DacSignalSin::signalTypeName()) {
            double amp = set.value("amplitude").toDouble();
            double offs = set.value("offset").toDouble();
            double freq = set.value("frequency", 1000).toDouble();
            double pha  = set.value("phase").toDouble();
            bool periodStrictPoints = set.value("strictPoints").toBool();
            ret = QSharedPointer<DacSignal>(new DacSignalSin(amp, offs, freq, pha, periodStrictPoints));
        } else if (sigType == DacSignalConst::signalTypeName()) {
            double amp = set.value("amplitude").toDouble();
            ret = QSharedPointer<DacSignalConst>(new DacSignalConst(amp));
        } else if (sigType == DacSignalPulse::signalTypeName()) {
            double ampPulse = set.value("amp_pulse").toDouble();
            double ampPause = set.value("amp_pause", 0).toDouble();
            double freq     = set.value("frequency", 1000).toDouble();
            double duty     = set.value("dutyCycle", 50).toDouble();
            if (duty > 1.)
                duty = 1.;
            if (duty < 0)
                duty = 0.;
            ret = QSharedPointer<DacSignalPulse>(new DacSignalPulse(ampPulse, ampPause, freq, duty));
        } else if (sigType == DacSignalRamp::signalTypeName()) {
            double ampStart  = set.value("amp_init").toDouble();
            double ampFinish = set.value("amp_end").toDouble();
            double freq      = set.value("frequency", 1000).toDouble();
            ret = QSharedPointer<DacSignalRamp>(new DacSignalRamp(ampStart, ampFinish, freq));
        }

        if (!key.isEmpty())
            set.endGroup();

        return ret;
    }

    QSharedPointer<DigSignal> OutSignalFabric::createDigSignal(QSettings &set, QString key) const     {
        QSharedPointer<DigSignal> ret;

        return ret;
    }

    void OutSignalFabric::saveSignal(QSharedPointer<OutSignal> signal, QSettings &set, QString key) const {
        if (!key.isEmpty())
            set.beginGroup(key);

        set.setValue("type", signal->typeName());
        set.setValue("strictPoints", signal->periodStrictPoints());

        if (signal->typeName() == DacSignalSin::signalTypeName()) {
            QSharedPointer<DacSignalSin> sin = qSharedPointerDynamicCast<DacSignalSin>(signal);
            set.setValue("amplitude", sin->amplitude());
            set.setValue("offset", sin->offset());
            set.setValue("frequency", sin->signalFreq());
            set.setValue("phase", sin->phase());
        } else if (signal->typeName() == DacSignalConst::signalTypeName()) {
            QSharedPointer<DacSignalConst> constSig = qSharedPointerDynamicCast<DacSignalConst>(signal);
            set.setValue("amplitude", constSig->amplitude());
        } else if (signal->typeName() == DacSignalPulse::signalTypeName()) {
            QSharedPointer<DacSignalPulse> pulseSig = qSharedPointerDynamicCast<DacSignalPulse>(signal);
            set.setValue("amp_pulse", pulseSig->pulseAmplitude());
            set.setValue("amp_pause", pulseSig->pauseAmplitude());
            set.setValue("frequency", pulseSig->signalFreq());
            set.setValue("dutyCycle", pulseSig->dutyCycle());
        } else if (signal->typeName() == DacSignalRamp::signalTypeName()) {
            QSharedPointer<DacSignalRamp> rampSig = qSharedPointerDynamicCast<DacSignalRamp>(signal);
            set.setValue("amp_init", rampSig->initAmplitude());
            set.setValue("amp_end", rampSig->endAmplitude());
            set.setValue("frequency", rampSig->signalFreq());
        }

        if (!key.isEmpty())
            set.endGroup();
    }

    QString OutSignalFabric::createSignalDescription(QSharedPointer<OutSignal> signal) const {
        QString descr;
        if (signal->typeName() == DacSignalSin::signalTypeName()) {
            QSharedPointer<DacSignalSin> sin = qSharedPointerDynamicCast<DacSignalSin>(signal);
            descr = tr("%1: F = %2 %3, Amp = %4, Offs = %5, Phase = %6%7")
                    .arg(DacSignalSin::signalName())
                    .arg(sin->signalFreq())
                    .arg(Units::Hz()->name())
                    .arg(sin->amplitude())
                    .arg(sin->offset())
                    .arg(sin->phase()*180/M_PI)
                    .arg(Units::degree()->name());
        } else if (signal->typeName() == DacSignalConst::signalTypeName()) {
            QSharedPointer<DacSignalConst> constSig = qSharedPointerDynamicCast<DacSignalConst>(signal);
            descr = tr("%1: Value = %2").
                    arg(DacSignalConst::signalName()).arg(constSig->amplitude());
        } else if (signal->typeName() == DacSignalPulse::signalTypeName()) {
            QSharedPointer<DacSignalPulse> pulseSig = qSharedPointerDynamicCast<DacSignalPulse>(signal);
            descr = tr("%1: F = %2 %3, D = %4 %, Amp Pulse = %5, Amp Pause = %6")
                    .arg(DacSignalPulse::signalName())
                    .arg(pulseSig->signalFreq())
                    .arg(Units::Hz()->name())
                    .arg(pulseSig->dutyCycle())
                    .arg(pulseSig->pulseAmplitude())
                    .arg(pulseSig->pauseAmplitude());
        } else if (signal->typeName() == DacSignalRamp::signalTypeName()) {
            QSharedPointer<DacSignalRamp> rampSig = qSharedPointerCast<DacSignalRamp>(signal);
            descr = tr("%1: F = %2 %3, Amp from %4 to %5")
                    .arg(DacSignalRamp::signalName())
                    .arg(rampSig->signalFreq())
                    .arg(Units::Hz()->name())
                    .arg(rampSig->initAmplitude())
                    .arg(rampSig->endAmplitude());
        }
        return descr;
    }
}
