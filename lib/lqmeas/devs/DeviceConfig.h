#ifndef DEVICECONFIG_H
#define DEVICECONFIG_H

#include <QString>
#include "lqmeas/Error.h"

class QSettings;

namespace LQMeas {
    class DevAdcConfig;
    class DevOutSyncConfig;
    class DeviceTypeInfo;

    /* Базовый класс конфигурации устройства. Классы конфигурации для
       конкретного устройства должны наследоваться от данного */
    class DeviceConfig {
    public:
		virtual const DevAdcConfig *adcConfig() const {return nullptr;}
		virtual const DevOutSyncConfig *outSyncConfig() const {return nullptr;}

        DevAdcConfig *adcConfig() {
            const DevAdcConfig *cfg = ((const DeviceConfig *)(this))->adcConfig();
            return const_cast<DevAdcConfig *>(cfg);
        }

        DevOutSyncConfig *outSyncConfig() {
            const DevOutSyncConfig *cfg = ((const DeviceConfig *)(this))->outSyncConfig();
            return const_cast<DevOutSyncConfig *>(cfg);
        }

        /* название устройства (или группы устройств с одним классом
           конфигурации), для которого предназначена конфигурация */
        virtual QString configDevTypeName() const = 0;
        /* создание новой копии текущей конфигурации */
        virtual DeviceConfig *clone() const = 0;
        /* загрузка параметров конфигурации из настроек из группы
           с именем key (относительно текущей группы в QSettings) */
        Error loadConfig(QSettings &set, QString key);
        /* сохранение параметров конфигурации в настройки в группу
           с именем key (относительно текущей группы в QSettings).
           Если key - пустая строка, то используется currentKey() */
        Error saveConfig(QSettings &set, QString key = QString()) const;
        /* обновление параметров. на основе установленных параметров
           рассчитываются реальные значения, которые могут быть установлены,
           и также может проверяться правильность конфигурации */
        Error updateConfig();
        /* Текущее значение ключа для сохранения настроек. Последнее
         * значение из loadConfig() или saveConfig() (с непустым значением
         * ключа) */
        QString currentKey() const {return m_curKey;}

        /* Имя поля под которым сохраняется тип конфигурации */
        static QString configDevTypeKeyName() {return "configDevTypeName";}

        virtual ~DeviceConfig() {}

        void updateDevTypeInfo(const DeviceTypeInfo *info);
    protected:
        virtual Error protLoadConfig(QSettings &set) = 0;
        virtual Error protSaveConfig(QSettings &set) const = 0;
        virtual Error protUpdateConfig() = 0;

        void reportLoadWarning(QString msg) const;
    private:
        mutable QString m_curKey;
    };
}

#endif // DEVICECONFIG_H
