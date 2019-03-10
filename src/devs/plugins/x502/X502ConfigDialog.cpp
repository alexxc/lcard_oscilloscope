#include "X502ConfigDialog.h"
#include "ui_X502ConfigDialog.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include "QLedIndicator.h"
#include "X502AsyncIoDialog.h"
#include "ComboBoxNoWheel.h"
#include "SpinBoxNoWheel.h"
#include "out/OutSignalConfigCtl.h"

#include "lqmeas/devs/x502/modules/E502/E502.h"
#include "lqmeas/devs/x502/modules/E502/E502Info.h"
#include "lqmeas/devs/x502/modules/L502/L502.h"
#include "lqmeas/devs/x502/modules/L502/L502Info.h"
#include "lqmeas/devs/x502/x502Info.h"
#include "lqmeas/units/Units.h"

#include "BlackfinFirmwareSelection.h"

X502ConfigDialog::X502ConfigDialog(QWidget *parent, const QSharedPointer<LQMeas::x502>& dev) :
    QDialog(parent),
    ui(new Ui::X502ConfigDialog),
    m_dev(dev) {

    bool change_enabled = !m_dev->adcIsRunning();

    ui->setupUi(this);

    setWindowTitle(windowTitle() + ". " + dev->devStrFull());

    ui->serial->setText(dev->serial());

	const LQMeas::x502Info *info = dev->devSpecInfo();
    ui->pldaVer->setText(info->pldaVerStr());
    ui->fpgaVer->setText(info->fpgaVerStr());
    if (info->mcuPresent()) {
        ui->armVer->setText(info->mcuVerStr());
    } else {
        ui->armVer->setVisible(false);
        ui->armVerLbl->setVisible(false);
    }

    addOption(tr("DAC"), info->dacPresent());
    addOption(tr("Galvanic decoupling"), info->galPresent());
    addOption(tr("BlackFin"), info->bfPresent());
    if (info->devspecTypeInfo()->deviceTypeName() == LQMeas::E502TypeInfo::name()) {
        addOption(tr("Ethernet"), info->ethPresent());
        addOption(tr("Industrial"), info->industrial());
    }

	unsigned ch_cnt = info->type()->adc()->adcChannelsCnt();
	m_cfg = QSharedPointer<LQMeas::x502Config>(dynamic_cast<LQMeas::x502Config*>(m_dev->devSpecConfig()->clone()));

	ui->adcChannels->setRowCount(static_cast<signed>(ch_cnt));

	for (unsigned ch=0; ch < ch_cnt; ++ch) {
        ChUi ch_ui{};
        fillChUi(ui->adcChannels, ch, &ch_ui, tr("Channel %1").arg(QString::number(ch+1)), change_enabled);

        ch_ui.chItem->setCheckState(m_cfg->adcChEnabled(ch) ? Qt::Checked : Qt::Unchecked);
        setComboBoxItemByData(ch_ui.mode,  m_cfg->adcChMode(ch));
        setComboBoxItemByData(ch_ui.range, m_cfg->adcChRangeNum(ch));
		ch_ui.avg->setChecked(m_cfg->adcChSoftAvg(ch));

        m_chUis.append(ch_ui);
    }

    ui->adcAllChannels->setRowCount(1);
    ui->adcAllChannels->setColumnCount(ui->adcChannels->columnCount());
    fillChUi(ui->adcAllChannels, 0, &m_allChUi, tr("All Channels"), change_enabled);
	m_allChUi.avg->setTristate(true);

    ui->adcChannels->resizeColumnToContents(1);
    ui->adcChannels->resizeColumnToContents(2);
    ui->adcChannels->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	for (int i = 0; i < ui->adcChannels->columnCount(); ++i) {
        ui->adcAllChannels->horizontalHeader()->resizeSection(i, ui->adcChannels->horizontalHeader()->sectionSize(i));
    }

    ui->adcFreq->setValue(m_cfg->adcFreq());
    ui->adcChFreq->setValue(m_cfg->adcChFreq());
    if (m_cfg->adcUseMaxChFreq()) {
        ui->adcMaxChFreqBox->setCheckState(Qt::Checked);
        ui->adcChFreq->setEnabled(false);
    } else {
        ui->adcMaxChFreqBox->setCheckState(Qt::Unchecked);
        ui->adcChFreq->setEnabled(true);
    }
    connect(ui->adcMaxChFreqBox, SIGNAL(stateChanged(int)), SLOT(adcMaxInChFreqChanged(int)));

    ui->adcFreq->setReadOnly(!change_enabled);
    ui->adcChFreq->setReadOnly(!change_enabled);
    ui->adcMaxChFreqBox->setEnabled(change_enabled);

    QSettings set;
    set.beginGroup("X502CfgDialog");
    ui->adcFreq->loadUnitSettings(set, "AdcFreqUnit");
    ui->outFreq->loadUnitSettings(set, "OutFreqUnit");
    ui->extRefFreq->loadUnitSettings(set, "extRefFreqUnit");
    resize(set.value("WidgetSize", QSize(1000, 800)).toSize());
    set.endGroup();

    ui->adcChFreq->setUnitMultCoef(ui->adcFreq->unitMultCoef());

    if (change_enabled) {
        ui->adcChFreq->setUnitReadOnly(true);
        connect(ui->adcFreq, SIGNAL(unitChanged(int)), ui->adcChFreq, SLOT(setUnitMultCoef(int)));
    }


    fillSyncBox(ui->syncFreqMode, m_cfg->freqMode());
    fillSyncBox(ui->syncStartMode, m_cfg->startMode());

    ui->syncFreqMode->setEnabled(change_enabled);
    ui->syncStartMode->setEnabled(change_enabled);


    ui->intRefFreq->addItem("2.0 " + LQMeas::Units::MHz()->name(), X502_REF_FREQ_2000KHZ);
    ui->intRefFreq->addItem("1.5 " + LQMeas::Units::MHz()->name(), X502_REF_FREQ_1500KHZ);
    t_x502_ref_freq freq = m_cfg->refFreqInternal();
	setComboBoxItemByData(ui->intRefFreq, static_cast<int>(freq));
    ui->intRefFreq->setEnabled(change_enabled);

    ui->extRefFreq->setReadOnly(!change_enabled);
    ui->extRefFreq->setValue(m_cfg->refFreqExternal());

    uint32_t pullups = m_cfg->pullups();


    if (dev->name() != LQMeas::L502TypeInfo::name()) {
        ui->pullLow->setVisible(false);
        ui->pullHigh->setVisible(false);
    } else {
        ui->pullLow->setChecked(pullups & X502_PULLUPS_DI_L);
        ui->pullHigh->setChecked(pullups & X502_PULLUPS_DI_H);

        ui->pullLow->setEnabled(change_enabled);
        ui->pullHigh->setEnabled(change_enabled);
    }

    if (dev->name() != LQMeas::E502TypeInfo::name()) {
        ui->pullCONV->setVisible(false);
        ui->pullSTART->setVisible(false);
    } else {
        ui->pullCONV->setChecked(pullups & X502_PULLDOWN_CONV_IN);
        ui->pullSTART->setChecked(pullups & X502_PULLDOWN_START_IN);

        ui->pullCONV->setEnabled(change_enabled);
        ui->pullSTART->setEnabled(change_enabled);
    }
    ui->pullSYN1->setChecked(pullups & X502_PULLUPS_DI_SYN1);
    ui->pullSYN2->setChecked(pullups & X502_PULLUPS_DI_SYN2);

    ui->pullSYN1->setEnabled(change_enabled);
    ui->pullSYN2->setEnabled(change_enabled);

    ui->outFreq->setValue(m_cfg->outSyncGenFreq());

    if (dev->supportCustomOutFreq() && info->dacPresent()) {
        ui->outFreq->setReadOnly(!change_enabled);        
    } else {
        ui->outFreq->setEnabled(false);
    }
    ui->outMode->addItem(tr("Cycle"), LQMeas::DevOutSyncConfig::OutSyncRamModeCycle);
    ui->outMode->addItem(tr("Stream"), LQMeas::DevOutSyncConfig::OutSyncRamModeStream);

    setComboBoxItemByData(ui->outMode, m_cfg->outSyncRamMode());

    ui->outMode->setEnabled(info->dacPresent() && change_enabled);

    new OutSignalConfigCtl(this, m_cfg,
                           dev->devTypeInfo()->out(),
                           QList<QCheckBox*>() << ui->dacCh1En << ui->dacCh2En,
                           QList<QLineEdit*>() << ui->dacCh1SigEdit << ui->dacCh2SigEdit,
                           change_enabled);

    ui->blackfinFirmFile->setText(m_cfg->bfFirmwareFilename());
    if (info->bfPresent()) {
        ui->enBlackfin->setChecked(m_cfg->bfIsEnabled());
    } else {
        ui->enBlackfin->setEnabled(false);
        ui->blackfinFirmFile->setEnabled(false);
    }
    new BlackfinFirmwareSelection(this, ui->enBlackfin, ui->blackfinFirmFile, info->bfPresent() && change_enabled);

    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(change_enabled);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(change_enabled);
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
            SLOT(setConfig()));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            SLOT(reject()));

    /* нельзя использовать каналы 17-32, если разрешены соответствующие
       каналы 1-16 в диф. режиме. Отслеживаем это динамически */
    if (change_enabled) {
		for (int ch=0; ch < m_chUis.size(); ++ch) {
			if (ch < X502_ADC_DIFF_CH_CNT) {
                updateCommChState(ch);
			}
            connect(m_chUis[ch].mode, SIGNAL(currentIndexChanged(int)), SLOT(chModeChanged()));
            connect(m_chUis[ch].range, SIGNAL(currentIndexChanged(int)), SLOT(chRangeChanged()));
			connect(m_chUis[ch].avg, SIGNAL(stateChanged(int)), SLOT(chAvgChanged(int)));
        }
		connect(ui->adcChannels, SIGNAL(cellChanged(int, int)), SLOT(cellDataChanged(int, int)));

		/* при изменении коэф. усреднения изменяем соответствующим образом макс.
		   частоту сбора */
		connect(ui->adcChannels, SIGNAL(cellChanged(int, int)), SLOT(setMaxAdcFreq()));
        connect(ui->intRefFreq, SIGNAL(currentIndexChanged(int)), SLOT(setMaxAdcFreq()));
        connect(ui->extRefFreq, SIGNAL(valueChanged(double)), SLOT(setMaxAdcFreq()));
        connect(ui->adcFreq, SIGNAL(valueChanged(double)), SLOT(setMaxAdcChFreq()));
        connect(ui->syncFreqMode, SIGNAL(currentIndexChanged(int)), SLOT(updateRefFreqElems()));
        connect(ui->syncFreqMode, SIGNAL(currentIndexChanged(int)), SLOT(setMaxAdcFreq()));

		connect(ui->adcAllChannels, SIGNAL(cellChanged(int, int)), SLOT(allChEnChanged(int, int)));
        connect(m_allChUi.mode, SIGNAL(currentIndexChanged(int)), SLOT(allChModeChanged()));
        connect(m_allChUi.range, SIGNAL(currentIndexChanged(int)), SLOT(allChRangeChanged()));
		connect(m_allChUi.avg, SIGNAL(stateChanged(int)), SLOT(allChAvgChanged(int)));
    }
    updateRefFreqElems();
    updateAllChState();
}

X502ConfigDialog::~X502ConfigDialog() {
    QSettings set;
    set.beginGroup("X502CfgDialog");
    set.setValue("WidgetSize", size());
    set.endGroup();
    delete ui;
}

void X502ConfigDialog::accept() {
    if (setConfig().isSuccess())
        QDialog::accept();
}

void X502ConfigDialog::setComboBoxItemByData(QComboBox *box, int data) {
    int idx = 0;
    for (int i=0; i < box->count(); i++) {
        if (box->itemData(i).toInt()==data) {
            idx = i;
            break;
        }
    }
    box->setCurrentIndex(idx);
}

void X502ConfigDialog::fillSyncBox(QComboBox *box, int value) {
    box->addItem(tr("Internal"), X502_SYNC_INTERNAL);
    box->addItem(tr("From external master"), X502_SYNC_EXTERNAL_MASTER);
    box->addItem(tr("SYN1 Rise"), X502_SYNC_DI_SYN1_RISE);
    box->addItem(tr("SYN2 Rise"), X502_SYNC_DI_SYN2_RISE);
    box->addItem(tr("SYN1 Fall"), X502_SYNC_DI_SYN1_FALL);
    box->addItem(tr("SYN2 Fall"), X502_SYNC_DI_SYN2_FALL);
    setComboBoxItemByData(box, value);
}

void X502ConfigDialog::addRangeItem(QComboBox *box, double val, unsigned code) {
    box->addItem("± " + QString::number(val) + " " + LQMeas::Units::V()->name(),  code);
}

void X502ConfigDialog::fillChUi(QTableWidget *table, unsigned int channel, X502ConfigDialog::ChUi *chUi, const QString& name, bool change_enabled) {
    chUi->chItem = new QTableWidgetItem();
    chUi->chItem->setText(name);
    if (!change_enabled)
        chUi->chItem->setFlags(chUi->chItem->flags() &= ~Qt::ItemIsEnabled);


    chUi->mode = new ComboBoxNoWheel();
    chUi->mode->addItem(tr("Comm. mode"), LQMeas::x502Config::AdcChModeComm);
	if (channel < X502_ADC_DIFF_CH_CNT) {
        chUi->mode->addItem(tr("Diff."), LQMeas::x502Config::AdcChModeDiff);
    }
    chUi->mode->addItem(tr("Zero"), LQMeas::x502Config::AdcChModeZero);
    chUi->mode->setEnabled(change_enabled);
#ifndef _WIN32
    chUi->mode->setStyleSheet("QComboBox { padding: 1px 18px 1px 0px; }");
#endif

    chUi->range = new ComboBoxNoWheel();
    addRangeItem(chUi->range, 10, X502_ADC_RANGE_10);
    addRangeItem(chUi->range, 5, X502_ADC_RANGE_5);
    addRangeItem(chUi->range, 2, X502_ADC_RANGE_2);
    addRangeItem(chUi->range, 1, X502_ADC_RANGE_1);
    addRangeItem(chUi->range, 0.5, X502_ADC_RANGE_05);
    addRangeItem(chUi->range, 0.2, X502_ADC_RANGE_02);
    chUi->range->setEnabled(change_enabled);
#ifndef _WIN32
    chUi->range->setStyleSheet("QComboBox { padding: 1px 18px 1px 0px; }");
#endif

	chUi->avg = new QCheckBox();
	chUi->avg->setEnabled(change_enabled);
#ifndef _WIN32
    chUi->avg->setStyleSheet("* { padding: 1px 18px 1px 0px; }");
#endif

	table->setItem(static_cast<signed>(channel), 0, chUi->chItem);
	table->setCellWidget(static_cast<signed>(channel), 1, chUi->mode);
	table->setCellWidget(static_cast<signed>(channel), 2, chUi->range);
	table->setCellWidget(static_cast<signed>(channel), 3, chUi->avg);
}

LQMeas::Error X502ConfigDialog::setConfig() {
    LQMeas::Error err;

	unsigned ch_cnt = m_dev->devTypeInfo()->adc()->adcChannelsCnt();

    /* сохраняем выбранные единицы измерения */
    QSettings set;
    set.beginGroup("X502CfgDialog");
    ui->adcFreq->saveUnitSettings(set, "AdcFreqUnit");
    ui->outFreq->saveUnitSettings(set, "OutFreqUnit");
    set.endGroup();


	for (unsigned ch=0; ch < ch_cnt; ch++) {
		m_cfg->adcSetChEnabled(ch, m_chUis[static_cast<signed>(ch)].chItem->checkState() == Qt::Checked);
		m_cfg->adcSetChMode(ch, static_cast<LQMeas::x502Config::AdcChMode>(m_chUis[static_cast<signed>(ch)].mode->itemData(m_chUis[static_cast<signed>(ch)].mode->currentIndex()).toInt()));
		m_cfg->adcSetChRangeNum(ch, m_chUis[static_cast<signed>(ch)].range->itemData(m_chUis[static_cast<signed>(ch)].range->currentIndex()).toInt());
		m_cfg->adcSetChSoftAvg(ch, m_chUis[static_cast<signed>(ch)].avg->isChecked());
    }

    m_cfg->adcSetFreq(ui->adcFreq->value());
    m_cfg->adcSetChFreq(ui->adcChFreq->value());
    m_cfg->adcSetUseMaxChFreq(ui->adcMaxChFreqBox->isChecked());


	m_cfg->setFreqMode(static_cast<t_x502_sync_mode>(ui->syncFreqMode->itemData(ui->syncFreqMode->currentIndex()).toInt()));
	m_cfg->setStartMode(static_cast<t_x502_sync_mode>(ui->syncStartMode->itemData(ui->syncStartMode->currentIndex()).toInt()));
	m_cfg->setRefFreqInternal(static_cast<t_x502_ref_freq>(ui->intRefFreq->itemData(ui->intRefFreq->currentIndex()).toInt()));
    m_cfg->setRefFreqExternal(ui->extRefFreq->value());
    m_cfg->setOutSyncGenFreq(ui->outFreq->value());
	m_cfg->outSyncSetRamMode(static_cast<LQMeas::DevOutSyncConfig::OutSyncRamMode>(ui->outMode->itemData(ui->outMode->currentIndex()).toInt()));

    unsigned pullups = 0;
    if (ui->pullLow->isChecked())
        pullups |= X502_PULLUPS_DI_L;
    if (ui->pullHigh->isChecked())
        pullups |= X502_PULLUPS_DI_H;
    if (ui->pullCONV->isChecked())
        pullups |= X502_PULLDOWN_CONV_IN;
    if (ui->pullSTART->isChecked())
        pullups |= X502_PULLDOWN_START_IN;
    if (ui->pullSYN1->isChecked())
        pullups |= X502_PULLUPS_DI_SYN1;
    if (ui->pullSYN2->isChecked())
        pullups |= X502_PULLUPS_DI_SYN2;
    m_cfg->setPullups(pullups);

    m_cfg->setBfEnalbed(ui->enBlackfin->isChecked());
    m_cfg->setBfFirmwareFilename(ui->blackfinFirmFile->text());

    m_cfg->updateConfig();

    err = m_dev->configure(m_cfg.data());

    if (err.isSuccess()) {
        ui->adcFreq->setValue(m_cfg->adcFreq());
        setMaxAdcChFreq();
        ui->adcChFreq->setValue(m_cfg->adcChFreq());
        ui->outFreq->setValue(m_cfg->outSyncGenFreq());
    } else {
        QMessageBox::critical(this, tr("ADC configuration error"), err.msg());
    }
    return err;
}

void X502ConfigDialog::updateRefFreqElems() {
	t_x502_sync_mode sync_mode = static_cast<t_x502_sync_mode>(ui->syncFreqMode->itemData(ui->syncFreqMode->currentIndex()).toInt());

    ui->extRefFreq->setVisible(sync_mode != X502_SYNC_INTERNAL);
    ui->extRefFreqLbl->setVisible(sync_mode != X502_SYNC_INTERNAL);

    ui->intRefFreq->setVisible(sync_mode == X502_SYNC_INTERNAL);
    ui->intRefFreqLbl->setVisible(sync_mode == X502_SYNC_INTERNAL);
}

void X502ConfigDialog::adcMaxInChFreqChanged(int val) {
    ui->adcChFreq->setEnabled(val != Qt::Checked);
}

void X502ConfigDialog::chModeChanged() {
    auto *modebox = qobject_cast<QComboBox *>(sender());
    for (int i=0; i < X502_ADC_DIFF_CH_CNT; i++) {
        if (m_chUis[i].mode == modebox) {
            updateCommChState(i);
            break;
        }
    }
    updateAllChState();
}

void X502ConfigDialog::chRangeChanged() {
    updateAllChState();
}

void X502ConfigDialog::chAvgChanged(int) {
    updateAllChState();
}

void X502ConfigDialog::updateCommChState(int i) {
    bool com_ch_en = (m_chUis[i].chItem->checkState() != Qt::Checked) ||
            (m_chUis[i].mode->itemData(m_chUis[i].mode->currentIndex()).toInt() != X502_LCH_MODE_DIFF);
    QTableWidgetItem *com_ch_item = m_chUis[i + X502_ADC_DIFF_CH_CNT].chItem;
    if (com_ch_en) {
        com_ch_item->setFlags(com_ch_item->flags() | Qt::ItemIsEnabled);
    } else {
        com_ch_item->setFlags(com_ch_item->flags() &= ~Qt::ItemIsEnabled);
        com_ch_item->setCheckState(Qt::Unchecked);
    }
    m_chUis[i + X502_ADC_DIFF_CH_CNT].avg->setEnabled(com_ch_en);
    m_chUis[i + X502_ADC_DIFF_CH_CNT].mode->setEnabled(com_ch_en);
    m_chUis[i + X502_ADC_DIFF_CH_CNT].range->setEnabled(com_ch_en);


}

void X502ConfigDialog::cellDataChanged(int row, int col) {
    if (col == 0) {
        if (row < X502_ADC_DIFF_CH_CNT)
            updateCommChState(row);
        updateAllChState();
    }
}

void X502ConfigDialog::setMaxAdcFreq() {
	double ref_freq;
	t_x502_sync_mode sync_mode = static_cast<t_x502_sync_mode>(ui->syncFreqMode->itemData(ui->syncFreqMode->currentIndex()).toInt());
	if (sync_mode == X502_SYNC_INTERNAL) {
		ref_freq = ui->intRefFreq->itemData(ui->intRefFreq->currentIndex()).toDouble();
	} else {
		ref_freq = ui->extRefFreq->value();
	}

	ui->adcFreq->setMaximum(ref_freq);
	setMaxAdcChFreq();
}

void X502ConfigDialog::setMaxAdcChFreq() {

    int ch_cnt = 0;
    for (auto & m_chUi : m_chUis) {
        if (m_chUi.chItem->checkState() == Qt::Checked) {
			++ch_cnt;
        }
    }
    if (ch_cnt != 0) {
        ui->adcChFreq->setMaximum(ui->adcFreq->value()/ch_cnt);
		if (ui->adcMaxChFreqBox->isChecked()) {
            ui->adcChFreq->setValue(ui->adcFreq->value()/ch_cnt);
		}
    }

}

void X502ConfigDialog::updateAllChState() {
    if (m_allUpMutex.tryLock()) {
        Qt::CheckState en_state = m_chUis.first().chItem->checkState();
        int mode = m_chUis.first().mode->currentData().toInt();
        int range = m_chUis.first().range->currentData().toInt();
		Qt::CheckState avg = m_chUis.first().avg->checkState();

		for (auto & m_chUi : m_chUis) {
			if (m_chUi.chItem->flags() &  Qt::ItemIsEnabled) {
				if (en_state != m_chUi.chItem->checkState())
                    en_state = Qt::PartiallyChecked;
				if (mode != m_chUi.mode->currentData().toInt())
                    mode = -1;
				if (range != m_chUi.range->currentData().toInt())
                    range = -1;
				if (avg != m_chUi.avg->checkState())
					avg = Qt::CheckState::PartiallyChecked;
            }
        }

        m_allChUi.chItem->setCheckState(en_state);
        if (mode < 0) {
            m_allChUi.mode->setCurrentIndex(-1);
        } else {
            setComboBoxItemByData(m_allChUi.mode, mode);
        }
        if (range < 0) {
            m_allChUi.range->setCurrentIndex(-1);
        } else {
            setComboBoxItemByData(m_allChUi.range, range);
        }
		m_allChUi.avg->setCheckState(avg);
        m_allUpMutex.unlock();
    }
}

void X502ConfigDialog::allChEnChanged(int  /*row*/, int col) {
    if ((col == 0) && (m_allChUi.chItem->checkState() != Qt::PartiallyChecked)) {
        if (m_allUpMutex.tryLock()) {
            bool en = m_allChUi.chItem->checkState() == Qt::Checked;
            for (const auto & m_chUi : m_chUis) {
                if (m_chUi.chItem->flags() &  Qt::ItemIsEnabled) {
                    m_chUi.chItem->setCheckState(en ? Qt::Checked : Qt::Unchecked);
                }
            }
            m_allUpMutex.unlock();
            updateAllChState();
        }
    }
}

void X502ConfigDialog::allChModeChanged() {
    if (m_allUpMutex.tryLock()) {
        int mode = m_allChUi.mode->currentData().toInt();
        if (mode >= 0) {
            for (const auto & m_chUi : m_chUis) {
                if (m_chUi.chItem->flags() &  Qt::ItemIsEnabled) {
                    setComboBoxItemByData(m_chUi.mode, mode);
                }
            }
        }
        m_allUpMutex.unlock();
        allChEnChanged(0, 0);
    }
}

void X502ConfigDialog::allChRangeChanged() {
    if (m_allUpMutex.tryLock()) {
        int range = m_allChUi.range->currentData().toInt();
        if (range >= 0) {
            for (const auto & m_chUi : m_chUis) {
                setComboBoxItemByData(m_chUi.range, range);
            }
        }
        m_allUpMutex.unlock();
        updateAllChState();
    }
}

void X502ConfigDialog::allChAvgChanged(int val) {
    if (m_allUpMutex.tryLock()) {
        for (const auto & m_chUi : m_chUis) {
			if (val == Qt::Checked) {
				m_chUi.avg->setChecked(true);
			}
			else if(val == Qt::Unchecked) {
				m_chUi.avg->setChecked(false);
			}
        }
        m_allUpMutex.unlock();
        updateAllChState();
    }
}

void X502ConfigDialog::digLinesConfig() {
    /** @todo Реализовать диалог настройки синхронного вывода линий */
}

void X502ConfigDialog::addOption(const QString& text, bool val) {
    auto *led = new QLedIndicator(nullptr, 22);
    led->setEnabled(false);
    led->setChecked(val);
    ui->optionsLout->addRow(text, led);
}

