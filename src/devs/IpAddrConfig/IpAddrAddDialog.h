#ifndef IPADDRADDDIALOG_H
#define IPADDRADDDIALOG_H

#include <QDialog>
#include <QHostAddress>


namespace Ui {
class IpAddrAddDialog;
}

class IpAddrAddDialog : public QDialog {
    Q_OBJECT

public:
    explicit IpAddrAddDialog(const QStringList& devlist, QWidget *parent = 0);
    ~IpAddrAddDialog();

    QHostAddress addr() const;
    QString devname() const;

private:
    Ui::IpAddrAddDialog *ui;
private slots:
    void ipTextChanged(QString text);
};

#endif // IPADDRADDDIALOG_H
