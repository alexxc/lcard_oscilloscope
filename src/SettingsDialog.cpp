#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QDoubleSpinBox>
#include "LQMeasStudio.h"
#include "proc/AdcBlockReceivePlugin.h"
//#include "proc/SpectrumPlugin.h"
#include "lqmeas/units/Units.h"

using namespace LQMeasStudio;

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog) {

    ui->setupUi(this);

    ui->procBlockTime->setSuffix(" " + LQMeas::Units::msec()->name());
    ui->procIntervaltime->setSuffix(" " + LQMeas::Units::msec()->name());


    AdcBlockReceivePlugin *blockReceiver = LQMeasStudioPlugin(AdcBlockReceivePlugin);
    if (blockReceiver) {
        ui->procBlockTime->setValue(blockReceiver->blockTime());
        ui->procIntervaltime->setValue(blockReceiver->blockInterval());
    } else {
        ui->procBlockTime->setEnabled(false);
        ui->procIntervaltime->setEnabled(false);
    }
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::accept() {
    AdcBlockReceivePlugin *blockReceiver = LQMeasStudioPlugin(AdcBlockReceivePlugin);
    if (blockReceiver) {
        blockReceiver->setBlockTime(ui->procBlockTime->value(),
                                    ui->procIntervaltime->value());
    }
    QDialog::accept();
}
