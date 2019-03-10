#ifndef IPADDRCONFIGDIALOG_H
#define IPADDRCONFIGDIALOG_H

#include <QDialog>


namespace Ui {
class IpAddrConfigDialog;
}

namespace LQMeasStudio {
    class IpAddrConfig;
}

class IpAddrConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit IpAddrConfigDialog(LQMeasStudio::IpAddrConfig *cfg, QWidget *parent = 0);
    ~IpAddrConfigDialog();

private slots:
    void addIpAddr();
    void remIpAddr();

    void updateActionEn();
private:
    Ui::IpAddrConfigDialog *ui;
    LQMeasStudio::IpAddrConfig *m_cfg;
};

#endif // IPADDRCONFIGDIALOG_H
