#include "DacSignalDialog.h"
#include "ui_DacSignalDialog.h"
#include "lqmeas/ifaces/out/DevOutSyncConfig.h"
#include "lqmeas/ifaces/out/DevOutInfo.h"

#include "DacSignalParams/DacSignalParamUiSin.h"
#include "DacSignalParams/DacSignalParamUiConst.h"
#include "DacSignalParams/DacSignalParamUiPulse.h"
#include "DacSignalParams/DacSignalParamUiRamp.h"

DacSignalDialog::DacSignalDialog(QWidget *parent, const LQMeas::DevOutInfo *info, LQMeas::DevOutSyncConfig *cfg, int ch) :
    QDialog(parent), m_info(info), m_cfg(cfg), m_ch(ch), m_curSigUi(0),

    m_curSignal(cfg->outSyncDacSignal(ch)),
    ui(new Ui::DacSignalDialog) {

    ui->setupUi(this);
    int set_idx = -1;

    m_sigUi << new DacSignalParamUiConst() << new DacSignalParamUiSin() <<
               new DacSignalParamUiPulse() << new DacSignalParamUiRamp();


    m_const_item_cnt = ui->paramsLout->rowCount();

    foreach (DacSignalParamUi *paramUI, m_sigUi) {
        if (m_curSignal && (m_curSignal->typeName() == paramUI->typeName())) {
            paramUI->setSignal(m_curSignal);
            set_idx = ui->signalTypeBox->count();
        }
        ui->signalTypeBox->addItem(paramUI->showName(), paramUI->typeName());
    }

    if (set_idx < 0)
        set_idx = 0;

    ui->signalTypeBox->setCurrentIndex(set_idx);

    setUiByIdx(set_idx);
    connect(ui->signalTypeBox, SIGNAL(currentIndexChanged(int)), SLOT(setUiByIdx(int)));

    if (info->outSyncPresetSupport()) {
        ui->presetType->addItem(tr("First signal point"), LQMeas::DevOutSyncConfig::DacPreset::TypeFirstPoint);
        ui->presetType->addItem(tr("Specified value"), LQMeas::DevOutSyncConfig::DacPreset::TypeSpecValue);
        ui->presetType->addItem(tr("Don't change"), LQMeas::DevOutSyncConfig::DacPreset::TypeDontChange);

        LQMeas::DevOutSyncConfig::DacPreset preset = cfg->outSyncDacPreset(ch);

        set_idx = 0;
        for (int i = 0; i < ui->presetType->count(); i++) {
            if (preset.type == ui->presetType->itemData(i).toInt()) {
                set_idx = i;
            }
        }
        ui->presetType->setCurrentIndex(set_idx);



        ui->presetValue->setMinimum(info->outDacMinVal(ch));
        ui->presetValue->setMaximum(info->outDacMaxVal(ch));
        ui->presetValue->setSuffix(" " + info->outDacUnit(ch)->name());
        ui->presetValue->setValue(preset.value);

        updatePresetValState();
        connect(ui->presetType, SIGNAL(currentIndexChanged(int)), SLOT(updatePresetValState()));
    } else {
        ui->presetGroup->setVisible(false);
    }
}

DacSignalDialog::~DacSignalDialog() {
    qDeleteAll(m_sigUi);
    delete ui;
}

QSharedPointer<LQMeas::DacSignal> DacSignalDialog::getSignal() {
    return m_curSigUi ? m_curSigUi->createSignal() : QSharedPointer<LQMeas::DacSignal>();
}

void DacSignalDialog::accept() {
    m_cfg->outSyncDacSetSignal(m_ch, getSignal());

    if (m_info->outSyncPresetSupport()) {
        LQMeas::DevOutSyncConfig::DacPreset preset;
        preset.type = (LQMeas::DevOutSyncConfig::DacPreset::Type)
                ui->presetType->itemData(ui->presetType->currentIndex()).toInt();
        preset.value = ui->presetValue->value();
        m_cfg->outSyncDacSetPreset(m_ch, preset);
    }
    QDialog::accept();
}

void DacSignalDialog::setUiByIdx(int idx) {
    if (m_curSigUi) {
        m_curSigUi->freeUi();
        for (int pos = ui->paramsLout->rowCount()-1; pos >= m_const_item_cnt; pos--) {
            removeRow(pos);
        }
        m_curSigUi = 0;
    }

    if (idx >=0) {
        QString typeName = ui->signalTypeBox->itemData(idx).toString();
        foreach (DacSignalParamUi *paramUI, m_sigUi) {
            if (typeName == paramUI->typeName()) {
                m_curSigUi = paramUI;
                m_curSigUi->createUi(m_info, m_ch, ui->paramsLout);
            }
        }
    }
}

void DacSignalDialog::updatePresetValState() {
    ui->presetValue->setEnabled(ui->presetType->itemData(ui->presetType->currentIndex()).toInt()
                                == LQMeas::DevOutSyncConfig::DacPreset::TypeSpecValue);
}

void DacSignalDialog::removeRow(int row) {
    QFormLayout::ItemRole roles[] = {QFormLayout::FieldRole, QFormLayout::LabelRole, QFormLayout::SpanningRole};

    for (int role_ind = 0; role_ind < sizeof(roles)/sizeof(roles[0]); role_ind++) {
        QLayoutItem* wgtItem = ui->paramsLout->itemAt ( row, roles[role_ind] );
        if (wgtItem){
            QWidget *wgt = wgtItem->widget();
            ui->paramsLout->removeWidget(wgt);
            ui->paramsLout->removeItem(wgtItem);
            delete wgt;
        }
    }
}
