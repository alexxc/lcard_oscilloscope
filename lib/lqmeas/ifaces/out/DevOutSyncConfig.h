#ifndef LQMEAS_DEVOUTSYNCCONFIG_H
#define LQMEAS_DEVOUTSYNCCONFIG_H

#include "DacSignals/DacSignal.h"
#include "DigSignals/DigSignal.h"
#include "lqmeas/Error.h"
#include <QVector>
#include <QSettings>
#include <QSharedPointer>

namespace LQMeas {
    class DevOutInfo;

    /* Конфигурация сигналов и каналов для вывода. Конфигурация конкретного устройства
       может быть унаследована и от данного класса. Реализует хранение
       и сохранение/загрузку сигналов и настроек каналов на вывод.

       */
    /** @todo OutRamSyncMode сделать сохранение в зависимости от свойств устройства */
    class DevOutSyncConfig {
    public:
        /** Режим работы для всех каналов с режимом #OutChModeRam */
        enum OutSyncRamMode {
            OutSyncRamModeCycle = 0,   /** Циклический режим (автогенератора) */
            OutSyncRamModeStream = 1,  /** Потоковый режим */
        };

        /** Режим генерации сигнала для канала */
        enum OutSyncChGenMode {
            OutSyncChGenModeRam = 0, /** сигнал генерируется по точкам программным образом */
            OutSyncChGenModeHard = 1 /** сигнал генерируется аппаратно по параметрам */
        };

        DevOutSyncConfig(const DevOutInfo *devinfo);
        DevOutSyncConfig(const DevOutSyncConfig *cfg);

        /* Настройки предустановки значения на вывод */
        template < class ValType> struct Preset {
            enum Type {
                /* Перед запуском синхронного вывода устанавливается
                   первое значение сигнала на выходы */
                TypeFirstPoint = 0,
                /* Перед запуском синхронного вывода асинхронно
                   устанавлиется указанное явно значение */
                TypeSpecValue  = 1,
                /* Не происходит никакого асинхронного вывода на выходы
                   перед стартом. т.е. остается старое значение до
                   запуска синхронной выдачи */
                TypeDontChange = 2
            };

            Type type;
            ValType value; /* значение, выводимое при type == TypeSpecValue */

            Preset() : type(TypeFirstPoint), value(0) {
            }
        } ;

        struct DacPreset : public Preset<double> {
        };

        struct DigPreset : public Preset<unsigned> {
        };

        /* Установленная в данный момент частота вывода в Гц (на каждый канал) */
        virtual double outSyncGenFreq() const = 0;


        /* Разрешен ли указанный канал ЦАП */
        bool outSyncDacChEnabled(int ch) const {return m_dacParams.at(ch).enabled;}
        /* Разрешен ли указанный канал цифровых линий */
        bool outSyncDigChEnabled(int ch) const {return m_digParams.at(ch).enabled;}

        /* Разрешение заданного канала ЦАП */
        virtual void outSyncDacChEnable(int ch, bool en);
        /* Запрет заданного канала ЦАП */
        virtual void outSyncDigChEnable(int ch, bool en);

        /* Получить установленный сигнал для указанного выхода ЦАП */
        QSharedPointer<DacSignal> outSyncDacSignal(int ch) const {return m_dacParams.at(ch).sig;}
        /* Задание сигнала для указанного выхода ЦАП */
        virtual Error outSyncDacSetSignal(int ch, QSharedPointer<DacSignal> signal);

        /* Получить установленный сигнал для указанного цифрового выхода */
        QSharedPointer<DigSignal> outSyncDigSignal(int ch) const {return m_digParams.at(ch).sig;}
        /* Задание сигнала для указанного цифрового выхода */
        virtual Error outSyncDigSetSignal(int ch, QSharedPointer<DigSignal> signal);

        /* Очистка всех сигналов, установленных с помощью outDacSetSignal()/
            outDigSetSignal() */
        virtual void outSyncClearSignals();


        /* текущие настройки предустановленного значения канала ЦАП */
        DacPreset outSyncDacPreset(int ch) const {return m_dacParams.at(ch).preset;}
        /* установить настройки предустановленного значения канала ЦАП */
        virtual Error outSyncDacSetPreset(int ch, DacPreset &val);

        /* текущие настройки предустановленного значения цифровой линии*/
        DigPreset outSyncDigPreset(int ch) const {return m_digParams.at(ch).preset;}
        /* установить настройки предустановленного значения цифровой линии*/
        virtual Error outSyncDigSetPreset(int ch, DigPreset &val);


        /* Режим генерации указанного канала ЦАП */
        virtual OutSyncChGenMode outSyncDacChGenMode(int ch) const {return m_dacParams.at(ch).gen_mode;}
        /* Режим генерации указанной цифровой линии */
        virtual OutSyncChGenMode outSyncDigChGenMode(int ch) const {return m_digParams.at(ch).gen_mode;}

        virtual Error setOutSyncDacChGenMode(int ch, OutSyncChGenMode mode);


        /* Текущий режим работы для каналов, генерируемых по точкам из RAM */
        OutSyncRamMode outSyncRamMode() const {return m_outRamSyncMode;}

        virtual Error outSyncSetRamMode(OutSyncRamMode mode);




        /* Количество каналов ЦАП в данном экземпляре модуля */
        int outSyncDacChannelsCount() const {return m_dacParams.size();}
        /* Количество цифровых линий в данном экземпляре модуля */
        int outSyncDigChannelsCount() const {return m_digParams.size();}

        /* Загрузка установленных сигналов и признака разрешения для
            всех каналов */
        Error outSyncLoadSignalConfig(QSettings &set);
        /* Сохранение установленных сигналов и признака разрешения для
            всех каналов */
        Error outSyncSaveSignalConfig(QSettings &set) const;

    private:
        void outSyncUpdateDevInfo(const DevOutInfo *devinfo);


        template<class SigType, class PresetType> struct ChParams {
        public:
            bool enabled;
            PresetType preset;
            QSharedPointer<SigType> sig;
            OutSyncChGenMode gen_mode;
        protected:
            void init(bool init_en, QSharedPointer<SigType> init_sig, PresetType init_preset, OutSyncChGenMode init_gen_mode) {
                enabled = init_en;
                sig = init_sig;
                preset = init_preset;
                gen_mode = init_gen_mode;
            }
        };


        struct DacChParams : public ChParams< DacSignal, DacPreset> {
        public:
            DacChParams(const DacChParams *ch_par);
            DacChParams(bool init_en = false,
                        QSharedPointer<DacSignal> init_sig = QSharedPointer<DacSignal>(),
                        DacPreset init_preset = DacPreset(),
                        OutSyncChGenMode gen_mode = OutSyncChGenModeRam);
        };

        struct DigChParams : public ChParams< DigSignal, DigPreset> {
        public:
            DigChParams(const DigChParams *ch_par);
            DigChParams(bool init_en = false,
                        QSharedPointer<DigSignal> init_sig = QSharedPointer<DigSignal>(),
                        DigPreset init_preset = DigPreset(),
                        OutSyncChGenMode gen_mode = OutSyncChGenModeRam);
        };



        OutSyncRamMode m_outRamSyncMode;
        QVector<DacChParams> m_dacParams;
        QVector<DigChParams> m_digParams;
        bool m_ram_mode_configurable;

        friend class DeviceConfig;
    };
}

#endif // LQMEAS_DEVOUTSYNCCONFIG_H
