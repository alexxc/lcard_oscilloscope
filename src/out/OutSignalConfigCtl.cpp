#include "OutSignalConfigCtl.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QEvent>

#include "lqmeas/ifaces/out/OutSignalFabric.h"
#include "DacSignalDialog.h"

OutSignalConfigCtl::OutSignalConfigCtl(QWidget *parent, QSharedPointer<LQMeas::DevOutSyncConfig> cfg, const LQMeas::DevOutInfo *devInfo,
                                       QList<QCheckBox*> chEnBoxList,
                                       QList<QLineEdit*> chSigEditList, bool change_enabled) :
    QObject(parent), m_info(devInfo), m_parentWgt(parent), m_chEnBoxList(chEnBoxList), m_chSigEditList(chSigEditList), m_cfg(cfg) {

    m_dacChCnt = qMin(cfg->outSyncDacChannelsCount(), qMin(chEnBoxList.size(), chSigEditList.size()));

    for (int ch = 0; ch < chEnBoxList.size(); ch++) {
        if (ch < m_dacChCnt) {
            m_chEnBoxList[ch]->setChecked(cfg->outSyncDacChEnabled(ch));
            showSigText(ch);
            chSigEditList[ch]->setReadOnly(true);
            if (change_enabled) {
                chSigEditList[ch]->installEventFilter(this);
                connect(m_chEnBoxList[ch], SIGNAL(stateChanged(int)), SLOT(onEnabledChanged(int)));
                connect(m_chSigEditList[ch], SIGNAL(textChanged(QString)), SLOT(onTextChanged()));
            } else {
                m_chEnBoxList[ch]->setEnabled(false);
                m_chSigEditList[ch]->setEnabled(false);
            }
        } else {
            m_chEnBoxList[ch]->setEnabled(false);
            if (ch < m_chSigEditList.size())
                m_chSigEditList[ch]->setEnabled(false);
        }
    }
}

OutSignalConfigCtl::~OutSignalConfigCtl() {

}

bool OutSignalConfigCtl::eventFilter(QObject *obj, QEvent *ev) {
    QLineEdit *edt = qobject_cast<QLineEdit*>(obj);

    if (edt && (ev->type() == QEvent::MouseButtonPress)) {
        int fnd_ch = -1;
        for (int ch=0; (ch < m_dacChCnt) && (fnd_ch < 0); ch++) {
            if (m_chSigEditList[ch] == edt)
                fnd_ch = ch;
        }

        if (fnd_ch >= 0) {
            DacSignalDialog dlg(m_parentWgt, m_info, m_cfg.data(), fnd_ch);
            if (dlg.exec() == QDialog::Accepted) {
                showSigText(fnd_ch);
            }
        }
    }
    return false;
}


void OutSignalConfigCtl::onEnabledChanged(int state) {
    QCheckBox *box = qobject_cast<QCheckBox*>(sender());
    int fnd_ch = -1;
    for (int ch=0; (ch < m_dacChCnt) && (fnd_ch < 0); ch++) {
        if (m_chEnBoxList[ch] == box)
            fnd_ch = ch;
    }

    if (fnd_ch >= 0) {
        if (state == Qt::Checked) {
            QSharedPointer<LQMeas::DacSignal> sig = m_cfg->outSyncDacSignal(fnd_ch);
            if (sig) {
                m_cfg->outSyncDacChEnable(fnd_ch, true);
            } else {
                DacSignalDialog dlg(m_parentWgt, m_info, m_cfg.data(), fnd_ch);
                if (dlg.exec() == QDialog::Accepted) {
                    m_cfg->outSyncDacChEnable(fnd_ch, true);
                    showSigText(fnd_ch);
                }
            }
        } else {
            m_cfg->outSyncDacChEnable(fnd_ch, false);
        }
        m_chEnBoxList[fnd_ch]->setCheckState(m_cfg->outSyncDacChEnabled(fnd_ch) ? Qt::Checked : Qt::Unchecked);
    }
}

void OutSignalConfigCtl::onTextChanged() {
    QLineEdit *edt = qobject_cast<QLineEdit*>(sender());
    if (edt) {
        edt->setCursorPosition(0);
    }
}

void OutSignalConfigCtl::showSigText(int ch) {
    QSharedPointer<LQMeas::DacSignal> sig = m_cfg->outSyncDacSignal(ch);
    if (sig) {
        m_chSigEditList[ch]->setText(LQMeas::OutSignalFabric::instance()->createSignalDescription(sig));
        m_chSigEditList[ch]->setCursorPosition(0);
    } else {
        m_chSigEditList[ch]->setText("");
    }
}


