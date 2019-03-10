#include "QHexSpinBox.h"
#include <QLineEdit>
#include <QKeyEvent>

QHexSpinBox::QHexSpinBox(QWidget *parent) : QSpinBox(parent) {
    setBitsWidth(16);
    setPrefix("0x");
    setDisplayIntegerBase(16);


}

void QHexSpinBox::setBitsWidth(unsigned bit_cnt) {
    m_bits_width = bit_cnt;
    m_max_val = ((1 << bit_cnt) - 1);
    m_nibles = (bit_cnt + 3)/4;
    if (bit_cnt < 31) {
		setRange(0, static_cast<int>(m_max_val));
    } else {
        setRange(INT_MIN, INT_MAX);
    }
}

QString QHexSpinBox::textFromValue(int value) const {
	return QString("%1").arg(u(value), static_cast<int>(m_nibles), 16, QChar('0')).toUpper();
}

int QHexSpinBox::valueFromText(const QString &text) const {
    return i(text.toUInt(nullptr, 16));
}

QValidator::State QHexSpinBox::validate(QString &input, int &pos) const {
    QString copy(input);

    if (copy.startsWith("0x")) {
        copy.remove(0, 2);
    }


    pos -= copy.size() - copy.trimmed().size();
    copy = copy.trimmed();
    if (copy.isEmpty())
        return QValidator::Intermediate;
    input = QString("0x") + copy.toUpper();
    bool okay;
    unsigned int val = copy.toUInt(&okay, 16);
    if (!okay || (val > m_max_val))
        return QValidator::Invalid;
    return QValidator::Acceptable;
}

void QHexSpinBox::keyPressEvent(QKeyEvent *event) {
    QSpinBox::keyPressEvent(event);
    /* чтобы по вводу только был принят текст, но не передавался ниже */
    if (event->key() == Qt::Key_Return) {
        event->accept();
    }
}

unsigned int QHexSpinBox::u(int i) const {
    return *reinterpret_cast<unsigned int *>(&i);
}

int QHexSpinBox::i(unsigned int u) const {
    return *reinterpret_cast<int *>(&u);
}
