#include "IpAddrAddDialog.h"
#include "ui_IpAddrAddDialog.h"
#include <QRegExpValidator>
#include <QPushButton>
#include <QComboBox>

IpAddrAddDialog::IpAddrAddDialog(const QStringList& devlist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpAddrAddDialog) {

    ui->setupUi(this);

    ui->devType->addItems(devlist);

    QString ipNumReg = R"((25\d|2[0-4]{1,1}\d|[0-1]{0,1}\d{1,2}|\d{1,2}){1,1})";
    ui->ipAddrEdit->setValidator(new QRegExpValidator(
                                     QRegExp( ipNumReg + "\\." + ipNumReg + "\\."
                                              + ipNumReg + "\\." + ipNumReg), this));
    connect(ui->ipAddrEdit, SIGNAL(textChanged(QString)), SLOT(ipTextChanged(QString)));

    ipTextChanged(ui->ipAddrEdit->text());
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

IpAddrAddDialog::~IpAddrAddDialog() {
    delete ui;
}

QHostAddress IpAddrAddDialog::addr() const {
    return QHostAddress(ui->ipAddrEdit->text());
}

QString IpAddrAddDialog::devname() const {
    return ui->devType->currentText();
}

void IpAddrAddDialog::ipTextChanged(QString text) {
    int pos = 0;
    QPushButton* okBtn = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (ui->ipAddrEdit->validator()->validate(text, pos) == QValidator::Acceptable) {
        ui->ipAddrEdit->setStyleSheet("QLineEdit { color: darkGreen }");
        okBtn->setEnabled(true);
    } else {
        ui->ipAddrEdit->setStyleSheet("QLineEdit { color: darkRed }");
        okBtn->setEnabled(false);
    }
}
