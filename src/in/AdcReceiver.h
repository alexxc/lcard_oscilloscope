#ifndef ADCRECEIVER_H
#define ADCRECEIVER_H

#include <QThread>
#include <QSharedPointer>
#include <QVector>

#include "in/ChannelInfo.h"
#include "lqmeas/ifaces/in/DevAdc.h"

namespace LQMeasStudio {

    class AdcReceiver : public QThread {
        Q_OBJECT
    public:
        struct Data {
        public:
			qint64 x_idx; /* номер индекса для первого отсчета блока */
            double  dt;  /* время между отсчетами канала */            
            QSharedPointer<ChannelInfo> ch;
            QVector<double> data;
        };


        explicit AdcReceiver(const QSharedPointer<LQMeas::Device>& dev,
							 QVector<QSharedPointer<ChannelInfo> > channels, QObject *parent = nullptr);


        LQMeas::DevAdc *adc() const {return m_adc;}
        QSharedPointer<LQMeas::Device>  device() const {return m_dev;}

        void run();
        LQMeas::Error lastError() const;
    signals:
		void dataReceived(QList<QSharedPointer<AdcReceiver::Data> > dataList);
    public slots:
        void stopRequest();
    private:

        QVector<QSharedPointer<ChannelInfo> > m_channels;
        LQMeas::DevAdc *m_adc;
        QSharedPointer<LQMeas::Device> m_dev;
        LQMeas::Error m_err;
        bool m_stopReq{};
    };
}

#endif // ADCRECEIVER_H
