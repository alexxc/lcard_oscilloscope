#ifndef LQMEASSTUDIO_PROCESSPLUGIN
#define LQMEASSTUDIO_PROCESSPLUGIN

#include <QObject>
#include <QSettings>
#include <QStringList>

namespace LQMeasStudio {
    /* Базовый класс для плагинов обработки данных.
       Используется для общего вызова запуска и останова обработки, загрузки
       и сохранения настроек плагина и т.п. */
    class ProcessPlugin : public QObject {
        Q_OBJECT
    public:
		ProcessPlugin(const QString& name, bool optional = false, QObject *parent = nullptr);

		virtual QString typePluginName() const {return "GenericPlugin";}

        virtual void procStartPrepare() {}
        virtual void procStart() {}
        virtual void procStopRequest() {}
        virtual void procStop() {}
        virtual void procClear() {}
        virtual void retranslate() {}

        virtual QStringList procPluginDependencyNames() const {return QStringList();}

        void procSetEnabled(bool enabled);
		bool procEnabled() const {return m_enabled && m_available;}

        void procLoadSettings(QSettings &set);
        void procSaveSettings(QSettings &set);

		void procCheckAvailable();

    protected:
		virtual void procProtLoadSettings(QSettings &set) {Q_UNUSED(set);}
		virtual void procProtSaveSettings(QSettings &set) {Q_UNUSED(set);}
		virtual void procProtSetEnabled(bool enabled) {Q_UNUSED(enabled);}

	private:

        void enableStateChanged(bool enabled);

        bool m_optional;
        bool m_enabled;
		bool m_available;
    };
}



#endif // LQMEASSTUDIO_PROCESSPLUGIN

