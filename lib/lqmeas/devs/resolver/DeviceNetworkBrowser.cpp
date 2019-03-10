#include "DeviceNetworkBrowser.h"
#include <QThread>
#include <QApplication>
#include "lqmeas/log/Log.h"

namespace LQMeas {

void DeviceNetworkBrowser::stopRequest() {
    if (m_thread) {
        m_stop_requested = true;
        m_thread->quit();
    }
}

bool DeviceNetworkBrowser::stopWiat(unsigned long time) {
    bool ret = true;
    if (m_thread) {
        ret = m_thread->wait(time);
        if (ret) {
            delete m_thread;
            m_thread = 0;
        }
    }
    return ret;

}

void DeviceNetworkBrowser::start() {
    m_thread = new QThread();
    m_thread->start();
    moveToThread(m_thread);
    m_stop_requested = false;

    emit startPorcessingRequest();
}

DeviceNetworkBrowser::DeviceNetworkBrowser() : m_stop_requested(false), m_thread(0) {
    qRegisterMetaType<LQMeas::Error>("LQMeas::Error");
    qRegisterMetaType<QSharedPointer<LQMeas::DeviceSvcRecord> >("QSharedPointer<LQMeas::DeviceSvcRecord>");
    qRegisterMetaType<LQMeas::DeviceNetworkBrowser::Event>("LQMeas::DeviceNetworkBrowser::Event");

    connect(this, SIGNAL(startPorcessingRequest()), SLOT(startPorcessing()));

}

DeviceNetworkBrowser::~DeviceNetworkBrowser() {
    if (m_thread) {
        m_thread->wait();
        delete m_thread;
    }
}

void DeviceNetworkBrowser::addRecord(QSharedPointer<DeviceSvcRecord> svcRec) {
        m_curDevs.append(svcRec);
        emit stateChanged(svcRec, EventAdd);
    }

    void DeviceNetworkBrowser::changeRecord(QSharedPointer<DeviceSvcRecord> svcRec) {
        for (int i = 0; i < m_curDevs.size(); i++) {
            if (svcRec->isEqual(m_curDevs[i].data())) {
                m_curDevs[i] = svcRec;
                emit stateChanged(svcRec, EventChange);
                break;
            }
        }
    }

    void DeviceNetworkBrowser::removeRecord(QSharedPointer<DeviceSvcRecord> svcRec) {
        for (int i = 0; i < m_curDevs.size(); i++) {
            if (svcRec->isEqual(m_curDevs[i].data())) {
                emit stateChanged(svcRec, EventRemoved);
                m_curDevs.removeAt(i);
                break;
            }
        }
    }

    void LQMeas::DeviceNetworkBrowser::setError(LQMeas::Error err, QString msg) {
        LQMeasLog->error(msg, err);
        emit error(err, msg);
    }

    void DeviceNetworkBrowser::startPorcessing() {
        run();
        moveToThread(QApplication::instance()->thread());
    }

}

