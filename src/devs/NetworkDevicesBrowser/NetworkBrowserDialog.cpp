#include "NetworkBrowserDialog.h"
#include "ui_NetworkBrowserDialog.h"

#include "LQMeasStudio.h"
#include "NetworkBrowserModel.h"

NetworkBrowserDialog::NetworkBrowserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetworkBrowserDialog) {
    ui->setupUi(this);
    ui->browseEnable->setChecked(LQMeasStudioEnv->netBrowser()->browseIsEnabled());
    ui->devicesView->setModel(LQMeasStudioEnv->netBrowser());

}

NetworkBrowserDialog::~NetworkBrowserDialog() {
    delete ui;
}

void NetworkBrowserDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void NetworkBrowserDialog::on_browseEnable_stateChanged(int arg1) {
    LQMeasStudioEnv->netBrowser()->browseEnable(arg1 == Qt::Checked);
}
