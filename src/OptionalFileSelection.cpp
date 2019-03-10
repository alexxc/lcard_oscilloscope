#include "OptionalFileSelection.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QEvent>
#include <QFileDialog>
#include <QSettings>


OptionalFileSelection::OptionalFileSelection(QWidget *parent, QCheckBox *enBox, QLineEdit *edt, bool change_enabled) :
    QObject(parent), m_parentWgt(parent), m_enBox(enBox), m_edt(edt) {

    m_edt->setReadOnly(true);
    if (change_enabled) {
        edt->installEventFilter(this);
        connect(enBox, SIGNAL(stateChanged(int)), SLOT(onEnabledChanged(int)));
    } else {        
        enBox->setEnabled(false);
    }
}

bool OptionalFileSelection::eventFilter(QObject *obj, QEvent *ev) {
    auto *edt = qobject_cast<QLineEdit*>(obj);

    if (edt && (ev->type() == QEvent::MouseButtonPress)) {
        startFileSelection();        
    }
    return false;
}

void OptionalFileSelection::onEnabledChanged(int state) {
    auto *box = qobject_cast<QCheckBox*>(sender());
    if (box) {
        if (state == Qt::Checked) {
            if (m_edt->text().isEmpty()) {
                startFileSelection();
            }
        }
    }
}

void OptionalFileSelection::startFileSelection() {
    QSettings set;
    QString dir;
    bool savedir = !fileLastdirSettingsEntry().isEmpty();
    if (savedir)
        dir = set.value(fileLastdirSettingsEntry()).toString();

    QString filename = QFileDialog::getOpenFileName(m_parentWgt, fileDlgCaption(),
                                                    dir, fileDlgFilters());
    if (!filename.isEmpty()) {
        m_edt->setText(filename);
        if (savedir)
            set.setValue(fileLastdirSettingsEntry(), filename);
    }
}
