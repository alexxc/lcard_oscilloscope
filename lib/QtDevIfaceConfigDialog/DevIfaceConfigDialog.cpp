#include "DevIfaceConfigDialog.h"
#include "ui_DevIfaceConfigDialog.h"

#include <QHostAddress>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include "ConfigPasswordDialog.h"
#include "SvcInstanceNameValidator.h"




DevIfaceConfigDialog::DevIfaceConfigDialog(Config &cfg,  QWidget *parent,
                                           int flags) :
    QDialog(parent),
    ui(new Ui::DevIfaceConfigDialog),
    m_modified(false), m_flags(flags) {

    ui->setupUi(this);

    bool addition_set_present = false;

    QString ipNumbReg = "(25\\d|2[0-4]{1,1}\\d|[0-1]{0,1}\\d{1,2}|\\d{1,2}){1,1}";
    QRegExpValidator* ipValidator = new QRegExpValidator(
                QRegExp( ipNumbReg + "\\." + ipNumbReg + "\\."
                         + ipNumbReg + "\\." + ipNumbReg), this);
    QString macNumbReg = "(\\d|[A-F]|[a-f]){2}";
    QString macRegexp  = "(" + macNumbReg + "\\" + macSeparator() + "){5}" + macNumbReg;
    QRegExpValidator* macValidator = new QRegExpValidator(
                QRegExp(macRegexp), this);

    if (m_flags & FlagShowEthEnable) {
        enableEthernetCheck = new QCheckBox();
        enableEthernetCheck->setChecked(cfg.eth_en);
        connect(enableEthernetCheck, SIGNAL(stateChanged(int)), SLOT(dataChanged()));
        ui->paramsLout->addRow(tr("Enable Ethernet interface"), enableEthernetCheck);
    }


    if (flags & FlagShowInterface) {
        devIfaceBox = new QComboBox();
        devIfaceBox->addItem(tr("USB"), DevIfaceUSB);
        devIfaceBox->addItem(tr("TCP/IP (Ethernet)"), DevIfaceTCP);
        devIfaceBox->addItem(tr("Unknown interface"), DevIfaceUnknown);
        ui->paramsLout->addRow(tr("Device interface"), devIfaceBox);


        int set_idx = -1;
        int def_idx = -1;
        for (int i=0; (set_idx < 0) && (i < devIfaceBox->count()); i++) {
            if (cfg.iface == devIfaceBox->itemData(i)) {
                set_idx = i;
            }
            if (devIfaceBox->itemData(i)==DevIfaceUnknown)
                def_idx = i;
        }
        devIfaceBox->setCurrentIndex(set_idx < 0 ? def_idx : set_idx);
        connect(devIfaceBox, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged()));
    }

    ipAddrEdit = new QLineEdit();
    ipAddrEdit->setValidator(ipValidator);
    ipAddrEdit->setText(QHostAddress(cfg.ip_addr).toString());
    connect(ipAddrEdit, SIGNAL(textChanged(QString)), SLOT(dataChanged()));
    ui->paramsLout->addRow(tr("Device IP address"), ipAddrEdit);


    ipNetmaskEdit = new QLineEdit();
    ipNetmaskEdit->setValidator(ipValidator);
    ipNetmaskEdit->setText(QHostAddress(cfg.ip_netmask).toString());
    connect(ipNetmaskEdit, SIGNAL(textChanged(QString)), SLOT(dataChanged()));
    ui->paramsLout->addRow(tr("IP subnet mask"), ipNetmaskEdit);


    ipGateEdit = new QLineEdit();
    ipGateEdit->setValidator(ipValidator);
    ipGateEdit->setText(QHostAddress(cfg.ip_gateway).toString());
    connect(ipGateEdit, SIGNAL(textChanged(QString)), SLOT(dataChanged()));
    ui->paramsLout->addRow(tr("IP Gateway"), ipGateEdit);


    if (flags & FlagShowAutoIP) {
        autoIPEnCheck = new QCheckBox();
        autoIPEnCheck->setChecked(cfg.auto_ip_en);
        ui->paramsLout->addRow(tr("Get IP-paramtes automatically"), autoIPEnCheck);
        connect(autoIPEnCheck, SIGNAL(stateChanged(int)), SLOT(dataChanged()));
    }

    if (flags & FlagShowSvcInstanceName) {
        instanceNameEdit = new QLineEdit();
        instanceNameEdit->setText(cfg.instance_name);
        instanceNameEdit->setValidator(new SvcInstanceNameValidator());
        ui->paramsLout->addRow(tr("Device instance name"), instanceNameEdit);
        connect(instanceNameEdit, SIGNAL(textChanged(QString)), SLOT(dataChanged()));
    }

    if (flags & FlagShowFactoryMac) {
        addition_set_present = true;
        factoryMacEdit = new QLineEdit();
        factoryMacEdit->setReadOnly(true);
        factoryMacEdit->setValidator(macValidator);
        QFont font = factoryMacEdit->font();
        font.setFamily("Monospace");
        font.setStyleHint(QFont::TypeWriter);
        factoryMacEdit->setFont(font);


        factoryMacEdit->setText(macStr(cfg.factory_mac));

        ui->additionalParamsLout->addRow(tr("Factory MAC-address"), factoryMacEdit);
    }

    if (flags & FlagShowUserMac) {
        addition_set_present = true;
        userMacCheck = new QCheckBox();
        userMacCheck->setChecked(cfg.user_mac_en);
        connect(userMacCheck, SIGNAL(stateChanged(int)), SLOT(dataChanged()));
        ui->additionalParamsLout->addRow(tr("Enable user MAC-address"), userMacCheck);

        userMacEdit = new QLineEdit();
        userMacEdit->setValidator(macValidator);
        QFont font = userMacEdit->font();
        font.setFamily("Monospace");
        font.setStyleHint(QFont::TypeWriter);
        userMacEdit->setFont(font);

        userMacEdit->setText(macStr(cfg.user_mac));
        connect(userMacEdit, SIGNAL(textChanged(QString)), SLOT(dataChanged()));
        ui->additionalParamsLout->addRow(tr("User MAC-address"), userMacEdit);
    }


    if (flags & FlagShowSetPassword) {
        addition_set_present = true;
        newPassSetCheck = new QCheckBox();
        newPassSetCheck->setChecked(false);
        ui->additionalParamsLout->addRow(tr("Set new password"), newPassSetCheck);
        connect(newPassSetCheck, SIGNAL(stateChanged(int)), SLOT(dataChanged()));


        newPassEdit = new QLineEdit();
        newPassEdit->setEchoMode(QLineEdit::Password);
        newPassEdit->setEnabled(false);
        ui->additionalParamsLout->addRow(tr("New password"), newPassEdit);
        connect(newPassEdit, SIGNAL(textChanged(QString)), SLOT(dataChanged()));


        newPassRepeatEdit = new QLineEdit();
        newPassRepeatEdit->setEchoMode(QLineEdit::Password);
        newPassRepeatEdit->setEnabled(false);
        ui->additionalParamsLout->addRow(tr("Repeat new password"), newPassRepeatEdit);
        connect(newPassRepeatEdit, SIGNAL(textChanged(QString)), SLOT(dataChanged()));
    }



    ui->additionalParams->setVisible(addition_set_present);


    dataChanged();
    resize(width(), 20);
    m_modified = false;
}

DevIfaceConfigDialog::~DevIfaceConfigDialog() {
    delete ui;
}

DevIfaceConfigDialog::DevIfaces DevIfaceConfigDialog::devIface() const {
	return m_flags &  FlagShowInterface ? static_cast<DevIfaces>(devIfaceBox->itemData(devIfaceBox->currentIndex()).toInt()) :
                                          DevIfaceUnknown;
}

quint32 DevIfaceConfigDialog::ipAddr() const {
    return  QHostAddress(ipAddrEdit->text()).toIPv4Address();
}

quint32 DevIfaceConfigDialog::ipMask() const {
    return QHostAddress(ipNetmaskEdit->text()).toIPv4Address();
}

quint32 DevIfaceConfigDialog::ipGate() const {
    return QHostAddress(ipGateEdit->text()).toIPv4Address();
}

QString DevIfaceConfigDialog::passwd() const {
    return m_pass;
}

bool DevIfaceConfigDialog::ethIfaceEnabled() const {
    return m_flags & FlagShowEthEnable ? enableEthernetCheck->isChecked() : true;
}

bool DevIfaceConfigDialog::autoIPEnabled() const {
    return m_flags & FlagShowAutoIP ? autoIPEnCheck->isChecked() : false;
}

bool DevIfaceConfigDialog::userMacEnabled() const {
    return m_flags & FlagShowUserMac ? userMacCheck->isChecked() : false;
}

QByteArray DevIfaceConfigDialog::userMac() const {
    QByteArray ret;
    QStringList macStr = userMacEdit->text().split(macSeparator());
    if (macStr.size() == Config::MAC_ADDR_LEN) {
		for (QString macByte : macStr) {
			ret.append(macByte.toInt(nullptr, 16) % 16);
        }
    }
    return ret;
}

bool DevIfaceConfigDialog::setNewPassReq() const {
    return m_flags & FlagShowSetPassword ? newPassSetCheck->isChecked() : false;
}

QString DevIfaceConfigDialog::newPass() const {
    return m_flags & FlagShowSetPassword ? newPassEdit->text() : QString();
}

QString DevIfaceConfigDialog::instanceName() const {
    return m_flags & FlagShowSvcInstanceName ? instanceNameEdit->text() : QString();
}


void DevIfaceConfigDialog::accept() {
    if (m_modified) {
        if (m_flags & FlagPasswordRequired) {
            ConfigPasswordDialog passDlg(this);
            if (passDlg.exec() == QDialog::Accepted) {
                m_pass = passDlg.passwd();
                QDialog::accept();
            }
        } else {
            if (QMessageBox::information(this, tr("Changes saving"),
                                         tr("Write new device settings?"),
                                         QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes) {
                QDialog::accept();
            } else {
                QDialog::reject();
            }
        }
    } else {
        QDialog::reject();
    }
}



void DevIfaceConfigDialog::dataChanged() {
    bool cfg_ok = true;
    int pos=0;
    m_modified = true;

    QString text = ipAddrEdit->text();
    if (ipAddrEdit->validator()->validate(text, pos) == QValidator::Acceptable) {
        ipAddrEdit->setStyleSheet("QLineEdit { color: darkGreen }");
    } else {
        ipAddrEdit->setStyleSheet("QLineEdit { color: darkRed }");
        cfg_ok = false;
    }

    text = ipNetmaskEdit->text();
    if (ipNetmaskEdit->validator()->validate(text, pos) == QValidator::Acceptable) {
        ipNetmaskEdit->setStyleSheet("QLineEdit { color: darkGreen }");
    } else {
        ipNetmaskEdit->setStyleSheet("QLineEdit { color: darkRed }");
        cfg_ok = false;
    }

    text = ipGateEdit->text();
    if (ipGateEdit->validator()->validate(text, pos) == QValidator::Acceptable) {
        ipGateEdit->setStyleSheet("QLineEdit { color: darkGreen }");
    } else {
        ipGateEdit->setStyleSheet("QLineEdit { color: darkRed }");
        cfg_ok = false;
    }



    if (m_flags & FlagShowInterface) {
        if (devIface()==DevIfaceUnknown)
            cfg_ok =  false;
    }


    if (m_flags & FlagShowUserMac) {
        text = userMacEdit->text();
        if (userMacEdit->validator()->validate(text, pos) == QValidator::Acceptable) {
            userMacEdit->setStyleSheet("QLineEdit { color: darkGreen }");
        } else {
            userMacEdit->setStyleSheet("QLineEdit { color: darkRed }");
            cfg_ok = false;
        }
    }

    if (m_flags & FlagShowSetPassword)  {
        bool setPass = newPassSetCheck->isChecked();

        if (setPass) {
            newPassEdit->setEnabled(true);
            newPassRepeatEdit->setEnabled(true);


            if (newPassEdit->text()!=newPassRepeatEdit->text()) {
                newPassRepeatEdit->setStyleSheet("QLineEdit { color: darkRed }");
                cfg_ok = false;
            } else {
                newPassRepeatEdit->setStyleSheet("QLineEdit { color: darkGreen }");
            }
        } else {
            newPassEdit->setEnabled(false);
            newPassRepeatEdit->setEnabled(false);
            newPassRepeatEdit->setStyleSheet(QString());
        }
    }

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(cfg_ok);
}



QString DevIfaceConfigDialog::macStr(const quint8 *mac) {
    QString ret;
    for (unsigned i = 0; i < Config::MAC_ADDR_LEN; i++) {
        ret += QString("%1").arg(mac[i], 2, 16, QChar('0'));
        if (i!=(Config::MAC_ADDR_LEN-1))
            ret += macSeparator();
    }
    return ret;
}
