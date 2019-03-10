#ifndef AGILENT3458_H
#define AGILENT3458_H

#include "lqmeas/devs/LQMeasDevice.h"
#include "Agilent3458api.h"

class Agilent3458 : public LQMeasDevice {
    Q_OBJECT
public:
    static QString typeDeviceName() {return "Agilent3458";}

    QString name() const {return typeDeviceName();}
    QString serial() const {return "";}

    bool isOpened() const {return m_is_opened;}


    LQMeasError getValue(double *val);
    LQMeasError startSingle();

    void *rawHandle() {return &m_hnd;}


protected:

    LQMeasError error(int err) const;

    LQMeasError protOpen();
    LQMeasError protClose();
    LQMeasError protLoadConfig(QSettings &set) {return LQMeasError::Success();}
    LQMeasError protSaveConfig(QSettings &set) const {return LQMeasError::Success();}
    LQMeasError protConfigure();
private:
    bool m_is_opened;
    A3458 m_hnd;
    explicit Agilent3458();
    friend class gpibResolver;
};

#endif // AGILENT3458_H
