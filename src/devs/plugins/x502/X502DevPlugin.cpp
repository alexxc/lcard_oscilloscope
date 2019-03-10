#include "X502DevPlugin.h"
#include "lqmeas/devs/x502/x502Resolver.h"
#include "lqmeas/devs/x502/modules/E502/E502.h"
#include "lqmeas/devs/x502/modules/E502/E502TypeInfo.h"
#include "lqmeas/devs/x502/modules/L502/L502.h"
#include "lqmeas/devs/x502/modules/L502/L502TypeInfo.h"
#include "X502ConfigDialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QCoreApplication>
#include <QTime>
#include "e502api.h"

#include "DevIfaceConfigDialog.h"
#include "X502AsyncIoDialog.h"
#include "LbootDialog.h"
#include "LQMeasStudio.h"

X502DevPlugin::X502DevPlugin(QObject *parent) :
    LQDevPlugin(parent) {

}

LQMeas::DevicesResolver *X502DevPlugin::resolver() const {
    return LQMeas::x502Resolver::resolver();
}

QStringList X502DevPlugin::devnames() const {
    return QStringList() << LQMeas::E502TypeInfo::name() << LQMeas::L502TypeInfo::name();
}

void X502DevPlugin::actionExec(LQDevPlugin::ActionTypes type, QSharedPointer<LQMeas::Device> dev, QWidget *parent) const {
    if (type == ActionTypeDevConfig) {
        QSharedPointer<LQMeas::x502> x502 = qSharedPointerObjectCast<LQMeas::x502>(dev);
        X502ConfigDialog dialog(parent, x502);
        dialog.exec();
    } else if (type == ActionTypeDevIfaceConfig) {
        devIfaceConfig(dev, parent);
    } else if (type == ActionTypeDevUpdateFirmware) {
        devUpdateFirmware(dev, parent);
    } else if (type == ActionTypeAsyncIO) {
        QSharedPointer<LQMeas::x502> x502 = qSharedPointerObjectCast<LQMeas::x502>(dev);
        X502AsyncIoDialog dialog(parent, x502);
        dialog.exec();
    }
}

bool X502DevPlugin::actionEnabled(LQDevPlugin::ActionTypes type, QSharedPointer<LQMeas::Device> dev) const {
    if ((type == ActionTypeDevConfig) || (type == ActionTypeAsyncIO)) {
        return true;
    }

    if ((type == ActionTypeDevIfaceConfig) || (type == ActionTypeDevUpdateFirmware)) {
         QSharedPointer<LQMeas::E502> e502 = qSharedPointerObjectCast<LQMeas::E502>(dev);
		 /** @todo проверка наличия Ethernet */
         if (e502 && !e502->adcIsRunning()) {
             return true;
         }
    }

    return false;
}

LQMeas::Error X502DevPlugin::devIfaceConfig(const QSharedPointer<LQMeas::Device>& dev, QWidget *parent) const {
    QSharedPointer<LQMeas::x502> x502 = qSharedPointerObjectCast<LQMeas::x502>(dev);
    t_x502_hnd hnd = x502->devHandle();
    int32_t err_code = X502_ERR_OK;

    t_e502_eth_config_hnd cfg = E502_EthConfigCreate();
    if (cfg) {
        err_code = E502_EthConfigRead(hnd, cfg);
        if (err_code != X502_ERR_OK) {
            QMessageBox::critical(parent, tr("Read interface configuration"),
                                  tr("Cannot read interface configuration: %1").
                                  arg(x502->error(err_code).msg()));
        } else {
            uint32_t bool_val;
            DevIfaceConfigDialog::Config dialogCfg;
			err_code = E502_EthConfigGetIPv4Addr(cfg, static_cast<uint32_t*>(&dialogCfg.ip_addr));
			if (err_code == X502_ERR_OK) {
				err_code = E502_EthConfigGetIPv4Mask(cfg, static_cast<uint32_t*>(&dialogCfg.ip_netmask));
			}
			if (err_code == X502_ERR_OK) {
				err_code = E502_EthConfigGetIPv4Gate(cfg, static_cast<uint32_t*>(&dialogCfg.ip_gateway));
			}
            if (err_code == X502_ERR_OK) {
                err_code = E502_EthConfigGetEnabled(cfg,  &bool_val);
				if (err_code == X502_ERR_OK) {
                    dialogCfg.eth_en = bool_val != 0;
				}
            }
            if (err_code == X502_ERR_OK) {
                err_code = E502_EthConfigGetAutoIPEnabled(cfg,  &bool_val);
				if (err_code == X502_ERR_OK) {
                    dialogCfg.auto_ip_en = bool_val != 0;
				}
            }
            if (err_code == X502_ERR_OK) {
                err_code = E502_EthConfigGetUserMACEnabled(cfg,  &bool_val);
				if (err_code == X502_ERR_OK) {
                    dialogCfg.user_mac_en = bool_val != 0;
				}
            }
			if (err_code == X502_ERR_OK) {
				err_code = E502_EthConfigGetUserMac(cfg, static_cast<unsigned char*>(dialogCfg.user_mac));
			}
			if (err_code == X502_ERR_OK) {
				err_code = E502_EthConfigGetFactoryMac(cfg, static_cast<unsigned char*>(dialogCfg.factory_mac));
			}

            if (err_code == X502_ERR_OK) {
                char inst_name[X502_INSTANCE_NAME_SIZE];
				err_code = E502_EthConfigGetInstanceName(cfg, static_cast<char*>(inst_name));
                if (err_code == X502_ERR_OK) {
					dialogCfg.instance_name = QString::fromUtf8(static_cast<char*>(inst_name));
                }

            }

            if (err_code != X502_ERR_OK) {
                QMessageBox::critical(parent, tr("Get interface parameters"),
                                      tr("Cannot get interface parameters: %1").
                                      arg(x502->error(err_code).msg()));
            } else {
                DevIfaceConfigDialog dlg(dialogCfg, parent,
                                       DevIfaceConfigDialog::FlagPasswordRequired |
                                       DevIfaceConfigDialog::FlagShowFactoryMac |
                                       DevIfaceConfigDialog::FlagShowUserMac |
                                       DevIfaceConfigDialog::FlagShowAutoIP  |
                                       DevIfaceConfigDialog::FlagShowSetPassword |
                                       DevIfaceConfigDialog::FlagShowEthEnable |
                                       DevIfaceConfigDialog::FlagShowSvcInstanceName);
                if (dlg.exec() == QDialog::Accepted) {
                    err_code = E502_EthConfigSetIPv4Addr(cfg, dlg.ipAddr());
					if (err_code == X502_ERR_OK) {
                        err_code = E502_EthConfigSetIPv4Mask(cfg, dlg.ipMask());
					}
					if (err_code == X502_ERR_OK) {
                        err_code = E502_EthConfigSetIPv4Gate(cfg, dlg.ipGate());
					}
					if (err_code == X502_ERR_OK) {
                        err_code = E502_EthConfigSetEnabled(cfg,  dlg.ethIfaceEnabled() ? 1 : 0);
					}
					if (err_code == X502_ERR_OK) {
                        err_code = E502_EthConfigSetAutoIPEnabled(cfg, dlg.autoIPEnabled() ? 1 : 0);
					}
					if (err_code == X502_ERR_OK) {
                        err_code = E502_EthConfigSetUserMACEnabled(cfg, dlg.userMacEnabled() ? 1 : 0);
					}
					if (err_code == X502_ERR_OK) {
						err_code = E502_EthConfigSetUserMac(cfg, reinterpret_cast<uint8_t*>(dlg.userMac().data()));
					}
					if (err_code == X502_ERR_OK) {
                        err_code = E502_EthConfigSetInstanceName(cfg, dlg.instanceName().toUtf8());
					}

					if ((err_code == X502_ERR_OK) && dlg.setNewPassReq()) {
                        err_code = E502_EthConfigSetNewPassword(cfg, dlg.newPass().toUtf8());
					}

					if (err_code == X502_ERR_OK) {
                        err_code = E502_EthConfigWrite(hnd, cfg, dlg.passwd().toUtf8());
					}

                    if (err_code != X502_ERR_OK) {
                        LQMeas::Error err = x502->error(err_code);
                        QMessageBox::critical(parent, tr("Write interface configuration"),
                                              tr("Cannot write interface configuration: Error %1: %2").
											  arg(QString::number(err.errorCode()), err.msg()));
                    } else {
                        QMessageBox::information(parent, tr("Write interface configuration"),
                                                 tr("Interface configuration was written successfully"));
                    }


                    /* если подключены по Ethernet и изменили настройки - можно закрывать соединение,
                       так как для дальнейшей работы нужно соединение с новыми настройками */
					if ((err_code == X502_ERR_OK) && (x502->iface() == LQMeas::Device::InterfaceEthernet)) {
                        LQMeasStudioEnv->deviceTree()->refreshDeviceList();
					}
                }
            }
        }


        E502_EthConfigFree(cfg);
    }
    return x502->error(err_code);
}

LQMeas::Error X502DevPlugin::devUpdateFirmware(const QSharedPointer<LQMeas::Device>& dev, QWidget *parent) const {
    QSettings set;
    QString dir;
    int32_t err_code  = X502_ERR_OK;
    QSharedPointer<LQMeas::E502> e502 = qSharedPointerObjectCast<LQMeas::E502>(dev);

    set.beginGroup("DevPlugins");
    set.beginGroup("x502");
    /* используем сохранненную начальную директорию для открытия файла */
    dir = set.value("firmwareDir").toString();

    QString filename = QFileDialog::getOpenFileName(parent, tr("Choice of firmware file"),
                                                    dir, tr("Firmware files") + "(*.bin);;" + tr("All files") +  "(*.*)");
    if (!filename.isEmpty()) {
        /* если файл выбрали, то сохраняем его директорию, чтобы в следующий раз
           открыть диалог на ней */
        set.setValue("firmwareDir", filename);

        t_x502_hnd hnd = e502->devHandle();

        /* получаем параметры в зависимости от характеристик устройства */
        QStringList args;
        QString serial = dev->serial();
        if (!serial.isEmpty()) {
            args.append("--serial=" + serial);
        }
        LQMeas::Device::DeviceInterface iface = dev->iface();
        /* проверяем интерфейс для определения нужных параметров для загрузчика */
        if (iface == LQMeas::Device::InterfaceUSB) {
            args.append("usb");
        } else if (iface == LQMeas::Device::InterfaceEthernet) {
            args.append("--tftp-ip-addr=" + e502->ipAddr().toString());
            args.append("tftp");
        }

        err_code = E502_SwitchToBootloader(hnd);
        if (err_code != X502_ERR_OK) {
            QMessageBox::critical(parent, tr("Switch to bootloader"),
                                  tr("Cannot switch device to bootloader: %1").
                                  arg(e502->error(err_code).msg()));
        } else {
            dev->close();
            args << "-r" << "--recovery" << "--hash" << "--con-time=5000" << "--devname=E502";
            args.append(filename);

            if (iface == LQMeas::Device::InterfaceEthernet) {
                QTime dieTime = QTime::currentTime().addMSecs( 2000 );
                while( QTime::currentTime() < dieTime ) {
                    QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
                }
            }

            LbootDialog dlg(parent, args, 2500);
            dlg.exec();

            LQMeasStudioEnv->deviceTree()->refreshDeviceList();
        }
    }

    return e502->error(err_code);
}
