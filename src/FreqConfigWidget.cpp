#include "FreqConfigWidget.h"
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QKeyEvent>
#include <QSettings>
#include "lqmeas/units/Units.h"

FreqConfigWidget::FreqConfigWidget(QWidget *parent) : QWidget(parent), m_val(0), m_min(0), m_max(100000) {
    auto *lout = new QHBoxLayout();
    m_freqEdit = new QDoubleSpinBox();
    m_freqEdit->setDecimals(3);
    m_freqEdit->setKeyboardTracking(false);

    lout->addWidget(m_freqEdit);

    m_unitBox = new QComboBox();
    m_unitBox->addItem(LQMeas::Units::Hz()->name(),  LQMeas::Units::Hz()->multipler());
    m_unitBox->addItem(LQMeas::Units::KHz()->name(), LQMeas::Units::KHz()->multipler());
    m_unitBox->addItem(LQMeas::Units::MHz()->name(), LQMeas::Units::MHz()->multipler());
    m_unitBox->setMaximumWidth(100);

    m_unitBox->setCurrentIndex(0);
    lout->addWidget(m_unitBox);
    lout->setMargin(0);
    setLayout(lout);

    connect(m_unitBox, SIGNAL(currentIndexChanged(int)), SLOT(onUnitChanged(int)));
    connect(m_freqEdit, SIGNAL(valueChanged(double)), SLOT(onValueChanged(double)));
    connect(this, SIGNAL(valueChangedFromUi(double)), SIGNAL(valueChanged(double)));

    onUnitChanged(0);
}

int FreqConfigWidget::unitMultCoef() const {
    return m_unitBox->itemData(m_unitBox->currentIndex()).toInt();
}

void FreqConfigWidget::loadUnitSettings(QSettings& set, const QString& key) {
    set.beginGroup(key);
    bool ok;
    int val = set.value("UnitMultCoef", 1).toInt(&ok);
    if (ok)
        setUnitMultCoef(val);
    set.endGroup();
}

void FreqConfigWidget::saveUnitSettings(QSettings& set, const QString& key) {
    set.beginGroup(key);
    set.setValue("UnitMultCoef", unitMultCoef());
    set.endGroup();
}

void FreqConfigWidget::setValue(double val) {
    if (m_val != val) {
        disconnect(m_freqEdit, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
        m_val = val;
        m_freqEdit->setValue(val/unitMultCoef());
        connect(m_freqEdit, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
        Q_EMIT valueChanged(m_val);
    }
}

void FreqConfigWidget::setReadOnly(bool readOnly) {
    m_freqEdit->setReadOnly(readOnly);
    setUnitReadOnly(readOnly);
}

void FreqConfigWidget::setMaximum(double max) {
    m_max = max;
    m_freqEdit->setMaximum(m_max/unitMultCoef());
}

void FreqConfigWidget::setMinimum(double min) {
    m_min = min;
    m_freqEdit->setMinimum(m_min/unitMultCoef());
}

void FreqConfigWidget::setUnitReadOnly(bool readOnly) {
    m_unitBox->setEnabled(!readOnly);
}

void FreqConfigWidget::setUnitMultCoef(int mult) {
    for (int i=0; i < m_unitBox->count(); i++) {
        if (m_unitBox->itemData(i).toInt() == mult) {
            m_unitBox->setCurrentIndex(i);
            break;
        }
    }
}

void FreqConfigWidget::onUnitChanged(int  /*idx*/) {
    int mult = unitMultCoef();
    disconnect(m_freqEdit, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
    m_freqEdit->setMaximum(m_max/mult);
    m_freqEdit->setMinimum(m_min/mult);
    m_freqEdit->setValue(m_val/mult);
    connect(m_freqEdit, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
    Q_EMIT unitChanged(mult);
}

void FreqConfigWidget::onValueChanged(double val) {
    double new_val = val * unitMultCoef();
    if (new_val != m_val) {
        m_val = new_val;
        Q_EMIT valueChangedFromUi(m_val);
    }
}

void FreqConfigWidget::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    /* чтобы по вводу только был принят текст и обновлено значение, но событие
       не передавалось дальше */
    if (event->key() == Qt::Key_Return) {
        event->accept();
    }
}

