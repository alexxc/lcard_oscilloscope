#include "InReceiveLauncher.h"

#include <utility>

#include "LQMeasStudio.h"

namespace LQMeasStudio {
    void InReceiveLauncher::procClear() {
        qDeleteAll(m_receivers);
        m_receivers.clear();
    }

    void InReceiveLauncher::procStartPrepare() {

        /** @todo Список каналов должен формироваться в момент настройки, а не запуска */
        LQMeasStudioEnv->channelsConfig()->clear();
        foreach (QSharedPointer<LQMeas::Device> dev, LQMeasStudioEnv->deviceTree()->selectedList()) {
            LQMeas::DevAdc *adc = dev->devAdc();
            if (adc) {
                QVector<QSharedPointer<ChannelInfo> > channels;

				for (unsigned ch_num=0; ch_num < adc->adcConfig()->adcEnabledChCnt(); ++ch_num) {
                    QSharedPointer<ChannelInfo> ch = LQMeasStudioEnv->channelsConfig()->
                            add(dev, ch_num, tr("Channel ") + QString::number(adc->adcChNum(ch_num)+1));
                    channels.append(ch);
                }

                if (!channels.empty()) {
                    AdcReceiver *receiver = new AdcReceiver(dev, channels);


                    connect(receiver, SIGNAL(finished()), SLOT(onAdcReceiverFinished()));
                    receiver->moveToThread(receiver);
                    m_receivers.append(receiver);
                }
            }
        }
    }

    void InReceiveLauncher::procStart() {
        foreach (AdcReceiver *receiver, m_receivers) {
            if (receiver->adc()->adcConfig()->adcExternalStart())
                startAdc(receiver);
        }

        foreach (AdcReceiver *receiver, m_receivers) {
            if (!receiver->adc()->adcConfig()->adcExternalStart())
                startAdc(receiver);
        }
    }

    void InReceiveLauncher::procStopRequest() {
        foreach (AdcReceiver *receiver, m_receivers) {
            receiver->stopRequest();
        }
    }

    void InReceiveLauncher::procStop() {
        foreach (AdcReceiver* receiver, m_receivers) {
            receiver->wait();
        }


        foreach (AdcReceiver *receiver, m_receivers) {
            disconnect(receiver, SIGNAL(finished()), this, SLOT(onAdcReceiverFinished()));
        }
    }

    unsigned InReceiveLauncher::runningDevs() {
        unsigned cnt = 0;
        foreach (AdcReceiver *receiver, m_receivers) {
            if (receiver->isRunning()) {
                cnt++;
            }
        }
        return cnt;
    }
    InReceiveLauncher::InReceiveLauncher(QObject *parent) :
        ProcessPlugin(typePluginName(), false, parent) {

    }

    void InReceiveLauncher::onAdcReceiverFinished() {
        auto *receiver = qobject_cast<AdcReceiver *>(sender());
        if (receiver && !receiver->lastError().isSuccess()) {
            emitError(receiver->device(), receiver->lastError());
        }
    }

    void InReceiveLauncher::emitError(QSharedPointer<LQMeas::Device> dev, LQMeas::Error err) {
        emit deviceError(std::move(dev), std::move(err), tr("Data acquisition error"),
                         tr("Error occured during data acquisition"));
    }

    void InReceiveLauncher::startAdc(AdcReceiver *receiver)     {
        LQMeas::Error err = receiver->adc()->adcStart();
        if (err.isSuccess()) {
            receiver->start();
        } else {
            emitError(receiver->device(), err);
        }
    }
}
