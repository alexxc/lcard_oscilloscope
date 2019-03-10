#ifndef LQMEAS_DEVICEINFO_H
#define LQMEAS_DEVICEINFO_H

#include <QString>
#include "DeviceTypeInfo.h"

namespace LQMeas {


    /* Информация о экземпляре устройства. Включает в себя серийный номер, а также
       информацию о типе устройства (или конкретной модфицикации).

       Каждый класс устройства реализует свой класс информации типа <тип устройтсва>Info,
       в который добавляется специфическая информация для конкретного экземпляра, такая
       как версии прошивок и т.п. */
    class DeviceInfo {
    public:
        DeviceInfo(const DeviceTypeInfo *type, QString serial) : m_serial(serial), m_type(type) {
        }

        virtual ~DeviceInfo() {
            m_type->release();
        }

        const DeviceTypeInfo *type() const {return m_type;}
        QString serial() const {return  m_serial;}

        /* признак, что обе информации относятся к одному устройству */
        virtual bool sameDevice(const DeviceInfo *info) const {
            return (m_type->devceModificationName() == info->m_type->devceModificationName()) &&
                    ((info->serial() == m_serial) || m_serial.isEmpty() || info->serial().isEmpty());
        }

        virtual const DeviceInfo *clone() const = 0;
    protected:
        DeviceInfo(const DeviceInfo &info) {
            m_serial = info.m_serial;
            m_type = info.m_type->clone();
        }
    private:
        const DeviceTypeInfo *m_type;
        QString m_serial;
    };
}

#endif // DEVICEINFO_H
