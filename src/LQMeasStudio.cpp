#include <QCoreApplication>

#include "LQMeasStudio.h"
#include "ChannelSignalParamPanel.h"
#include "devs/NetworkDevicesBrowser/NetworkBrowserModel.h"
#include "devs/IpAddrConfig/IpAddrConfig.h"


namespace LQMeasStudio {
    ProcessPlugin *Environment::plugin(const QString& typeName) const {
        foreach (ProcessPlugin *plugin, m_procPlugings) {
            if (plugin->objectName()==typeName)
                return plugin;
        }
        return nullptr;
    }

    Environment *Environment::instance() {
            static Environment env;
            return &env;
    }

    void Environment::loadSettings(QSettings &set) {
        set.beginGroup("LQMeasEnviroment");
        m_ipConfig->loadSettings(set);
        m_netBrowser->loadSettings(set);

		savingDir = set.value("SavingDirectory", qApp->applicationDirPath()).toString();

        set.beginGroup("ProcessPlugins");
        foreach (ProcessPlugin *plugin, LQMeasStudioEnv->processPlugings()) {
            plugin->procLoadSettings(set);
        }
        foreach (ProcessPlugin *plugin, LQMeasStudioEnv->processPlugings()) {
			plugin->procCheckAvailable();
        }

        set.endGroup();
        set.endGroup();
    }

    void Environment::saveSettings(QSettings &set) const {
        set.beginGroup("LQMeasEnviroment");
		set.setValue("SavingDirectory", savingDir);
        set.beginGroup("ProcessPlugins");
        foreach (ProcessPlugin *plugin, LQMeasStudioEnv->processPlugings()) {
            plugin->procSaveSettings(set);
        }
        set.endGroup();
        //m_devTree->saveSettings();
        m_ipConfig->saveSettings(set);
        m_netBrowser->saveSettings(set);
        set.endGroup();
    }

    void Environment::init(DeviceTree *devtree, QVBoxLayout *mainWgtLout) {
        m_mainWidgetLayout = mainWgtLout;
        m_devTree = devtree;
        m_procPlugings.append(&m_inLauncher);
        m_procPlugings.append(&m_outGen);
        m_procPlugings.append(devtree);

        m_ipConfig = new IpAddrConfig();
        m_chSigPanel = new ChannelSignalParamPanel();
        m_netBrowser = new NetworkBrowserModel();
    }

    Environment::~Environment() {
        delete m_netBrowser;
        delete m_ipConfig;
        /** @note sigPanel не удаляем, т.к. оно удаляется родительским окном, однако
           стоит подумать над более очевидным вариантом */
        //delete m_chSigPanel;
    }
}
