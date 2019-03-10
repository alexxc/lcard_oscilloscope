#include "lcompDevSeqAdcCfgDialog.h"
#include "ui_lcompDevSeqAdcCfgDialog.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QComboBox>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>

lcompDevSeqAdcCfgDialog::lcompDevSeqAdcCfgDialog(QSharedPointer<lcompStreamDevice> dev,
                                                 QWidget *parent) :
    QDialog(parent), m_dev(dev),
    ui(new Ui::lcompDevSeqAdcCfgDialog) {

    ui->setupUi(this);


    bool change_enabled = !m_dev->adcIsRunning();

    lcompStreamDevice::AdcConfig cfg = dev->adcCurConfig();
    unsigned ch_cnt = dev->adcChannelsCnt();

    ui->adcChannels->setRowCount(ch_cnt);

    for (unsigned ch=0; ch < ch_cnt; ch++) {
        ChUi ch_ui;
        ch_ui.chItem = new QTableWidgetItem();
        ch_ui.chItem->setText(QString("Канал %1").arg(QString::number(ch+1)));
        ch_ui.chItem->setCheckState(cfg.Ch[ch].Enabled ? Qt::Checked : Qt::Unchecked);
        if (!change_enabled)
            ch_ui.chItem->setFlags(ch_ui.chItem->flags() &= ~Qt::ItemIsEnabled);


        ch_ui.mode = new QComboBox();
        ch_ui.mode->addItem(tr("С общей землей"), lcompStreamDevice::AdcConfig::ADC_MODE_COMM);
        if (ch < LCOMP_SEQ_DIFF_CH_CNT) {
            ch_ui.mode->addItem(tr("Дифф."), lcompStreamDevice::AdcConfig::ADC_MODE_DIFF);
        }
        ch_ui.mode->addItem(tr("Изм. нуля"), lcompStreamDevice::AdcConfig::ADC_MODE_ZERO);
        ch_ui.mode->setEnabled(change_enabled);

        ch_ui.range = new QComboBox();
        for (unsigned int r = 0; r < m_dev->adcRangesCnt(); r++) {
            ch_ui.range->addItem("± " + QString::number(m_dev->adcRangeMaxVal(r)) + tr(" В"), r);
        }
        ch_ui.range->setEnabled(change_enabled);

        setComboBoxItemByData(ch_ui.mode,  cfg.Ch[ch].Mode);
        setComboBoxItemByData(ch_ui.range, cfg.Ch[ch].Range);

        ui->adcChannels->setItem(ch, 0, ch_ui.chItem);
        ui->adcChannels->setCellWidget(ch, 1, ch_ui.mode);
        ui->adcChannels->setCellWidget(ch, 2, ch_ui.range);

        m_chUis.append(ch_ui);
    }

    ui->adcChannels->resizeColumnsToContents();

    ui->adcFreq->setValue(cfg.FreqAdc);
    ui->adcChFreq->setValue(cfg.FreqCh);

    ui->adcFreq->setReadOnly(!change_enabled);
    ui->adcChFreq->setReadOnly(!change_enabled);


    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(change_enabled);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(change_enabled);
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
            SLOT(setConfig()));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            SLOT(reject()));
}

lcompDevSeqAdcCfgDialog::~lcompDevSeqAdcCfgDialog() {
    delete ui;
}

void lcompDevSeqAdcCfgDialog::accept() {
    if (setConfig().isSuccess())
        QDialog::accept();
}

LQMeasError lcompDevSeqAdcCfgDialog::setConfig() {
    LQMeasError err = LQMeasError::Success();
    lcompStreamDevice::AdcConfig cfg = m_dev->adcCurConfig();
    unsigned ch_cnt = m_dev->adcChannelsCnt();

    for (unsigned ch=0; ch < ch_cnt; ch++) {
        cfg.Ch[ch].Enabled = m_chUis[ch].chItem->checkState() == Qt::Checked;
        cfg.Ch[ch].Mode    = (lcompStreamDevice::AdcConfig::ADC_MODE)m_chUis[ch].mode->itemData(m_chUis[ch].mode->currentIndex()).toInt();
        cfg.Ch[ch].Range   = m_chUis[ch].range->itemData(m_chUis[ch].range->currentIndex()).toInt();

    }

    cfg.FreqAdc = ui->adcFreq->value();
    cfg.FreqCh = ui->adcChFreq->value();

    err = m_dev->adcSetConfig(cfg);
    if (err.isSuccess())
        err = m_dev->configure();
    if (err.isSuccess()) {
        ui->adcFreq->setValue(m_dev->adcFreq());
        ui->adcChFreq->setValue(m_dev->adcChFreq());
    } else {
        QMessageBox::critical(this, tr("Ошибка настройки АЦП"), err.msg());
    }
    return err;
}

void lcompDevSeqAdcCfgDialog::setComboBoxItemByData(QComboBox *box, int data) {
    int idx = 0;
    for (int i=0; i < box->count(); i++) {
        if (box->itemData(i).toInt()==data) {
            idx = i;
            break;
        }
    }
    box->setCurrentIndex(idx);
}
