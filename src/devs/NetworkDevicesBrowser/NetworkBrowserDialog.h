#ifndef NETWORKBROWSERDIALOG_H
#define NETWORKBROWSERDIALOG_H

#include <QDialog>

namespace Ui {
class NetworkBrowserDialog;
}

class NetworkBrowserDialog : public QDialog {
    Q_OBJECT

public:
    explicit NetworkBrowserDialog(QWidget *parent = 0);
    ~NetworkBrowserDialog();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_browseEnable_stateChanged(int arg1);

private:
    Ui::NetworkBrowserDialog *ui;
};

#endif // NETWORKBROWSERDIALOG_H
