#ifndef LQMEAS_OUTRAMSIGNALGENERATOR_H
#define LQMEAS_OUTRAMSIGNALGENERATOR_H

#include "DacSignals/DacSignal.h"
#include "DigSignals/DigSignal.h"
#include "lqmeas/Error.h"
#include <QVector>
#include <QList>
#include <QSharedPointer>

namespace LQMeas {

    /* Класс предназначен для генерации точек заданного набора сигналов для последующей
       отправки в устройство.
       Перед генерацией необходимо вызвать genSignalsInit(), а затем вызывать
       genNextData() для получения новых точек для отправки в устройство */
    class OutRamSignalGenerator {
    public:
        /* сигналы создаются при создании. dac_chs и dig_chs задают номера каналов, которым
           соответствуют указанные сигналы */
        OutRamSignalGenerator(QList<QSharedPointer <DacSignal> > dac_signals, QList<int> dac_chs,
                                   QList<QSharedPointer <DigSignal> > dig_signals, QList<int> dig_chs);



        int dacSignalsCount() const {return m_dac_signals.size();}
        int digSignalsCount() const {return m_dig_signals.size();}

        int dacSignalChannel(int sig_num) const {return m_dac_ch_nums.at(sig_num);}
        int digSignalChannel(int sig_num) const {return m_dig_ch_nums.at(sig_num);}

        QList<QSharedPointer <DacSignal> > dacSignals() const {return m_dac_signals;}
        QList<QSharedPointer <DigSignal> > digSignals() const {return m_dig_signals;}
        QList<QSharedPointer <OutSignal> > allSignals() const {return m_all_signals;}


        /* Инициализация генерации сигналов. Переводит все сигналы в состояние
           начала генерации, т.е. чтобы genNextData() вернул первые точки сигналов */
        void genSignalsInit();


        /* генерация следующих points точек для всех сигналов выходных
           каналов. Отсчеты ЦАП чередуются для каждого канала.
           размер dac_data будет равен dacSignalsCount() * point.
           Цифровые линии - в виде битовой маски по номеру канала (не меняется
           от количества разрешенных каналов) - поддерживается только 32 выхода
        */
        Error genNextData(size_t points, QVector<double> &dac_data,
                          QVector<unsigned> &dig_data);

    private:
        QList<QSharedPointer <DacSignal> > m_dac_signals;
        QList<QSharedPointer <DigSignal> > m_dig_signals;
        QList<QSharedPointer <OutSignal> > m_all_signals;
        QList<int> m_dig_ch_nums;
        QList<int> m_dac_ch_nums;
    };
}

#endif // LQMEAS_OUTRAMSIGNALGENERATOR_H
