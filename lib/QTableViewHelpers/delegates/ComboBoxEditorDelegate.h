#ifndef COMBOBOXEDITORDELEGATE_H
#define COMBOBOXEDITORDELEGATE_H


#include <QStyledItemDelegate>


class QComboBox;


class ComboBoxEditorDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    ComboBoxEditorDelegate(QObject *parent = 0);
    ~ComboBoxEditorDelegate();


    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

    bool eventFilter(QObject *object, QEvent *event);
protected:
    virtual void fillBoxItems(QComboBox *box, const QModelIndex &index) const;
    virtual QVariant getBoxData(QComboBox *box, int idx) const;
    virtual int getBoxIndexByData(QComboBox *box, const QModelIndex &index) const;

private Q_SLOTS:
    void onItemActivated(int idx);

private:
    mutable QComboBox *m_box;
    mutable int m_idx;

};

#endif // COMBOBOXEDITORDELEGATE_H
