#ifndef LQMEASSTUDIO_DEVICETREE_H
#define LQMEASSTUDIO_DEVICETREE_H

#include <QObject>
#include <QSharedPointer>
#include <QHash>
#include "lqmeas/devs/Device.h"
#include "plugins/LQDevPlugin.h"
#include "ProcessPlugin.h"


class QTreeWidget;
class QTreeWidgetItem;
class QAction;

namespace LQMeasStudio {
    class DeviceTree : public ProcessPlugin {
        Q_OBJECT
    public:
		QString typePluginName() const {return "DeviceTree";}
		explicit DeviceTree(QTreeWidget *treeWgt, QWidget *parent = nullptr);



        QList< QSharedPointer<LQMeas::Device> > devlist() const {return m_devlist;}
        QList< QSharedPointer<LQMeas::Device> > selectedList() const;
        bool selected(const QSharedPointer<LQMeas::Device>& dev) const;

        QList<LQDevPlugin*> devPlugins() const {return m_devplugins;}

        void procStart();
        void procStop();

    public slots:
        void saveDevicesState(QSettings& set);
        void refreshDeviceList();
        void addAction(LQDevPlugin::ActionTypes type, QAction *action);
        void refreshDeviceActions();

    private slots:
        void onItemChanged(QTreeWidgetItem* item, int col);
        void onActionTriggered();
        void onCurrentChanged();
        void execDefaultAction();
    private:
		void addDevNode(const QSharedPointer<LQMeas::Device>& dev, QTreeWidgetItem *topItem=nullptr);
        LQDevPlugin *getPlugin(const QSharedPointer<LQMeas::Device>& dev);


        QTreeWidget *m_treeWgt;
        QList< QSharedPointer<LQMeas::Device> > m_devlist;
        QHash< QTreeWidgetItem *, QSharedPointer<LQMeas::Device> > m_devTreeItems;
        QList<LQDevPlugin*> m_devplugins;
        QHash<QAction *, LQDevPlugin::ActionTypes> m_actionTypes;
        QWidget *m_mainWdgt;

        static const int column_dev_check=0;

    };
}

#endif // LQMEASSTUDIO_DEVICETREE_H
