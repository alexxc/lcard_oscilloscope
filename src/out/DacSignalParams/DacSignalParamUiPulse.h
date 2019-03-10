#ifndef DACSIGNALPARAMUIPULSE_H
#define DACSIGNALPARAMUIPULSE_H
#include "DacSignalParamUi.h"


class QDoubleSpinBox;

class DacSignalParamUiPulse : public DacSignalParamUi {
    Q_OBJECT
public:
    DacSignalParamUiPulse();

    QString showName() const;
    QString typeName() const;

    void createUi(const LQMeas::DevOutInfo *devinfo, int ch, QFormLayout *lout);
    void freeUi(void);

    QSharedPointer<LQMeas::DacSignal> createSignal();

    QDoubleSpinBox *m_freqBox;
    QDoubleSpinBox *m_dutyBox;
    QDoubleSpinBox *m_pulseAmpBox;
    QDoubleSpinBox *m_pauseAmpBox;
};

#endif // DACSIGNALPARAMUIPULSE_H
