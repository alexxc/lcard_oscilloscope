#ifndef DEVIFACECONFIGDIALOG_H
#define DEVIFACECONFIGDIALOG_H

#include <QDialog>

class QLineEdit;
class QCheckBox;
class QComboBox;

namespace Ui {
class DevIfaceConfigDialog;
}

class DevIfaceConfigDialog : public QDialog {
    Q_OBJECT
public:

    enum DevIfaces {
        DevIfaceUnknown = 0,
        DevIfaceUSB = 1,
        DevIfaceTCP = 2
    };


    struct Config {
        static const int MAC_ADDR_LEN = 6;

        bool eth_en;
        DevIfaces iface;                      /* Выбор интерфейса (CONF_IFACE_...) */
        quint32 ip_addr;
        quint32 ip_netmask;
        quint32 ip_gateway;
        bool    auto_ip_en;
        quint8  factory_mac[MAC_ADDR_LEN];
        bool    user_mac_en;
        quint8  user_mac[MAC_ADDR_LEN];
        QString instance_name;
    };

    enum FeaturesFlags {
        FlagPasswordRequired    = 0x00000001,
        FlagShowDeviceReset     = 0x00000002,
        FlagShowFactoryMac      = 0x00000004,
        FlagShowUserMac         = 0x00000008,
        FlagShowAutoIP          = 0x00000010,
        FlagShowSetPassword     = 0x00000020,
        FlagShowInterface       = 0x00000040,
        FlagShowEthEnable       = 0x00000080,
        FlagShowSvcInstanceName = 0x00000100
    } ;



    explicit DevIfaceConfigDialog(Config &cfg,  QWidget *parent, int flags);
    ~DevIfaceConfigDialog();



    DevIfaces devIface() const;

    quint32 ipAddr() const;
    quint32 ipMask() const;
    quint32 ipGate() const;

    QString     passwd() const;
    bool        ethIfaceEnabled() const;
    bool        autoIPEnabled() const;
    bool        userMacEnabled() const;
    QByteArray  userMac() const;

    bool        setNewPassReq() const;
    QString     newPass() const;
    QString     instanceName() const;


    static char macSeparator() {return ':';}
public slots:
    virtual void accept();
private slots:
    void dataChanged(void);
private:
    QString macStr(const quint8 *mac);

    bool m_modified;
    int m_flags;
    QString m_pass;
    Ui::DevIfaceConfigDialog *ui;

    QComboBox *devIfaceBox;


    QLineEdit *ipAddrEdit, *ipNetmaskEdit, *ipGateEdit;
    QLineEdit *newPassEdit, *newPassRepeatEdit;
    QLineEdit *userMacEdit, *factoryMacEdit;    
    QLineEdit *instanceNameEdit;
    QCheckBox *enableEthernetCheck, *userMacCheck, *autoIPEnCheck, *newPassSetCheck;
};

#endif // DEVIFACECONFIGDIALOG_H
