#ifndef DACSIGNALPARAMUISIN_H
#define DACSIGNALPARAMUISIN_H

#include "DacSignalParamUi.h"

class QDoubleSpinBox;

class DacSignalParamUiSin : public DacSignalParamUi {
    Q_OBJECT
public:
    DacSignalParamUiSin();

    QString showName() const;
    QString typeName() const;

    void createUi(const LQMeas::DevOutInfo *devinfo, int ch, QFormLayout *lout);
    void freeUi(void);

    virtual QSharedPointer<LQMeas::DacSignal> createSignal();

    QDoubleSpinBox *m_freqBox;
    QDoubleSpinBox *m_ampBox;
    QDoubleSpinBox *m_offsBox;
    QDoubleSpinBox *m_phaBox;

};

#endif // DACSIGNALPARAMUISIN_H
