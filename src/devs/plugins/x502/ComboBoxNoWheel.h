#ifndef COMBOBOXNOWHEEL_H
#define COMBOBOXNOWHEEL_H

#include <QComboBox>

class ComboBoxNoWheel : public QComboBox {
    Q_OBJECT
public:
    ComboBoxNoWheel(QWidget *parent = 0);
protected:
    void wheelEvent( QWheelEvent * e);
};

#endif // COMBOBOXNOWHEEL_H
