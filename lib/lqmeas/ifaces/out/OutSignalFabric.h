#ifndef LQMEAS_OUTSIGNALFABRIC_H
#define LQMEAS_OUTSIGNALFABRIC_H

#include <QObject>
#include <QSharedPointer>
#include "DacSignals/DacSignal.h"
#include "DigSignals/DigSignal.h"

class QSettings;

namespace LQMeas {
    class OutSignalFabric : QObject {
        Q_OBJECT
    public:
        static const OutSignalFabric *instance();


        QSharedPointer<DacSignal> createDacSignal(QSettings &set, QString key = QString()) const;
        QSharedPointer<DigSignal> createDigSignal(QSettings &set, QString key = QString()) const;
        void saveSignal(QSharedPointer<OutSignal> signal, QSettings &set, QString key = QString()) const;

        QString createSignalDescription(QSharedPointer<OutSignal> signal) const;
    private:
        explicit OutSignalFabric();
    };
}

#endif // LQMEAS_OUTSIGNALFABRIC_H
