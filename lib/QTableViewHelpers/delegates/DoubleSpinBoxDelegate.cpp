#include "DoubleSpinBoxDelegate.h"
#include <QDoubleSpinBox>
#include <math.h>

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent) : QStyledItemDelegate(parent) {

}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setFrame(false);

    QVariant varval;

    unsigned dec_val = 2;
    double step = 1;
    double max_val = 9999;
    double min_val = -9999;

    varval = index.model()->data(index, DecimalsRole);
    if (varval.isValid()) {
        dec_val = varval.toInt();
    }


    varval = index.model()->data(index, StepRole);
    if (dec_val > 0)
        step = pow(0.1, dec_val);
    if (varval.isValid()) {
        step = varval.toDouble();
    }

    varval = index.model()->data(index, MaxRole);
    if (varval.isValid()) {
        max_val = varval.toDouble();
    }

    varval = index.model()->data(index, MinRole);
    if (varval.isValid()) {
        min_val = varval.toDouble();
    }

    editor->setDecimals(dec_val);
    editor->setSingleStep(step);
    editor->setMaximum(max_val);
    editor->setMinimum(min_val);
    return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}
