#include "IpAddrConfigDialog.h"
#include "ui_IpAddrConfigDialog.h"

#include <QTableView>
#include <QAction>
#include <QToolButton>

#include "IpAddrAddDialog.h"
#include "IpAddrConfig.h"

IpAddrConfigDialog::IpAddrConfigDialog(LQMeasStudio::IpAddrConfig *cfg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpAddrConfigDialog),
    m_cfg(cfg) {

    ui->setupUi(this);

    connect(ui->actionAddIpAddr, SIGNAL(triggered()), SLOT(addIpAddr()));
    connect(ui->actionRemIPAddr, SIGNAL(triggered()), SLOT(remIpAddr()));

    ui->ipAddrTable->setModel(cfg);
    ui->ipAddrTable->addAction(ui->actionAddIpAddr);
    ui->ipAddrTable->addAction(ui->actionRemIPAddr);

    ui->ipAddrTable->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->addAddr->setDefaultAction(ui->actionAddIpAddr);
    ui->remAddr->setDefaultAction(ui->actionRemIPAddr);


    connect(ui->ipAddrTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(updateActionEn()));

    updateActionEn();


}

IpAddrConfigDialog::~IpAddrConfigDialog() {
    delete ui;
}

void IpAddrConfigDialog::addIpAddr() {
    IpAddrAddDialog dialog(m_cfg->supportedDevs(), this);
    if (dialog.exec()==QDialog::Accepted) {
        m_cfg->addRecord(dialog.devname(), dialog.addr());
    }
}

void IpAddrConfigDialog::remIpAddr() {
    int idx = ui->ipAddrTable->currentIndex().row();
    if (idx >= 0) {
        m_cfg->remRecord(idx);
    }
}

void IpAddrConfigDialog::updateActionEn() {
    bool is_val = ui->ipAddrTable->currentIndex().isValid();
    int row = ui->ipAddrTable->currentIndex().row();
    ui->actionRemIPAddr->setEnabled(is_val);
}
