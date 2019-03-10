#ifndef QSPINBOXNOWHEEL_H
#define QSPINBOXNOWHEEL_H

#include <QSpinBox>

class SpinBoxNoWheel : public QSpinBox {
    Q_OBJECT
public:
    SpinBoxNoWheel(QWidget *parent = 0);
protected:
    void wheelEvent( QWheelEvent * e);
};

#endif // QSPINBOXNOWHEEL_H
