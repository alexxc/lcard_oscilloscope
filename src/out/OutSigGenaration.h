#ifndef OUTSIGGENARATION_H
#define OUTSIGGENARATION_H

#include "ProcessPlugin.h"

#include "lqmeas/devs/Device.h"

namespace LQMeas {
    class DevOutSyncStatusTracker;
}

namespace LQMeasStudio {
    class OutSigGenaration : public ProcessPlugin {
        Q_OBJECT
    public:
		QString typePluginName() const {return "OutSigGenaration";}
        OutSigGenaration() : ProcessPlugin(typePluginName()) {}

        void procStartPrepare();
        void procStart();
        void procStopRequest();
        void procStop();
        void procClear();

		unsigned int runningDevs();



    signals:
        void deviceError(QSharedPointer<LQMeas::Device> dev, LQMeas::Error err, QString caption, QString msg);        
        void deviceGenUnderflow(QSharedPointer<LQMeas::Device> dev);

    private Q_SLOTS:

        void onGenError(LQMeas::Error err);
        void onGenUnderflow();
    private:
        QSharedPointer<LQMeas::Device> getDev(LQMeas::DevOutSyncStatusTracker *tracker);

        QList<QSharedPointer<LQMeas::Device> > m_devs;


    };
}

#endif // OUTSIGGENARATION_H
