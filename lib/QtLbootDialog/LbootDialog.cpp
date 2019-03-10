#include "LbootDialog.h"
#include "ui_LbootDialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTime>
#include "LbootOutRecord.h"




LbootDialog::LbootDialog(QWidget *parent, QStringList args, unsigned finishWaitTime) :
    QDialog(parent), m_started(false), m_finishWaitTime(finishWaitTime),
    ui(new Ui::LbootDialog) {
    ui->setupUi(this);
    setOutTextVisible(false);


    args.prepend("--output-fmt=dsv");


    m_args = args;
    m_lbootProcess = new QProcess(this);

    connect(m_lbootProcess, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(onProcFinished(int,QProcess::ExitStatus)));
    connect(m_lbootProcess, SIGNAL(readyReadStandardOutput()), SLOT(onProcDataReady()));
    connect(m_lbootProcess, SIGNAL(error(QProcess::ProcessError)), SLOT(onError(QProcess::ProcessError)));

}

LbootDialog::~LbootDialog() {
    delete ui;
}

void LbootDialog::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    if (!m_started) {
        QString program = "lboot";
        m_lbootProcess->start(program, m_args);
    }
}

void LbootDialog::onProcFinished(int exitCode, QProcess::ExitStatus) {
    if (exitCode==0) {
        ui->bootProgress->setValue((firm_write_end_perc + 100)/2);
        QTime dieTime = QTime::currentTime().addMSecs( m_finishWaitTime );
        while( QTime::currentTime() < dieTime ) {
            QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        }

        ui->stageLbl->setText(LbootOutRecord::tr("Done successfully"));
        ui->bootProgress->setValue(100);
    }
    ui->buttonBox->setEnabled(true);
}

void LbootDialog::onProcDataReady() {
    while (m_lbootProcess->canReadLine()) {
        QString line = m_lbootProcess->readLine();
        line = line.trimmed();
        LbootOutRecord rec(line);
        if (rec.valid()) {
            if (rec.status() == LbootOutRecord::statusStart()) {
                ui->stageLbl->setText(rec.stageName());
            } else if (rec.status() == LbootOutRecord::statusProgress()) {
                if (rec.stage() == LbootOutRecord::stageWriteFirmware()) {
                    int cpl = rec.param(LbootOutRecord::paramCplSize()).toInt();
                    int size = rec.param(LbootOutRecord::paramFullSize()).toInt();
                    double progr = ((double)cpl/size)*(firm_write_end_perc-firm_write_start_perc)
                            + firm_write_start_perc;
                    ui->bootProgress->setValue(progr);
                }
            } else if (rec.status() == LbootOutRecord::statusError()) {
                QMessageBox::critical(this, tr("Error"),
                                      QString("%1 (%2): %3").
                                      arg(rec.errorTypeDescr()).
                                      arg(rec.param(LbootOutRecord::paramErrCode())).
                                      arg(rec.errorDescr()),
                                      QMessageBox::Ok, QMessageBox::Ok);
            }
        }

        ui->outputText->appendPlainText(line);
    }
}

void LbootDialog::onError(QProcess::ProcessError) {
    QMessageBox::critical(this, tr("Error"),
                          tr("Cannot execute lboot process"),
                          QMessageBox::Ok, QMessageBox::Ok);
    ui->buttonBox->setEnabled(true);
}

void LbootDialog::setOutTextVisible(bool visible) {
    ui->outputText->setVisible(visible);
    ui->btnDetails->setText(visible ? tr("Hide details...") : tr("Show details..."));
    qApp->processEvents();
    resize(width(), visible ? 450 : 20);
}




void LbootDialog::on_btnDetails_clicked() {
    setOutTextVisible(!ui->outputText->isVisible());
}
