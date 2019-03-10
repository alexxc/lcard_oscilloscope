#ifndef LQMEAS_DEVICENETWORKBROWSER_H
#define LQMEAS_DEVICENETWORKBROWSER_H


#include <QSharedPointer>
#include <QList>
#include "lqmeas/Error.h"
#include "DeviceSvcRecord.h"

namespace LQMeas {
    class DeviceNetworkBrowser : public QObject {
        Q_OBJECT
    public:
        enum Event {
            EventAdd,
            EventChange,
            EventRemoved
        };

    public slots:
        void stopRequest();
        bool stopWiat(unsigned long time = ULONG_MAX);
        void start();
    signals:
        void stateChanged(QSharedPointer<LQMeas::DeviceSvcRecord>,LQMeas::DeviceNetworkBrowser::Event);
        void error(LQMeas::Error err, QString msg);
        void startPorcessingRequest();

    protected:
        DeviceNetworkBrowser();
        virtual ~DeviceNetworkBrowser();
        bool stopIsRequested() const {return m_stop_requested;}

        virtual void run() = 0;

        void addRecord(QSharedPointer<DeviceSvcRecord> svcRec);
        void changeRecord(QSharedPointer<DeviceSvcRecord> svcRec);
        void removeRecord(QSharedPointer<DeviceSvcRecord> svcRec);
        void setError(Error err, QString msg);
    private slots:
        void startPorcessing();
    private:



        QList<QSharedPointer<DeviceSvcRecord> > m_curDevs;
        bool m_stop_requested;
        QThread *m_thread;
    };
}

#endif // LQMEAS_DEVICENETWORKBROWSER_H
