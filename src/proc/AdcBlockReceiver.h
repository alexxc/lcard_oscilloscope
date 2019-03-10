#ifndef LQMEASSTUDIO_ADCBLOCKRECEIVER_H
#define LQMEASSTUDIO_ADCBLOCKRECEIVER_H

#include <QObject>
#include <QHash>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QByteArray>

#include "in/AdcReceiver.h"
#include "AdcBlockReceivePlugin.h"

namespace LQMeasStudio {
    class AdcBlockReceiver : public QObject {
        Q_OBJECT
    public:
        AdcBlockReceiver();

        void start();
        void stop();
        void clear();

        void setBlockTime(double blockTime, double blockInterval);

        double blockTime() const {return m_procBlockTime;}
        double blockInterval() const {return m_procBlockInterval;}

	signals:
		void receiveBlock(QList<QSharedPointer<DataBlock> > blockList);

	private slots:
		void processData(QList<QSharedPointer<AdcReceiver::Data> > dataList);

	private:
		void saveBlocks(QList<QSharedPointer<DataBlock> > &blockList);
		struct ChProcInfo {
            QVector<double> y;
            unsigned procPoints;
            bool blockSent;
            double startX;

            ChProcInfo() : procPoints(0), blockSent(false) {}
        };
        QHash<int, ChProcInfo*> m_chProcInfo;
        double m_procBlockTime;
        double m_procBlockInterval;
    };
}

#endif // LQMEASSTUDIO_ADCBLOCKRECEIVER_H
