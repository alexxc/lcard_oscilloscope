#include "WaitDialog.h"
#include "ui_WaitDialog.h"

WaitDialog::WaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint
                   | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

WaitDialog::~WaitDialog()
{
    delete ui;
}

void WaitDialog::setText(const QString& txt) {
    ui->label->setText(txt);
}
