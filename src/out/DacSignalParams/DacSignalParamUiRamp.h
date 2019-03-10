#ifndef DACSIGNALPARAMUIRAMP_H
#define DACSIGNALPARAMUIRAMP_H

#include "DacSignalParamUi.h"


class QDoubleSpinBox;

class DacSignalParamUiRamp : public DacSignalParamUi {
    Q_OBJECT
public:
    DacSignalParamUiRamp();

    QString showName() const;
    QString typeName() const;

    void createUi(const LQMeas::DevOutInfo *devinfo, int ch, QFormLayout *lout);
    void freeUi(void);

    QSharedPointer<LQMeas::DacSignal> createSignal();

    QDoubleSpinBox *m_freqBox;
    QDoubleSpinBox *m_initialAmpBox;
    QDoubleSpinBox *m_endAmpBox;
};


#endif // DACSIGNALPARAMUIRAMP_H
