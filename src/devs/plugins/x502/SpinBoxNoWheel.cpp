#include "SpinBoxNoWheel.h"
#include <QWheelEvent>

SpinBoxNoWheel::SpinBoxNoWheel(QWidget *parent) : QSpinBox(parent) {

}

void SpinBoxNoWheel::wheelEvent(QWheelEvent *e) {
    e->ignore();
}
