#ifndef MULTIMETERPROCESSTHREAD_H
#define MULTIMETERPROCESSTHREAD_H

#include "MultimeterThread.h"

namespace LQMeas {
    class MultimeterProcessor : public QObject {
        Q_OBJECT

    public slots:
        void processData(LQMeas::DevAdc *adc, QVector<double> vals, LQMeas::MultimeterThread::MultimeterData data);
    signals:
        void dataProcessed(LQMeas::MultimeterThread::MultimeterData data);
    };
}

#endif // MULTIMETERPROCESSTHREAD_H
