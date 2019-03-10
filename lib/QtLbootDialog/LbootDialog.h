#ifndef LBOOTDIALOG_H
#define LBOOTDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QProcess>

namespace Ui {
class LbootDialog;
}

class LbootDialog : public QDialog {
    Q_OBJECT

public:
    explicit LbootDialog(QWidget *parent, QStringList args, unsigned finishWaitTime=0);
    ~LbootDialog();

protected:
    virtual void showEvent ( QShowEvent * event );
private slots:
    void onProcFinished(int exitCode, QProcess::ExitStatus);
    void onProcDataReady();
    void onError(QProcess::ProcessError);
    void on_btnDetails_clicked();

private:

    void setOutTextVisible(bool visible);

    static const int firm_write_start_perc = 5;
    static const int firm_write_end_perc   = 90;

    Ui::LbootDialog *ui;
    QProcess *m_lbootProcess;
    bool m_started;
    QStringList m_args;
    unsigned m_finishWaitTime;
};

#endif // LBOOTDIALOG_H
