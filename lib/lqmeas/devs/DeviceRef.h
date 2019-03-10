#ifndef DEVICEREF_H
#define DEVICEREF_H

#include <QString>

namespace LQMeas {
    class DeviceInfo;
    class Device;

    class DeviceRef {
    public:
        DeviceRef(const DeviceInfo *info, const DeviceRef *parentRef = 0);
        virtual ~DeviceRef();

        virtual DeviceRef *clone() const {return new DeviceRef(*this);}

        virtual bool checkDevice(const Device *device) const;
        virtual bool sameDevice(const DeviceRef *ref) const;

        const DeviceInfo *devInfo() const {return m_info;}

        const DeviceRef *parentRef() const {return m_parent;}
        virtual int parentPos() const {return 0;}

        void setParentRef(DeviceRef *ref) {
            delete m_parent;
            m_parent = ref;
        }
    protected:
        DeviceRef(const DeviceRef &ref);

    private:
        const DeviceInfo *m_info;
        const DeviceRef *m_parent;
    };
}

#endif // DEVICEREF_H
