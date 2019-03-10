#ifndef LQMEAS_DEVOUTSYNC_H
#define LQMEAS_DEVOUTSYNC_H

#include "lqmeas/Error.h"
#include "DevOutSyncConfig.h"
#include "OutRamSignalGenerator.h"

namespace LQMeas {
    class Device;
    class DevOutSyncModeImpl;
    class DevOutSyncStatusTracker;

    /***********************************************************************//**
        @brief Интерфейс для синхронного вывода аналоговых и/или цифровых сигналов.

        Интерфейс служит для реализации генерации синхронных сигналов.
        Устройство, поддерживающее данный интерфейс, должно также поддержать
        интерфейс DevOutInfo, который содержит информацию о поддерживаемых
        возможностях вывода устройства.

        Изначальная настройка сигналов и режимов выполняется через конфигурацию
        устройства, которая должна наследоваться от DevOutSyncConfig.
        Для вывода сигнала необходимо как установить действительный сигнал
        для канала, так и резрешить его генерацию.

        Помимо сигнала можно настроить предустановленное значение, которое будет
        устанавливаться асинхронно перед запуском синхронного вывода, т.е.
        это значение будет установленно до момента запуска синхронизации и
        с него начнется вывод.

        Этот интерфейс может служить как для генерации сигналов в циклическом, так
        и потоковом режиме, а также для генерации сигналов аппаратными средствами,
        без рассчета.

        Типичный вариант использования:
          - настройка модуля и сигналов (в произвольном порядке) через DevOutSyncConfig:
               - установка сигналов для нужных каналов с помощью outSyncDacSetSignal()/outSyncDigSetSignal()
               - установка предустановленных значений с помощью outSyncDacSetPreset()/outSyncDigSetPreset()
               - разрешение интересующих каналов с помощью outSyncDacChEnable()/outSyncDigChEnable()
          - вызов outSyncGenPrepare() для предварительной предзагрузки сигнала
          - вызов outSyncGenStart() по которому начнется генерация
          для останова генерации
          - сперва вызвать outSyncGenStopRequest() по которому будет подан запрос на завершение
          - затем вызвать outSyncGenStopWait(), который вернет управление, когда генерация уже завершена
          для смены сигналов на лету (сейчас не проверялась и не во всех режимах может работать):
          - установить новые сигналы с помощью outSyncDacSetGenSignal()/outSyncDigSetGenSignal()
          - вызвать outSyncGenUpdate() по которому произойдет замена сигналов
            на установленные (только для тех каналов, для которых изменились сигналы)

         Для отслеживаения ошибок при запущенном выводе можно подключится
         к сигналам объекта слежения, получаемого outSyncStatusTracker().
         При ошибках или некоторых событиях вывода, этот объект генерирует
         соответствующие сигналы, если отслеживание этих событий поддерживается
         устройством и режимом


         При наследовании от данного сигнала, необходимо после создания
         объекта добавить классы, реализующие поддержку определенного режима
         с помощью outSyncAddMode(). Есть стандартные реализации потокового
         и циклического вывода, которые требуют поддержки от устройства
         соответствующих стандартных приватных интерфейсов
      ************************************************************************/
    class DevOutSync {
    public:

        /* Признак, запущена ли сейчас генерация данных */
        bool outSyncGenRunning() const;

        /* Данная функция должна вызываться перед началом генерации. По ней
           могут быть выставлены начальные значения, предзагружены отсчеты
           сигналов и т.п. */
        Error outSyncGenPrepare();
        /* Начало генерации сигнала в синхронном режиме */
        Error outSyncGenStart();

        Error outSyncGenStopRequest(unsigned tout = 30000);
        /* Останов генерации сигнала в режиме автогенератора. */
        Error outSyncGenStop();
        /* Обновление сигналов при включенной генерации */
        Error outSyncGenUpdate();


        const DevOutSyncConfig *outSyncConfig() const {return outSyncConfigMutable();}


        /* Задание сигнала для указанного выхода ЦАП */
        virtual Error outSyncDacSetGenSignal(int ch, QSharedPointer<DacSignal> signal);
        /*Получить установленный сигнал для указанного выхода ЦАП */
        QSharedPointer<DacSignal> outSyncDacGenSignal(int ch) const;

        /* Задание сигнала для указанного цифрового выхода */
        virtual Error outSyncDigSetGenSignal(int ch, QSharedPointer<DigSignal> signal);
        /* Получить установленный сигнал для указанного цифрового выхода */
        QSharedPointer<DigSignal> outSyncDigGenSignal(int ch) const;


        DevOutSyncStatusTracker *outSyncStatusTracker() const {return m_statusTracker;}

        virtual ~DevOutSync();

        bool outSyncHasEnabledChannels();
    protected:
        virtual DevOutSyncConfig *outSyncConfigMutable() const = 0;

        DevOutSync(Device *dev);

        /* Добавить реализацию синхронного вывода, поддерживаемую модулем.
           Объект удаляется при удалении DevOutSync, поэтому можно не сохранять
           в классе реализации для удаления*/
        void outSyncAddMode(DevOutSyncModeImpl *impl);

        /* Проверка, что для указанного выхода ЦАП будет выполняться расчет
            точек для загрузки в модуль (канал разрешен, есть действительный
            сигнал и режим OutSyncChGenModeRam) */
        bool outSyncDacValidRamSignal(int ch) const;
        /* Проверка, что для указанного цифрового выхода будет выполняться расчет
            точек для загрузки в модуль (канал разрешен, есть действительный
            сигнал и режим OutSyncChGenModeRam) */
        bool outSyncDigValidRamSignal(int ch) const;
    private:


        /* асинхронная предустановка данных перед синхронным выводом */
        Error presetVals(QSharedPointer<OutRamSignalGenerator> generator);


        QList<DevOutSyncModeImpl*> m_modesImpl;
        DevOutSyncModeImpl *m_curMode;
        DevOutSyncStatusTracker *m_statusTracker;
        enum {
            GenStateStop,
            GenStatePrepared,
            GenStateRunning,
            GenStateStopReq
        } m_gen_state;

        Device *m_dev;
    };
}

#endif // LQMEAS_IDEVOUTSYNC_H
