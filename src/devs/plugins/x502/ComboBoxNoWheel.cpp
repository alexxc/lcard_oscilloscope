#include "ComboBoxNoWheel.h"
#include <QWheelEvent>

ComboBoxNoWheel::ComboBoxNoWheel(QWidget *parent) : QComboBox(parent) {

}

void ComboBoxNoWheel::wheelEvent(QWheelEvent *e) {
    e->ignore();
}
