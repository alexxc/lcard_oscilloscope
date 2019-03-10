#ifndef DACSIGNALPARAMUICONST_H
#define DACSIGNALPARAMUICONST_H

#include "DacSignalParamUi.h"

class QDoubleSpinBox;

class DacSignalParamUiConst : public DacSignalParamUi {
    Q_OBJECT
public:
    QString showName() const;
    QString typeName() const;

    void createUi(const LQMeas::DevOutInfo *devinfo, int ch, QFormLayout *lout);
    void freeUi(void);

    virtual QSharedPointer<LQMeas::DacSignal> createSignal();
private:
    QDoubleSpinBox *m_val;
};

#endif // DACSIGNALPARAMUICONST_H
