#include "ConfigPasswordDialog.h"
#include "ui_ConfigPasswordDialog.h"

ConfigPasswordDialog::ConfigPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigPasswordDialog) {
    ui->setupUi(this);
}

ConfigPasswordDialog::~ConfigPasswordDialog() {
    delete ui;
}

QString ConfigPasswordDialog::passwd() const {
    return ui->passEdit->text();
}
