#ifndef FREQCONFIGWIDGET_H
#define FREQCONFIGWIDGET_H

#include <QWidget>
class QSettings;
class QDoubleSpinBox;
class QComboBox;

class FreqConfigWidget : public QWidget {
    Q_OBJECT
public:
    explicit FreqConfigWidget(QWidget *parent = 0);

    int unitMultCoef() const;
    double value() const {return m_val;}

    void loadUnitSettings(QSettings &set, const QString& key);
    void saveUnitSettings(QSettings &set, const QString& key);
Q_SIGNALS:
    void valueChanged(double val);
    void valueChangedFromUi(double val);
    void unitChanged(int mult);
public Q_SLOTS:
    void setValue(double val);
    void setReadOnly(bool readOnly);
    void setMaximum(double max);
    void setMinimum(double min);
    void setUnitReadOnly(bool readOnly);
    void setUnitMultCoef(int mult);

private Q_SLOTS:
    void onUnitChanged(int idx);
    void onValueChanged(double val);
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    QDoubleSpinBox *m_freqEdit;
    QComboBox *m_unitBox;
    double m_val, m_max, m_min;
};

#endif // FREQCONFIGWIDGET_H
