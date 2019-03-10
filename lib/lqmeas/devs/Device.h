#ifndef LQMEAS_DEVICE_H
#define LQMEAS_DEVICE_H

#include <QObject>
#include "lqmeas/StdErrors.h"
#include "lqmeas/devs/x502/x502Config.h"

class QSettings;

namespace LQMeas {
    class DeviceInfo;
    class DeviceTypeInfo;
    class DeviceConfig;
    class DevicesResolver;

    class DevAdc;
    class DevAdcInfo;
    class DevInAsyncDig;
    class DevOutInfo;
    class DevOutSync;
    class DevOutAsyncDac;
    class DevOutAsyncDig;

    /* Базовый класс устройства, от которого наследуются остальные модули.

       Сам класс не должен создаваться вручную, а его должны создавать
       соответствующие классы поиска устройств, унаследованные от DevicesResolver.

       Включает в себя:
          - информацию о устройстве (объединенную в отдельный класс DeviceInfo)
          - методы для открытия/закрытия связи с устройством
          - методы для конфигурации устройства (см. ниже)
          - методы для получения указателей на поддерживаемые устройством общие программные
            интерфейсы

       Информация о устройстве резделена на два класса:
          - DeviceTypeInfo - включает информацию о поддерживаемых возможностях
                             самого типа или его определенной модификации. Эти
                             свойства являются постоянными для данной модификации модуля.
          - DeviceInfo     - информация о конкретном экземпляре устройства
                             (серийный номер, версии прошивок и т.д.).
       Если информация не действительна до открытия устройства, то сперва создается
            информация с пустыми параметрами устройства и типом по умолчанию, который
            потом уже заменяется на действительную информацию (как правило после открытия
            первого соединения).
       Также сам класс устройства содержит информацию о подключении, такую как
            интерфейс подключения.

       Конфигурация устройств.
          Для установки конфигурации устройства используется метод configure,
          в который передается указатель на конфигурацию. По этому методу
          вся конфигурация записывается в устройство и при успехе обновляются
          и значения конфигурации в самом классе.

          Для каждого устройства (который поддерживает конфигурацию) конфигурация
          представляет собой класс <имя устройства>Config, унаследованный
          от DeviceConfig, и содержащий все настройки устройства. Также каждый
          класс конфигурации реализует загрузку и сохранение всех настроек
          через QSettings.

          Текущую конфигурацию устройства можно получить с помощью метода
          currentConfig() (в виде базового класса DeviceConfig). Также для
          удобства классы конкретных устройств реализуют метод devspecConfig(),
          чтобы получить класс конфигурации, приведенных к специализированному
          типу.

          Изменение полученного таким образом класса конфигурации невозможно
          напрямую.
          Для изменения следует создать копию (либо с помощью метода clone(),
          либо с помощью конструктора копирования), изменить ее параметры и
          заново вызвать configure().

    ***************************************************************************/
    class Device : public QObject {
        Q_OBJECT
    public:
       /* статус выполнения операции устройством */
        enum OpStatus {
            OpStatusStart,
            OpStatusFinish,
            OpStatusError,
            OpStatusProgress
        };

        /* варианты аппаратного интерфейса, по которому могут быть
         *  подключены устройства */
        enum DeviceInterface {
            InterfaceUnknown    = 0,
            InterfaceUSB        = 1,     
            InterfacePCI        = 2,
            InterfaceEthernet   = 3,
            InterfaceLTR        = 4 /* для модулей LTR подключенных к крейту */
        };


        virtual ~Device();

        /* ------------------  Информация о устройстве -----------------------*/
        /* информация о типе устройства */

        const DeviceInfo *devInfo() const {return m_info;}
        const DeviceTypeInfo *devTypeInfo() const;

        /* название типа устройства (без модификаций) */
        QString name() const;
        /* название устройства с учетом модификаций */
        QString modificationName() const;
        /* серийный номер устройства */
        QString serial() const;
        /* аппаратный интерфейс по которому подключено устройство */
        virtual DeviceInterface iface() const {return InterfaceUnknown;}
        /* строка с названием аппаратного интерфейса устройства */
        QString ifaceName() const;
        /* строка с дополнительной информацией о расположении устройства
           (содержимое зависит от аппаратного интерфейса) */
        virtual QString location() const {return QString();}
        /* строка для краткого отображения устройства (имя, сер. номер) */
        virtual QString devStr() const;
        /* строка для отображения устройства с учетом поля location() */
        virtual QString devStrFull() const;
        /* ключ для сохранения информации о устройстве */
        virtual QString devKey() const;



        /* установлена ли связь с устройством */
        virtual bool isOpened() const = 0;

        /* ------------- Получение программных интерфейсов -------------*/

        /* если к устройству могут быть подключены дочерние устройства,
         *  то данный метод возвращает интерфейс для получения списка этих устройств */
		virtual DevicesResolver *childResolver() {return nullptr;}

        /* Интерфейс для генерации синхронный сигналов. Если поддержан, то должен
           быть поддержан и devOutInfo() */
		virtual DevOutSync *devOutSync() {return nullptr;}
        /* Указатель на интерфейс для асинхронного вывода на ЦАП. Если поддержан, то должен
           быть поддержан и devOutInfo() */
		virtual DevOutAsyncDac *devOutAsyncDac() {return nullptr;}
        /* Указатель на интерфейс для асинхронного вывода на цифровые линии. Если поддержан, то должен
           быть поддержан и devOutInfo() */
		virtual DevOutAsyncDig *devOutAsyncDig() {return nullptr;}

        /* Указатель на интерфейс для потокового ввода данных (или 0, если не поддерживается) */
		virtual DevAdc *devAdc() {return nullptr;}
        /* Указатель на интерфейс для асинхронного ввод цифровых линий */
		virtual DevInAsyncDig *devInAsyncDig() {return nullptr;}


        /* Получить класс с текущей конфигурацией устройства.
           Нельзя изменять параметры данного класса напрямую - для создания
           измененной конфигурации нужно создавать копию */
		const x502Config *currentConfig() const {return m_cfg;}
    public Q_SLOTS:
        /* установить соединение с устройством (до этого большинство функций не действительны) */
        Error open();
        /* Закрыть соединение с устройством */
        Error close();
        /* Конфигурация устройства  в соответствии параметрами из указанного класса конфигурации..
         *  Если указан нулевой указатель, то будут просто повторно записаны последние установленные
         *  настройки модуля */
		Error configure(const x502Config *cfg = nullptr);
        /* загрузка параметров конфигурации из настроек из группы
               с именем key (относительно текущей группы в QSettings) и
               конфигурация устройства в соответствии с загруженными
               параметрами */
        Error loadConfig(QSettings &set, QString key);
        /* сохранение параметров конфигурации в настройки в группу
               с именем key (относительно текущей группы в QSettings).
               Если key - пустая строка, то используется название из
               последнего вызова loadConfig()/saveConfig() для данного 
               устройства */
        Error saveConfig(QSettings &set, QString key = QString());

    Q_SIGNALS:
        /* данным сигналом устройство может оповещать о ходе выполнения длительной
         *  операции (загрузка прошивки и т.п.).
         *  status - указывает статус выполнения операции
         *  descr    - строка с описанием выполняемой операции
         *  done    - указывает прогресс выполнения (относительно size)
         *  size       - значение прогресса, соответствующие завершению операции
         *  err         - значение ошибки, при status == opStatusError */
        void opProgress(LQMeas::Device::OpStatus status, QString descr, int done, int size, LQMeas::Error err);
        /* сигнал оповещает при изменении информации о устройстве */
        void devDescrChanged();
    protected:
        virtual Error protOpen()=0;
        virtual Error protClose()=0;
        virtual Error protConfigure(const DeviceConfig *cfg) = 0;

        void setDevicInfo(const DeviceInfo *info);

        DeviceConfig *mutableConfig() const {return m_cfg;}
        void sendOpProgess(OpStatus status, QString descr, int done, int size, Error err);
		explicit Device(x502Config *defaultCfg, const DeviceInfo *info, QObject *parent = nullptr);
    private:
		x502Config *m_cfg;
        const DeviceInfo *m_info;

        friend class DevAdc;
    };
}
#endif // LQMEAS_DEVICE_H
