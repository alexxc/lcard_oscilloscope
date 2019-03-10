#include "X502AsyncIoDialog.h"
#include "ui_X502AsyncIoDialog.h"
#include "QLedIndicator.h"
#include <QGridLayout>
#include <QLabel>
#include "lqmeas/devs/DeviceTypeInfo.h"
#include "lqmeas/ifaces/out/DevOutInfo.h"
#include "lqmeas/ifaces/out/DevOutAsyncDig.h"
#include "lqmeas/ifaces/in/DevInAsyncDig.h"
#include <QMessageBox>
#include <QPushButton>
#include "QHexSpinBox.h"

X502AsyncIoDialog::X502AsyncIoDialog(QWidget *parent, const QSharedPointer<LQMeas::x502>& dev) :
    QDialog(parent),
	m_dev(dev), ui(new Ui::X502AsyncIoDialog),
	m_out_async_update(false) {
    ui->setupUi(this);

    setWindowTitle(windowTitle() + ". " + dev->devStrFull());

    int pos_cnt = 18;
    for (int i = 0; i< dev->devTypeInfo()->out()->outDigChannelsCnt(); i++) {
        addOutPin(pos_cnt-i-1, i, QString("DO%1").arg(QString::number(i+1)));
    }

    addInPin(0, 17, QString("DI_SYN2"));
    addInPin(1, 16, QString("DI_SYN1"));
    //addInPin(2, 15, QString("DI16\nSTART_IN"));
    //addInPin(3, 14, QString("DI15\nCONV_IN"));
    //addInPin(4, 13, QString("DI14\nDI_SYN2"));
    for (int i = 0; i < 16; i++) {
        addInPin(pos_cnt-i-1, i, QString("DI %1").arg(QString::number(i+1)));
    }

    connect(ui->asyncInBtn, SIGNAL(clicked()), SLOT(asyncIn()));
    connect(ui->asyncOutBtn, SIGNAL(clicked()), SLOT(asyncOut()));
    connect(ui->asyncOutHexVal, SIGNAL(editingFinished()), SLOT(asyncOutHexChanged()));

    QSettings set;
    set.beginGroup("X502AsyncIoDialog");
    updateOutIndicators(set.value("AsyncOutDigVal").toInt());


    if (m_dev->devTypeInfo()->out()->outDacAsyncSupport()) {
        for (int i = 0; i < m_dev->devTypeInfo()->out()->outDacChannelsCnt(); i++) {
            addDac(i);
        }
    }
}

X502AsyncIoDialog::~X502AsyncIoDialog() {
    QSettings set;
    set.beginGroup("X502AsyncIoDialog");
    set.setValue("AsyncOutDigVal", m_last_out_val);
    for (const auto & m_dacState : m_dacStates) {
        set.setValue(QString("AsyncOutDacVal%1").arg(QString::number(m_dacState.dac_ch)),
                                                  m_dacState.valBox->value());
    }
    set.endGroup();

    delete ui;
}

void X502AsyncIoDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void X502AsyncIoDialog::asyncOut() {
	LQMeas::Error err = m_dev->outAsyncDigAll(static_cast<unsigned>(m_last_out_val));
    if (!err.isSuccess()) {
        QMessageBox::critical(this, tr("Write digital outputs"), m_dev->devStr() + ": " + "Write digital outputs error" +
							  QString(" (%1): %2").arg(QString::number(err.errorCode()), err.msg()));
    }
}

void X502AsyncIoDialog::asyncIn() {
    unsigned val;
    LQMeas::Error err = m_dev->inAsyncDig(&val);
    if (err.isSuccess()) {
        for (auto m_inState : m_inStates) {
            m_inState.led->setChecked(val & (1 << m_inState.pin_num));
        }
        ui->asyncInHexEdit->setText(QString("0x%1").arg(val, 5, 16, QChar('0')));
    } else {
        QMessageBox::critical(this, tr("Read digital inputs"), m_dev->devStr() + ": " + "Read digital inputs error" +
							  QString(" (%1): %2").arg(QString::number(err.errorCode()), err.msg()));
    }
}

void X502AsyncIoDialog::asyncOutHexChanged() {
    if (!m_out_async_update) {
        updateOutIndicators(ui->asyncOutHexVal->value());
    }
}

void X502AsyncIoDialog::asyncOutPinChanged() {
    if (!m_out_async_update) {
		signed out_val = 0;
        for (auto m_outState : m_outStates) {
            if (m_outState.led->isChecked()) {
                out_val |= (1 << m_outState.pin_num);
            }
        }
        updateOutIndicators(out_val);
    }
}

void X502AsyncIoDialog::asyncOutDacPressed() {
    auto *btn = qobject_cast<QPushButton *>(sender());
    if (btn) {
        for (const auto & m_dacState : m_dacStates) {
            if (m_dacState.btn == btn) {
                LQMeas::Error err = m_dev->devOutAsyncDac()->outAsyncDac(
                            m_dacState.dac_ch, m_dacState.valBox->value());
                if (!err.isSuccess()) {
                    QMessageBox::critical(this, tr("Write DAC value"), m_dev->devStr() + ": " + "Write DAC value error" +
										  QString(" (%1): %2").arg(QString::number(err.errorCode()), err.msg()));
                }
            }
        }
    }
}

void X502AsyncIoDialog::addOutPin(int pos, int pin_num, const QString& name) {
    auto *ind = new QLedIndicator();
    AsyncPinState state(pin_num, ind);

    int col = 2 + pos;
    ui->asyncOutGrid->addWidget(new QLabel(name), 0, col, 1, 1, Qt::AlignHCenter);
    ui->asyncOutGrid->addWidget(ind, 1, col);

	connect(ind, SIGNAL(toggled(bool)), SLOT(asyncOutPinChanged()));

    m_outStates.append(state);
}

void X502AsyncIoDialog::addInPin(int pos, int pin_num, const QString& name) {
    auto *ind = new QLedIndicator();
    ind->setEnabled(false);
    AsyncPinState state(pin_num, ind);

    int col = 2 + pos;
    QLabel *lab = new QLabel(name);
    ui->asyncOutGrid->addWidget(lab, 3, col, 1, 1, Qt::AlignHCenter);
    ui->asyncOutGrid->addWidget(ind, 4, col);

    m_inStates.append(state);
}

void X502AsyncIoDialog::addDac(int dac_ch) {
    int row = m_dacStates.size();
    const LQMeas::DevOutInfo *outInfo = m_dev->devTypeInfo()->out();
	/* Асинхронный выход разрешен, только если на этот канал не выводятся сейчас данные синхронно */
    bool en = !m_dev->devOutSync() || !m_dev->devOutSync()->outSyncGenRunning() ||
               (outInfo->outDacSyncModeCfgPerCh() && !m_dev->devOutSync()->outSyncConfig()->outSyncDacChEnabled(dac_ch));

    AsyncDacState state{};
    state.dac_ch = dac_ch;
    state.btn = new QPushButton(tr("Output to DAC%1").arg(QString::number(dac_ch+1)));
    state.btn->setEnabled(en);
    state.valBox = new QDoubleSpinBox();
    state.valBox->setSuffix(QString(" %1").arg(outInfo->outDacUnit(dac_ch)->name()));
    state.valBox->setMaximum(outInfo->outDacMaxVal(dac_ch));
    state.valBox->setMinimum(outInfo->outDacMinVal(dac_ch));
    state.valBox->setDecimals(3);
    state.valBox->setEnabled(en);
    ui->asyncDacLout->addWidget(state.btn, row, 0);
    ui->asyncDacLout->addWidget(state.valBox, row, 1);
    m_dacStates.append(state);
    if (en) {
        connect(state.btn, SIGNAL(clicked()), SLOT(asyncOutDacPressed()));
    }

    QSettings set;
    set.beginGroup("X502AsyncIoDialog");
    state.valBox->setValue(set.value(QString("AsyncOutDacVal%1").arg(QString::number(state.dac_ch))).toDouble());
    set.endGroup();
}

void X502AsyncIoDialog::updateOutIndicators(signed val) {
    m_out_async_update = true;
    m_last_out_val = val;
    for (auto m_outState : m_outStates) {
        m_outState.led->setChecked(val & (1 << m_outState.pin_num));
    }

    ui->asyncOutHexVal->setValue(val);
    m_out_async_update = false;

}
