#ifndef CHECKBOXITEMDELEGATE_H
#define CHECKBOXITEMDELEGATE_H


#include <QStyledItemDelegate>
#include <QModelIndex>

class DelegateIndexSelector;

class CheckBoxItemDelegate : public QStyledItemDelegate  {
    Q_OBJECT
public:
    CheckBoxItemDelegate(QObject *parent, Qt::AlignmentFlag alignment = Qt::AlignHCenter, DelegateIndexSelector *paintSelector = 0);
    ~CheckBoxItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option_init, const QModelIndex &index) const;
    bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index );

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {return 0;}
private:
    void fillCheckRect(const QStyleOptionViewItem &option, QStyleOptionButton &btnOpt) const;

    QModelIndex m_lastClickedIndex;
    DelegateIndexSelector *m_paintSelector;
    Qt::AlignmentFlag m_align;
};


#endif // CHECKBOXITEMDELEGATE_H
