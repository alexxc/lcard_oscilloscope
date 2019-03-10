#include "AdcReceiver.h"
#include "lqmeas/devs/Device.h"
#include "lqmeas/StdErrors.h"

#include <math.h>
#include <QElapsedTimer>

namespace LQMeasStudio {
	AdcReceiver::AdcReceiver(const QSharedPointer<LQMeas::Device> &dev, QVector<QSharedPointer<ChannelInfo> > channels, QObject *parent) :
		QThread(parent), m_channels(channels), m_dev(dev), m_err(LQMeas::Error::Success()) {
		m_adc = dev->devAdc();
		qRegisterMetaType< QList<QSharedPointer<AdcReceiver::Data> > >("QList<QSharedPointer<LQMeasStudio::AdcReceiver::Data> >");
		qRegisterMetaType< QList<QSharedPointer<AdcReceiver::Data> > >("QList<QSharedPointer<AdcReceiver::Data> >");
	}

	void AdcReceiver::run() {
		signed meas_time = 100;
		unsigned ch_size;
		unsigned recv_size;
		unsigned enabled_ch_cnt;
		qint64 cur_x_idx = 0;
		double dt = 1./m_adc->adcConfig()->adcChFreq();

		m_stopReq = false;

		ch_size = static_cast<unsigned>(lround(static_cast<double>(meas_time)/(dt*1000.) + 0.5));
		if (ch_size < 2)
			ch_size = 2;

		enabled_ch_cnt = m_adc->adcConfig()->adcEnabledChCnt();
		recv_size = ch_size*enabled_ch_cnt;


		QScopedArrayPointer<double> vals(new double[recv_size]);


		 while (m_err.isSuccess() && !m_stopReq) {
			unsigned int recvd_size = 0;

			while (m_err.isSuccess() && !m_stopReq && (recvd_size < recv_size)) {
				unsigned cur_recvd_size=0;
				m_err = m_adc->adcGetData(&vals.data()[recvd_size], recv_size-recvd_size, 0,
										  100, &cur_recvd_size);
				if (m_err.isSuccess())
					recvd_size+=cur_recvd_size;
			}

			if (m_err.isSuccess() && (recvd_size != 0)) {
				QList<QSharedPointer<Data> > dataList;
				ch_size = recvd_size/enabled_ch_cnt;
				for (unsigned ch=0; ch < m_adc->adcConfig()->adcEnabledChCnt(); ++ch) {
					QSharedPointer<Data> chData = QSharedPointer<Data>(new Data());
					chData->ch = m_channels.at(static_cast<signed>(ch));
					chData->data.resize(static_cast<signed>(ch_size));
					chData->x_idx = cur_x_idx;
					chData->dt = dt;

					for (unsigned i=0; i < ch_size; ++i) {
						chData->data[static_cast<signed>(i)] = vals[static_cast<signed>(enabled_ch_cnt*i + ch)];
					}

					dataList.append(chData);
				}

				cur_x_idx += ch_size;

				emit dataReceived(dataList);
			}
		}

		if (m_adc->adcIsRunning()) {
			LQMeas::Error stop_err = m_adc->adcStop();
			if (m_err.isSuccess())
				m_err = stop_err;
		}

	}

	LQMeas::Error AdcReceiver::lastError() const {
		return m_err;
	}

	void AdcReceiver::stopRequest() {
			m_stopReq = true;
	}
}
