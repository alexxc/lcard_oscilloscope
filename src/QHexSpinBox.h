#ifndef QHEXSPINBOX_H
#define QHEXSPINBOX_H

#include <QSpinBox>

class QHexSpinBox : public QSpinBox {
    Q_OBJECT
public:
    QHexSpinBox(QWidget *parent = 0);

    unsigned int hexValue() const {
        return u(value());
    }
    void setHexValue(unsigned int value) {
        setValue(i(value));
    }

    void setBitsWidth(unsigned bit_cnt);

protected:
    QString textFromValue(int value) const;
    int valueFromText(const QString &text) const;
    QValidator::State validate(QString &input, int &pos) const;

    void keyPressEvent(QKeyEvent *event);
private:
    unsigned int u(int i) const;
    int i(unsigned int u) const;

    unsigned m_max_val{};
    unsigned m_bits_width{};
    unsigned m_nibles{};
};

#endif // QHEXSPINBOX_H
