#ifndef LQMEASSTUDIO_ADCBLOCKRECEIVEPLUGIN_H
#define LQMEASSTUDIO_ADCBLOCKRECEIVEPLUGIN_H

#include "ProcessPlugin.h"
#include <QSharedPointer>
#include "in/AdcReceiver.h"
#include "DataBlock.h"

class QThread;

namespace LQMeasStudio {
    class AdcBlockReceiver;

    class AdcBlockReceivePlugin : public ProcessPlugin {
        Q_OBJECT
    public:       
		QString typePluginName() const {return "AdcBlockReceivePlugin";}

		explicit AdcBlockReceivePlugin(QObject *parent = nullptr);
        ~AdcBlockReceivePlugin();

        double blockTime() const;
        double blockInterval() const;

        void procStartPrepare();
        void procStopRequest();
        void procStop();
        void procClear();
    public slots:
        void setBlockTime(double blockTime, double intervalTime);

    signals:
		void receiveBlock(QList<QSharedPointer<DataBlock> > blockList);

    protected:
        void procProtLoadSettings(QSettings &set);
        void procProtSaveSettings(QSettings &set);

    private slots:
		void processBlock(QList<QSharedPointer<DataBlock> > blockList);

	private:
        AdcBlockReceiver *m_receiver;
        QThread *m_procThread;
    };
}
#endif // LQMEASSTUDIO_ADCBLOCKRECEIVEPLUGIN_H
