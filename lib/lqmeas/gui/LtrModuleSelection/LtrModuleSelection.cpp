#include "LtrModuleSelection.h"
#include <QLineEdit>
#include <QComboBox>
#include <QSettings>
#include <QMessageBox>
#include <QProgressDialog>

#include "lqmeas/devs/ltr/LtrResolver.h"
#include "lqmeas/Error.h"

namespace LQMeas {
    void LtrModuleSelection::init(QWidget *parent) {
        /* создаем диалог, который будем использовать для индикации прогресса модулей */
        m_progrDialog =  new QProgressDialog("",0, 0, 100, parent, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint
                                             | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        m_progrDialog->setMinimumWidth(350);
        m_progrDialog->setWindowModality(Qt::WindowModal);
    }



    LtrModuleSelection::LtrModuleSelection(QStringList devs, QComboBox *crate, QComboBox *slot,
                                           QLineEdit *serial, QWidget *parent, QString set_group) :
        QObject(parent), m_set_group(set_group),
        m_crateBox(crate), m_slotBox(slot), m_serialEdt(serial),
        m_parent(parent), m_dev_reopen_cnt(0),
        m_validator(QSharedPointer<DeviceValidator>(new DeviceNameValidator(devs))) {

        init(parent);
    }

    LtrModuleSelection::LtrModuleSelection(QSharedPointer<DeviceValidator> validator, QComboBox *crate, QComboBox *slot,
                                           QLineEdit *serial, QWidget *parent, QString set_group) :
        QObject(parent), m_set_group(set_group), m_validator(validator),
        m_crateBox(crate), m_slotBox(slot), m_serialEdt(serial),
        m_parent(parent), m_dev_reopen_cnt(0) {

        init(parent);
    }

    LtrModuleSelection::~LtrModuleSelection() {
        closeCurModule();
    }

    void LtrModuleSelection::reset(bool refresh) {
        m_dev_reopen_cnt = 0;
        QString serial;
        int slot=0;
        if (m_dev) {
            slot = m_dev->slot();
            serial =  m_dev->crate()->serial() ;
        }
        closeCurModule();
        if (refresh)
            refreshDevList(serial, slot);
    }

    void LtrModuleSelection::refreshDevList(QString prefCrate, int prefSlot) {
        disconnect(m_crateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCrateChanged()));

        if (prefCrate.isEmpty() && !prefSlot) {
            QSettings set;
            set.beginGroup(m_set_group);
            prefCrate = set.value("crateSerial").toString();
            prefSlot = set.value("slot",0).toInt();
            set.endGroup();
        }

        m_devList = LtrResolver::resolver()->getModuleList(m_validator.data());
        m_crateSerialList.clear();
        int fnd_ind = 0;

         foreach (QSharedPointer<LtrModule> dev, m_devList) {
             if (!m_crateSerialList.contains(dev->crate()->serial())) {
                 if (dev->crate()->serial()==prefCrate)
                     fnd_ind = m_crateSerialList.size();
                 m_crateSerialList.append(dev->crate()->serial());
             }
         }

         m_crateBox->clear();
         foreach (QString serial, m_crateSerialList) {
             m_crateBox->addItem(serial);
         }

         if (m_crateBox->count()) {
            m_crateBox->setCurrentIndex(fnd_ind);
         }

         onCrateChanged(prefSlot, true);

         connect(m_crateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCrateChanged()));

         if (m_devList.empty()) {
             QMessageBox::warning(m_parent, tr("Module not found"),
                                  tr("Cannot find specified device!") + "\n" +
                                  tr("Check crate with modules connection and press 'Reset'"),
                                  QMessageBox::Ok);
         }
    }

    void LtrModuleSelection::closeCurModule() {
        if (m_dev) {
            saveSettings();
            if (m_dev->isOpened()) {
                m_dev->close();
            }
            disconnect(m_dev.data(), SIGNAL(opProgress(LQMeas::Device::OpStatus, QString, int, int, LQMeas::Error)),
                       this, SLOT(onModuleOpProgr(LQMeas::Device::OpStatus, QString, int, int, LQMeas::Error)));
            m_dev = QSharedPointer<LtrModule>();
        }
    }

    void LtrModuleSelection::saveSettings() {
        if (m_dev) {
            QSettings set;
            set.beginGroup(m_set_group);
            set.setValue("crateSerial", m_dev->crate()->serial());
            set.setValue("slot", m_dev->slot());
        }
    }

    void LtrModuleSelection::onCrateChanged(int prefSlot, bool try_reopen) {
        disconnect(m_slotBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onModuleChanged()));
        m_slotBox->clear();

        if (m_crateBox->currentIndex() >=0) {
            int fnd_ind = 0;
            QString crate_ser = m_crateSerialList.at(m_crateBox->currentIndex());
            foreach (QSharedPointer<LtrModule> dev, m_devList) {
                if (dev->crate()->serial()==crate_ser) {
                    if (dev->slot()==prefSlot)
                        fnd_ind = m_slotBox->count();
                    m_slotBox->addItem(QString::number(dev->slot()), dev->slot());
                }
            }

            if (m_slotBox->count()) {
                m_slotBox->setCurrentIndex(fnd_ind);
            }
        }

        onModuleChanged(try_reopen);

        connect(m_slotBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onModuleChanged()));
    }

    void LtrModuleSelection::onModuleChanged(bool try_reopen) {
        int dev_ind;

        emit beforeModuleChange();

        closeCurModule();

        if (m_slotBox->currentIndex()>=0) {
            QString crate_ser = m_crateSerialList.at(m_crateBox->currentIndex());
            int slot =  m_slotBox->itemData(m_slotBox->currentIndex()).toInt();
            for (int i=0; i < m_devList.size(); i++) {
                if ((m_devList[i]->crate()->serial()==crate_ser) && (m_devList[i]->slot()==slot)) {
                    m_dev = m_devList[i];
                    dev_ind = i;
                    break;
                }
            }

            if (m_dev) {
                connect(m_dev.data(), SIGNAL(opProgress(LQMeas::Device::OpStatus, QString, int, int, LQMeas::Error)),
                        this, SLOT(onModuleOpProgr(LQMeas::Device::OpStatus, QString, int, int, LQMeas::Error)));

                Error err = m_dev->open();
                if (!err.isSuccess()) {
                    m_dev = QSharedPointer<LtrModule>();
                    if (try_reopen && (++m_dev_reopen_cnt < m_devList.size())) {
                        if (++dev_ind == m_devList.size())
                            dev_ind = 0;
                        refreshDevList(m_devList[dev_ind]->crate()->serial(), m_devList[dev_ind]->slot());
                    } else {
                        QMessageBox::critical(m_parent, tr("Module open error"),
                                             tr("Cannot open module") + ".\n" +
                                              tr("Select another module or check connection and press 'Reset'"),
                                             QMessageBox::Ok);

                    }
                }
            }
        }

        emit moduleChanged(m_dev);
    }

    void LtrModuleSelection::onModuleOpProgr(Device::OpStatus status, QString descr, int done, int size, Error err) {
        if (status == LtrModule::OpStatusStart) {
            m_progrDialog->setMaximum(size);
            m_progrDialog->setValue(done);
            m_progrDialog->show();
            m_progrDialog->setWindowTitle(descr);
        } else if (status == LtrModule::OpStatusProgress) {
            m_progrDialog->setValue(done);
        } else if (status == LtrModule::OpStatusFinish) {
            m_progrDialog->hide();
        }
    }
}


