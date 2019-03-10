#include "ComboBoxEditorDelegate.h"
#include <QComboBox>
#include <QEvent>
#include <QKeyEvent>
#include <QAbstractItemView>

ComboBoxEditorDelegate::ComboBoxEditorDelegate(QObject *parent) :
    QStyledItemDelegate(parent) {
}

ComboBoxEditorDelegate::~ComboBoxEditorDelegate() {

}

QWidget *ComboBoxEditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    m_idx = -1;

    QComboBox *box = new QComboBox(parent);
    box->setFrame(false);
    fillBoxItems(box, index);
    box->setAutoFillBackground(true);
    connect(box, SIGNAL(activated(int)), SLOT(onItemActivated(int)));
    box->view()->installEventFilter((QObject*)this);
    m_box = box;
    return box;
}

void ComboBoxEditorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QComboBox *box = qobject_cast<QComboBox *>(editor);
    if (box) {
        int idx = getBoxIndexByData(box, index);

        if (idx < 0) {
            idx = 0;
        } else {
            m_idx = idx;
        }

        box->setCurrentIndex(idx);
    }
}

void ComboBoxEditorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                          const QModelIndex &index) const {
    QComboBox *box = qobject_cast<QComboBox *>(editor);
    if (box) {
        if ((box->currentIndex()>=0) && (box->currentIndex()!=m_idx)) {
            m_idx = box->currentIndex();
            model->setData(index, getBoxData(box, m_idx));
        }
    }
}

void ComboBoxEditorDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}


bool ComboBoxEditorDelegate::eventFilter(QObject *object, QEvent *event) {
    bool ret = false;
    QComboBox *box = qobject_cast<QComboBox *>(object);
    if (box) {
        ret = QStyledItemDelegate::eventFilter(object, event);
        if (event->type() == QEvent::Show) {
             box->showPopup();
        }
    } else {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvet = static_cast<QKeyEvent*>(event);
            if (keyEvet->key() == Qt::Key_Escape) {
                emit closeEditor(m_box);
            }
        }
    }
    return ret;
}

void ComboBoxEditorDelegate::fillBoxItems(QComboBox *box, const QModelIndex &index) const {
    box->addItems(index.data(Qt::UserRole).toStringList());
}

QVariant ComboBoxEditorDelegate::getBoxData(QComboBox *box, int idx) const {
    return box->currentText();
}

int ComboBoxEditorDelegate::getBoxIndexByData(QComboBox *box, const QModelIndex &index) const {
    QString data = index.data(Qt::EditRole).toString();
    for (int i=0; i < box->count(); i++) {
        if (box->itemText(i)==data) {
           return i;
        }
    }
    return -1;
}

void ComboBoxEditorDelegate::onItemActivated(int idx) {
    QComboBox *box = qobject_cast<QComboBox *>(sender());
    if (box && (idx >= 0)) {
        emit commitData(box);
        emit closeEditor(box);
    }
}


