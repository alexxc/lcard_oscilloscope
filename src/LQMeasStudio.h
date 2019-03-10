#ifndef LQMEASSTUDIO_ENVIRONMENT_H
#define LQMEASSTUDIO_ENVIRONMENT_H

#include "ProcessPlugin.h"
#include "in/InDataChannelsConfig.h"
#include "devs/DeviceTree.h"
#include "in/InReceiveLauncher.h"
#include "out/OutSigGenaration.h"

#include <QVBoxLayout>

#define LQMEAS_TS_BASENAME "lqmeasstudio"

#define LQMeasStudioEnv  LQMeasStudio::Environment::instance()
#define LQMeasStudioPlugin(typeName) dynamic_cast<typeName*>(LQMeasStudio::Environment::instance()->plugin(#typeName))

namespace LQMeasStudio {
    class ChannelSignalParamPanel;
    class NetworkBrowserModel;
    class IpAddrConfig;

    class Environment {
    public:

		QVBoxLayout *mainWidgetLayout() const {return m_mainWidgetLayout;}
        ChannelSignalParamPanel *chSignalParamsPanel() const {return m_chSigPanel;}
        InDataChannelsConfig *channelsConfig() {return &m_inChCfg;}
        DeviceTree *deviceTree() const {return m_devTree;}
        InReceiveLauncher *inLauncher() {return &m_inLauncher;}
        OutSigGenaration  *outGenerator() {return &m_outGen;}
        IpAddrConfig *ipConfig() const {return m_ipConfig;}
        NetworkBrowserModel *netBrowser() const {return m_netBrowser;}

        QList<ProcessPlugin *> processPlugings() const {return m_procPlugings;}

        ProcessPlugin *plugin(const QString& typeName) const;

        static Environment *instance();

        void loadSettings(QSettings &set);
        void saveSettings(QSettings &set) const;

    public slots:
        void addPlugin(ProcessPlugin *plugin) {m_procPlugings.append(plugin);}
		QString savingDirectory() {return savingDir;}

	private:
		Environment() : m_chSigPanel(nullptr), m_netBrowser(nullptr) {}

        void init(DeviceTree *devtree, QVBoxLayout *mainWgtLout);

        ~Environment();

        ChannelSignalParamPanel *m_chSigPanel;
        DeviceTree *m_devTree;
        InReceiveLauncher m_inLauncher;
        OutSigGenaration  m_outGen;
        InDataChannelsConfig m_inChCfg;
		QVBoxLayout *m_mainWidgetLayout;

        QList<ProcessPlugin *> m_procPlugings;
        NetworkBrowserModel *m_netBrowser;
        IpAddrConfig *m_ipConfig;

		QString savingDir;
		friend class MainWindow;
    };
}

#endif // LQMEASSTUDIO_ENVIRONMENT_H
