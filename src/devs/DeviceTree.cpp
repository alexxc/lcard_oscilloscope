#include "DeviceTree.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSettings>
#include <QAction>
#include <QApplication>
#include "WaitDialog.h"

#include "config.h"

#ifdef LQMEASSTUDIO_USE_DEV_X502
    #include "plugins/x502/X502DevPlugin.h"
#endif
#ifdef LQMEASSTUDIO_USE_DEV_LCOMP
    #include "plugins/lcomp/lcompDevPlugin.h"
#endif
#ifdef LQMEASSTUDIO_USE_DEV_LTR
    #include "plugins/ltr/LtrDevPlugin.h"
#endif

namespace LQMeasStudio {
    DeviceTree::DeviceTree(QTreeWidget *treeWgt, QWidget *parent) :
        ProcessPlugin(typePluginName(), false, parent), m_treeWgt(treeWgt), m_mainWdgt(parent) {

    #ifdef LQMEASSTUDIO_USE_DEV_X502
        m_devplugins.append(new X502DevPlugin(this));
    #endif
    #ifdef LQMEASSTUDIO_USE_DEV_LCOMP
        m_devplugins.append(new lcompDevPlugin(this));
    #endif
    #ifdef LQMEASSTUDIO_USE_DEV_LTR
        m_devplugins.append(new LtrDevPlugin(this));
    #endif

        treeWgt->setContextMenuPolicy(Qt::ActionsContextMenu);
        connect(treeWgt, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
                this, SLOT(execDefaultAction()));
        connect(treeWgt->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(onCurrentChanged()));
    }

    void DeviceTree::saveDevicesState(QSettings &set) {
        set.beginGroup("Devices");
        /* сохранение настроек всех найденных устройств */
		for (const QSharedPointer<LQMeas::Device>& dev : m_devlist) {
            set.beginGroup(dev->devKey());
            set.setValue("selected", selected(dev));
            set.endGroup();
            dev->saveConfig(set, dev->devKey());
            dev->saveConfig(set, dev->name());
        }
        set.endGroup();

    }

    QList<QSharedPointer<LQMeas::Device> > DeviceTree::selectedList() const {
        QList<QSharedPointer<LQMeas::Device> > retlist;
		for (const QSharedPointer<LQMeas::Device>& dev : m_devlist) {
            if (selected(dev)) {
                retlist.append(dev);
            }
        }
        return retlist;
    }

    bool DeviceTree::selected(const QSharedPointer<LQMeas::Device>& dev) const {
		for (QTreeWidgetItem *item : m_devTreeItems.keys()) {
            if (m_devTreeItems[item]==dev) {
                return item->checkState(0) == Qt::Checked;
            }
        }
        return false;
    }

    void DeviceTree::procStart() {
        refreshDeviceActions();
    }

    void DeviceTree::procStop() {
        refreshDeviceActions();
    }


    void DeviceTree::refreshDeviceList() {
        QSettings set;
        WaitDialog waitDlg(m_mainWdgt);
        waitDlg.setModal(true);
        waitDlg.show();
        waitDlg.setText(tr("Refresh list of devices..."));

        /** @todo придумать более красивый способ */
        for (int i=0; i < 250; i++)
            qApp->processEvents();
        saveDevicesState(set);

        foreach (QSharedPointer<LQMeas::Device> dev, m_devlist) {            
            dev->close();
        }

        m_devlist.clear();
        m_devTreeItems.clear();
        m_treeWgt->clear();

        foreach (LQDevPlugin *plugin, m_devplugins) {
            QList<QSharedPointer<LQMeas::Device> > list = plugin->resolver()->getDevList();
            foreach (QSharedPointer<LQMeas::Device> dev, list) {
                addDevNode(dev);
            }
        }

        m_treeWgt->expandAll();
        connect(m_treeWgt, SIGNAL(itemChanged(QTreeWidgetItem*,int)), SLOT(onItemChanged(QTreeWidgetItem*,int)));
    }

    void DeviceTree::addDevNode(const QSharedPointer<LQMeas::Device>& dev, QTreeWidgetItem *topItem) {
        LQMeas::Error err = LQMeas::Error::Success();
        bool selected;

        auto *item = new QTreeWidgetItem();
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

        QSettings set;
        set.beginGroup("Devices");

        if (!dev->isOpened()) {
            err = dev->open();
            if (err.isSuccess()){
                if (set.childGroups().contains(dev->devKey())) {
                    dev->loadConfig(set, dev->devKey());
                } else if (set.childGroups().contains(dev->name())) {
                    dev->loadConfig(set, dev->name());
                }
            }
        }

        set.beginGroup(dev->devKey());
        selected = set.value("selected", true).toBool();
        set.endGroup();

        set.endGroup();

        /* проверяем, разрешено ли уже данное устройтво */
        /** @todo Есть одно не совсем логичное поведение. Если устройства было
           разрешено, потом при следующем обновлении устройств его не было и оно
           разрешено по другому интерфейсу, то при обновлении будет выбрано устройство
           по первому, а не второму... возможно это можно избежать если рассматривать
           идеалогию с отслеживанием устройств, а не обновлением списка... */
        if (selected) {
		   for (QTreeWidgetItem *tst_item : m_devTreeItems.keys()) {
                if ((tst_item != item) && (tst_item->checkState(0) == Qt::Checked)) {
                    QSharedPointer<LQMeas::Device> tst_dev = m_devTreeItems[tst_item];
                    if ((tst_dev->serial() == dev->serial()) &&
                            (tst_dev->name() == dev->name()) &&
                            (tst_item->checkState(0) == Qt::Checked)) {
                        selected = false;
                        break;
                    }
                }
            }
        }



        item->setCheckState(0, selected && dev->isOpened() ? Qt::Checked : Qt::Unchecked);

        item->setText(0, dev->devStrFull());
        if (dev->iface() == LQMeas::Device::InterfaceEthernet) {
            item->setIcon(0, QIcon(":/icons/ipaddr.png"));
        } else if (dev->iface() == LQMeas::Device::InterfaceUSB) {
            item->setIcon(0, QIcon(":/icons/usb.png"));
        } else if (dev->iface() == LQMeas::Device::InterfacePCI) {
            item->setIcon(0, QIcon(":/icons/pci.png"));
        }

        m_devTreeItems[item]  = dev;
        m_devlist.append(dev);

        item->setForeground(0, err.isSuccess() ? QBrush(Qt::darkGreen) : QBrush(Qt::red));

        if (!topItem) {

            m_treeWgt->addTopLevelItem(item);
        } else {
            topItem->addChild(item);
        }


        LQMeas::DevicesResolver *childResolver = dev->childResolver();
        if (childResolver) {
            QList<QSharedPointer<LQMeas::Device> > childs = childResolver->getDevList();
            foreach (QSharedPointer<LQMeas::Device> child, childs) {
                addDevNode(child, item);
            }
        }


    }

    LQDevPlugin *DeviceTree::getPlugin(const QSharedPointer<LQMeas::Device>& dev) {
        LQDevPlugin *ret = nullptr;
        if (dev) {
            foreach (LQDevPlugin *plugin, m_devplugins) {
                if (plugin->devnames().contains(dev->name())) {
                    ret = plugin;
                    break;
                }
            }
        }
        return ret;
    }


    void DeviceTree::onItemChanged(QTreeWidgetItem *item, int col) {
        if (col==column_dev_check) {
            Qt::CheckState check = item->checkState(col);
            QTreeWidgetItem *parent = item->parent();
            if (parent) {
                 Qt::CheckState par_check = check;
                 for (int i=0; (i < parent->childCount()) && (par_check != Qt::PartiallyChecked); i++) {
                     if (parent->child(i)->checkState(column_dev_check) != par_check)
                         par_check = Qt::PartiallyChecked;
                 }

                 parent->setCheckState(column_dev_check, par_check);
            }

            if (check != Qt::PartiallyChecked) {
                for (int i=0; i < item->childCount(); i++) {
                    item->child(i)->setCheckState(column_dev_check, check);
                }
            }

            if (check == Qt::Checked) {
                QSharedPointer<LQMeas::Device> dev = m_devTreeItems[item];
				for (QTreeWidgetItem *tst_item : m_devTreeItems.keys()) {
                    if ((tst_item != item) && (tst_item->checkState(0) == Qt::Checked)) {
                        QSharedPointer<LQMeas::Device> tst_dev = m_devTreeItems[tst_item];
                        if ((tst_dev->serial() == dev->serial()) &&
                                (tst_dev->name() == dev->name())) {
                            tst_item->setCheckState(0, Qt::Unchecked);
                        }
                    }
                }
            }
        }
    }

    void DeviceTree::addAction(LQDevPlugin::ActionTypes type, QAction *action) {
        m_actionTypes[action] = type;
        action->setEnabled(false);
        connect(action, SIGNAL(triggered()), SLOT(onActionTriggered()));
        m_treeWgt->addAction(action);
    }

    void DeviceTree::refreshDeviceActions() {
        QList<QTreeWidgetItem*> items = m_treeWgt->selectedItems();
        bool fnd_dev = false;
        if (!items.empty()) {
            QSharedPointer<LQMeas::Device> dev = m_devTreeItems[items[0]];
            LQDevPlugin *plugin = getPlugin(dev);
            if (dev && plugin) {
                fnd_dev = true;
				for (QAction *action : m_actionTypes.keys()) {
                    action->setEnabled(plugin->actionEnabled(m_actionTypes[action], dev));
                }
            }
        }
        if (!fnd_dev)  {
			for (QAction *action : m_actionTypes.keys()) {
                action->setEnabled(false);
            }
        }
    }


    void DeviceTree::onActionTriggered() {
        auto *action = qobject_cast<QAction *>(sender());
        if (action) {
            QList<QTreeWidgetItem*> items = m_treeWgt->selectedItems();
            if (!items.empty()) {
                QSharedPointer<LQMeas::Device> dev = m_devTreeItems[items[0]];
                LQDevPlugin *plugin = getPlugin(dev);
                if (dev && plugin) {
                    plugin->actionExec(m_actionTypes[action], dev, m_mainWdgt);
                }
            }
        }
    }

    void DeviceTree::onCurrentChanged() {
        refreshDeviceActions();
    }

    void DeviceTree::execDefaultAction() {
        QList<QTreeWidgetItem*> items = m_treeWgt->selectedItems();
        if (!items.empty()) {
            QSharedPointer<LQMeas::Device> dev = m_devTreeItems[items[0]];
            LQDevPlugin *plugin = getPlugin(dev);
            if (dev && plugin) {
                plugin->actionExec(plugin->defaultAction(), dev, m_mainWdgt);
            }
        }
    }
}

