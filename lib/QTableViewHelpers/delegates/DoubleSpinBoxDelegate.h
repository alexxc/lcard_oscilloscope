#ifndef DOUBLEPRECDELEGATE_H
#define DOUBLEPRECDELEGATE_H

#include <QStyledItemDelegate>

/* Делегат для редактирования значений типа double в DoubleSpinBox.
   В отличие от стандартного делегата позволяет настроить параметры
   элемента (кол-во числе после запятой, шаг приращения, макс. и мин. значения)
   по значениям модели на основе пользовательских ролей */
class DoubleSpinBoxDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    static const int DecimalsRole = Qt::UserRole + 1;
    static const int StepRole = Qt::UserRole + 2;
    static const int MaxRole = Qt::UserRole + 3;
    static const int MinRole = Qt::UserRole + 4;

    DoubleSpinBoxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // DOUBLEPRECDELEGATE_H
