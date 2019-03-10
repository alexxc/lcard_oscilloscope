#ifndef INRECEIVELAUNCHER_H
#define INRECEIVELAUNCHER_H

#include <QSharedPointer>
#include "ProcessPlugin.h"
#include "lqmeas/ifaces/in/DevAdc.h"
#include "AdcReceiver.h"
#include "ProcessPlugin.h"

namespace LQMeasStudio {
    class InReceiveLauncher : public ProcessPlugin {
        Q_OBJECT
    public:
		QString typePluginName() const {return "InReceiveLauncher";}

        void procStartPrepare();
        void procStart();
        void procStopRequest();
        void procStop();
        void procClear();

        unsigned runningDevs();


        explicit InReceiveLauncher(QObject *parent = 0);

        QList<AdcReceiver *> adcReceivers() const {return m_receivers;}
    signals:
        void deviceError(QSharedPointer<LQMeas::Device> dev, LQMeas::Error err, QString caption, QString msg);

    private slots:
        void onAdcReceiverFinished();
        void emitError(QSharedPointer<LQMeas::Device> dev, LQMeas::Error err);

    private:
        void startAdc(AdcReceiver *receiver);

        QList<AdcReceiver*> m_receivers;
    };
}

#endif // INRECEIVELAUNCHER_H
