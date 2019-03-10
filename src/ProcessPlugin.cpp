#include "ProcessPlugin.h"
#include "LQMeasStudio.h"


namespace LQMeasStudio {

    ProcessPlugin::ProcessPlugin(const QString& name, bool optional, QObject *parent) :
        QObject(parent), m_optional(optional), m_enabled(true), m_available(true) {
        setObjectName(name);
    }
    
    void ProcessPlugin::procSetEnabled(bool enabled) {        
        if (m_optional) {
            if (m_enabled != enabled) {
                bool prevEnabled = procEnabled();
                bool newEnabled = m_available && enabled;
                m_enabled = enabled;
                if (prevEnabled != newEnabled) {
                    enableStateChanged(newEnabled);
                }
            }
        }
    }

    void ProcessPlugin::procLoadSettings(QSettings &set)     {
        set.beginGroup(objectName());
        procProtLoadSettings(set);

        m_enabled = set.value("Enabled", true).toBool();
        procProtSetEnabled(m_enabled);
        set.endGroup();
    }

    void ProcessPlugin::procSaveSettings(QSettings &set) {
        set.beginGroup(objectName());
        set.setValue("Enabled", m_enabled);
        procProtSaveSettings(set);
        set.endGroup();
    }

    void ProcessPlugin::procCheckAvailable() {
        bool is_aviable = true;
        /* плагин не доступин, если хотя бы один плагин, от которого он зависит,
           не присутствует или запрещен */
        foreach (QString dependency, procPluginDependencyNames()) {
            ProcessPlugin *depPlugin = LQMeasStudioEnv->plugin(dependency);
            if (!depPlugin || !depPlugin->procEnabled()) {
                is_aviable = false;
                break;
            }
        }

        bool prevEnabled = procEnabled();
        bool newEnabled = is_aviable && m_enabled;
        m_available = is_aviable;
        if (prevEnabled != newEnabled) {
            enableStateChanged(newEnabled);
        }
    }

    void ProcessPlugin::enableStateChanged(bool enabled) {
        procProtSetEnabled(enabled);
        foreach (ProcessPlugin * plugin, LQMeasStudioEnv->processPlugings()) {
            if (plugin->procPluginDependencyNames().contains(objectName())) {
				plugin->procCheckAvailable();
            }
        }
    }



}
