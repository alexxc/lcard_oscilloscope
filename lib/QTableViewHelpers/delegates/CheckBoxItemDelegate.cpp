#include "CheckBoxItemDelegate.h"
#include "DelegateIndexSelector.h"

#include <QApplication>
#include <QStyleOptionButton>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>

CheckBoxItemDelegate::CheckBoxItemDelegate(QObject *parent, Qt::AlignmentFlag alignment, DelegateIndexSelector *paintSelector) :
    QStyledItemDelegate( parent ), m_paintSelector(paintSelector), m_align(alignment) {

}

CheckBoxItemDelegate::~CheckBoxItemDelegate() {
    delete m_paintSelector;
}

void CheckBoxItemDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    if (!m_paintSelector || m_paintSelector->delegatePaintEnabled(index)) {
        // Setting parameters
        Qt::CheckState state = (Qt::CheckState)index.data( Qt::CheckStateRole ).toInt();

        QStyleOptionButton btnOpt;

        btnOpt.state = 0;

        if (index.flags() & Qt::ItemIsEditable) {
            btnOpt.state |= QStyle::State_Enabled;
        } else {
            btnOpt.state |= QStyle::State_ReadOnly;
        }
        if ( option.state & QStyle::State_MouseOver )
            btnOpt.state |= QStyle::State_MouseOver; // Mouse over sell
        if (option.state & QStyle::State_Selected)
            btnOpt.state |= QStyle::State_Selected | QStyle::State_MouseOver;

        switch ( state ) {
        case Qt::Unchecked:
            btnOpt.state |= QStyle::State_Off;
            break;
        case Qt::PartiallyChecked:
            btnOpt.state |= QStyle::State_NoChange;
            break;
        case Qt::Checked:
            btnOpt.state |= QStyle::State_On;
            break;
        }
        // Check box rect
        fillCheckRect(option, btnOpt);

        QVariant bgBrush = index.data(Qt::BackgroundRole);
        if (bgBrush.isValid()) {
            painter->fillRect(option.rect, bgBrush.value<QBrush>());
        } else if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, option.palette.highlight());
        }

        // Mandatory: drawing check box
        QApplication::style()->drawControl( QStyle::CE_CheckBox, &btnOpt, painter );
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool CheckBoxItemDelegate::editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index ) {
    if (!m_paintSelector || m_paintSelector->delegatePaintEnabled(index)) {
        switch ( event->type() ) {
            case QEvent::MouseButtonPress:
                m_lastClickedIndex = index;
                break;
            case QEvent::MouseButtonRelease: {
                    if ( index != m_lastClickedIndex )
                        break;
                    QMouseEvent *e = static_cast< QMouseEvent * >( event );
                    if ( e->button() != Qt::LeftButton )
                        break;
                    m_lastClickedIndex = QModelIndex();

                    QStyleOptionButton btnOpt;
                    fillCheckRect(option, btnOpt);

                    if ( btnOpt.rect.contains( e->pos() ) ) {
                        Qt::CheckState state = (Qt::CheckState)index.data( Qt::CheckStateRole ).toInt();
                        state = state == Qt::Checked ? Qt::Unchecked : Qt::Checked;
                        model->setData(index, state, Qt::CheckStateRole);
                    }
                }
                break;
            case QEvent::MouseButtonDblClick:
                return true;
            default:
                break;
        }
    }

    return QAbstractItemDelegate::editorEvent( event, model, option, index );
}

void CheckBoxItemDelegate::fillCheckRect(const QStyleOptionViewItem &option, QStyleOptionButton &btnOpt) const {
    btnOpt.rect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, &btnOpt, NULL );
    int x;
    int y = option.rect.center().y() - btnOpt.rect.height() / 2;
    if (m_align & Qt::AlignLeft) {
        x = option.rect.left() + btnOpt.rect.width() / 2;
    } else if (m_align & Qt::AlignRight) {
        x = option.rect.right() - 3 * btnOpt.rect.width() / 2;
    } else {
        x = option.rect.center().x() - btnOpt.rect.width() / 2;
    }
    btnOpt.rect.moveTo( x, y );
}
