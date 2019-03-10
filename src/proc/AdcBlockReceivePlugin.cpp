#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QDebug>

#include "AdcBlockReceivePlugin.h"
#include "AdcBlockReceiver.h"

#include "LQMeasStudio.h"
#include "DataBlockProcessPlugin.h"
#include "ChannelSignalParamPanel.h"
#include "../lib/lqmeas/devs/x502/x502Config.h"

namespace LQMeasStudio {
    AdcBlockReceivePlugin::AdcBlockReceivePlugin(QObject *parent) :
        ProcessPlugin(typePluginName(), false, parent), m_receiver(new AdcBlockReceiver()), m_procThread(new QThread()) {

        qRegisterMetaType<QList<QSharedPointer<DataBlock> > >("QList<QSharedPointer<DataBlock> >");
    }

    AdcBlockReceivePlugin::~AdcBlockReceivePlugin() {
        delete m_receiver;
        delete m_procThread;
    }

    double AdcBlockReceivePlugin::blockTime() const {
        return m_receiver->blockTime();
    }

    double AdcBlockReceivePlugin::blockInterval() const {
        return m_receiver->blockInterval();
    }

    void AdcBlockReceivePlugin::procStartPrepare() {

        m_procThread->start();

        AdcBlockReceiver *prevReceiver = m_receiver;

        m_receiver = new AdcBlockReceiver();
		m_receiver->moveToThread(m_procThread);
        if (prevReceiver) {
            m_receiver->setBlockTime(prevReceiver->blockTime(), prevReceiver->blockInterval());
            delete prevReceiver;
        }
		connect(m_receiver, SIGNAL(receiveBlock(QList<QSharedPointer<DataBlock> > )),
				SLOT(processBlock(QList<QSharedPointer<DataBlock> > )));
        m_receiver->start();
    }

    void AdcBlockReceivePlugin::procStopRequest() {

    }

    void AdcBlockReceivePlugin::procStop() {
        m_receiver->stop();
        m_procThread->quit();
        m_procThread->wait();
		disconnect(m_receiver, SIGNAL(receiveBlock(QList<QSharedPointer<DataBlock> > )),
				   this, SLOT(processBlock(QList<QSharedPointer<DataBlock> > )));
    }

    void AdcBlockReceivePlugin::procClear() {        
        m_receiver->clear();
    }

    void AdcBlockReceivePlugin::setBlockTime(double blockTime, double intervalTime) {
        m_receiver->setBlockTime(blockTime, intervalTime);
    }

    void AdcBlockReceivePlugin::procProtLoadSettings(QSettings &set) {
        double dval;
        bool ok;
        double blockTime = m_receiver->blockTime();
        double blockInterval = m_receiver->blockInterval();

        dval = set.value("blockTime", blockTime).toDouble(&ok);
        if (ok && (dval > 0))
            blockTime = dval;

        dval = set.value("intervalTime", blockInterval).toDouble(&ok);
        if (ok && (dval > 0))
            blockInterval = dval;
        if (blockInterval < blockTime)
            blockInterval = blockTime;
        m_receiver->setBlockTime(blockTime, blockInterval);
    }

    void AdcBlockReceivePlugin::procProtSaveSettings(QSettings &set) {
        set.setValue("blockTime", m_receiver->blockTime());
        set.setValue("intervalTime", m_receiver->blockInterval());
    }

	void AdcBlockReceivePlugin::processBlock(QList<QSharedPointer<DataBlock> > blockList) {
		emit receiveBlock(blockList);
    }
}
