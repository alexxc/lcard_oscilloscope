#ifndef LBOOTOUTRECORD_H
#define LBOOTOUTRECORD_H

#include <QObject>
#include <QString>
#include <QHash>

class LbootOutRecord : public QObject {
    Q_OBJECT
public:
    LbootOutRecord(QString line);

    /* варианты статусов */
    static QString statusStart() {return "start";}
    static QString statusProgress() {return "progress";}
    static QString statusDone() {return "done";}
    static QString statusError() {return "error";}
    static QString statusWarning() {return "warning";}

    /* варианты этапов выполнения */
    static QString stageCheckParams()           {return "check_params";}
    static QString stageOpenDevice()            {return "open_device";}
    static QString stageCheckInfo()             {return "check_info";}
    static QString stageGetDevMode()            {return "get_devmode";}
    static QString stageCheckDevMode()          {return "check_devmode";}
    static QString stageGetBootVersion()        {return "get_bootver";}
    static QString stageGetDevFlags()           {return "get_devflags";}
    static QString stageGetBootFeatures()       {return "get_features";}
    static QString stageCheckBootVer()          {return "check_bootver";}
    static QString stageWriteProtData()         {return "wr_prot_data";}
    static QString stageWriteFirmware()         {return "write_firmware";}
    static QString stageWriteSign()             {return "write_sign";}
    static QString stageRecovery()              {return "recovery";}
    static QString stageStartApp()              {return "start_app";}


    static QString paramCplSize()               {return "cpl_size";}
    static QString paramFullSize()              {return "full_size";}
    static QString paramErrCode()               {return "err_code";}
    static QString paramErrType()               {return "err_type";}
    static QString paramErrString()             {return "err_str";}

    static QString errorTypeUtility()             {return "utility";}
    static QString errorTypeIface()               {return "interface";}
    static QString errorTypeDevice()              {return "device";}


    /** коды ошибок программы загрузчика */
    typedef enum {
        /** опция заданная в командной строке не поддерживается */
        LBOOT_ERR_INVALID_OPTION       = -1,
        /** недостаточное количество аргументов */
        LBOOT_ERR_INSUFFICIENT_ARGUMENTS  = -2,
        /** указан неверный интерфейс для передачи прошивки устройству */
        LBOOT_ERR_INVALID_INTERFACE       = -3,
        LBOOT_ERR_FIRMWARE_FILE_OPEN      = -4,
        LBOOT_ERR_SIGNATURE_FILE_OPEN     = -5,
        LBOOT_ERR_HASH_FILE_OPEN          = -6,
        LBOOT_ERR_MEMORY_ALLOC            = -7,
        LBOOT_ERR_ALLOC_RESOURCE          = -8,
        LBOOT_ERR_DEVICE_NOT_FOUND        = -9,
        LBOOT_ERR_IOCTRL_FAILED           = -10,
        LBOOT_ERR_INVALID_DEVICE          = -11,
        LBOOT_ERR_INVALID_SERIAL          = -12,
        LBOOT_ERR_INVALID_DEVMODE         = -13,
        /** загрузчик не поддерживает возможность загрузки, без запуска основной программы */
        LBOOT_ERR_UNSUP_FLAG_DONT_START   = -14,
        /** загрузчик не поддерживает возможность восстановления основной прошивки по команде */
        LBOOT_ERR_UNSUP_FLAG_RECOVERY     = -15,
        /** указан неверный фармат вывода в опции --output-fmt */
        LBOOT_ERR_INVALID_OUTPUT_FMT      = -16,
        /** загрузчик не поддерживает запись защищенного блока данных */
        LBOOT_ERR_UNSUP_WR_PROT_DATA      = -17,
        /** ошибка открытия файла с даннымы для записи в защищенный блок */
        LBOOT_ERR_PROT_DATA_FILE_OPEN     = -18,
        /** неверно задана скорость RS */
        LBOOT_ERR_MBRTU_INVALID_BAUDRATE = -100,
        /** неверно задана четность для порта RS */
        LBOOT_ERR_MBRTU_INVALID_PARITY   = -101,
        /** неверно задан адрес устройства на шине Modbus */
        LBOOT_ERR_MBRTU_INVALID_ADDRESS  = -102,
        /** ошибка создания контекста Modbus */
        LBOOT_ERR_MBRTU_CREATE_CONTEXT   = -103,
        /** ошибка функции modbus_set_slave() */
        LBOOT_ERR_MBRTU_SET_ADDR         = -104,
        /** ошибка открытия указанного порта */
        LBOOT_ERR_MBRTU_PORT_OPEN        = -105,
        /** ошибка чтения регистров modbus */
        LBOOT_ERR_READ_REGISTERS         = -106,
        LBOOT_ERR_WRITE_REGISTERS        = -107,

        /** Ошибка создания сокета */
        LBOOT_ERR_TFTP_SOCK_CREATE   = -200,
        /** Ошибка передачи данных */
        LBOOT_ERR_TFTP_SEND          = -201,
        /** Ошибка передачи данных */
        LBOOT_ERR_TFTP_RECV          = -202,
        /** Неверный формат принятого пакета */
        LBOOT_ERR_TFTP_RECV_PKT_FMT  = -203,
        /** Ошибка передачи данных */
        LBOOT_ERR_TFTP_TIMEOUT       = -204,
        /** Неверный формат адреса */
        LBOOT_ERR_TFTP_IP_ADDR       = -205,
        /** Неверный номер порта */
        LBOOT_ERR_TFTP_UDP_PORT      = -206,
        LBOOT_ERR_TFTP_BLOCK_SIZE    = -207,
        /** Принят пакет ERR по TFTP */
        LBOOT_ERR_TFTP_ERR_PACKET    = -208,
        LBOOT_ERR_TFTP_RX_OVERFLOW   = -209,
        LBOOT_ERR_TFTP_RX_INSUF_SIZE = -210,
        LBOOT_ERR_TFTP_PACKET_SKIP   = -211,
        LBOOT_ERR_TFTP_WSA_INIT      = -212,

        /** Передана неверная подпись */
        LBOOT_ERR_UNCORRECT_SIGN                = -1000,
        /** Попытка записать нестабильную версию прошивки в качестве резервной копии
            (определяется по флагу в определенном месте в прошивке) */
        LBOOT_ERR_UNSTABLE_RECOV_WR             = -1001,
        /** Передаваймый файл прошивки предназначен для другого устройства
            (название устройства в прошивке не совпало с тербуемым (переданным
             в параметре запроса от прилоения или дефолтным)) */
        LBOOT_ERR_FIRMWARE_FOR_INVALID_DEVICE    = -1002,
        /** Неверный признак команды начала записи в прошивку */
        LBOOT_ERR_UNCORRECT_START_SIGN           = -1003,
        /** Нельзя записать данный тип прошивки (основной или резервной). Возникает
            либо если идет попытка записать резервную, когда приложение в запросе
            это запретило, либо в случае, если идет попытка перезаписать единственно
            верную на текущий момент прошивку (напрмер попытка переписать основную,
            когда нет резервной) */
        LBOOT_ERR_WR_DISABLED                    = -1004,
        /** Ошибка записи во flash-память блока прошивки (аппратная)*/
        LBOOT_ERR_FLASH_WRITE                    = -1005,
        /** Неверный стартовый адрес в записываемой прошивке - вектор сборса
            указывает на место не внутри прошивки */
        LBOOT_ERR_UNCORRECT_FIRM_RESET_ADDR      = -1006,
        /** Неверная длина данных в управляющем запросе */
        LBOOT_ERR_INVALID_REQ_DATA_LENGTH        = -1007,
        /** Неверная операция. В данном режиме работы загрузчика эта операция
            не может быть выполнена */
        LBOOT_ERR_INVALID_OPERATION              = -1008,
        /** Попытка записать упакованную прошивку, когда данная версия загрузчика
            ее не поддерживает */
        LBOOT_ERR_UNSUP_UNPACK                   = -1009,
        /** Упакованная прошивка закончилась, хотя формат архива требует еще данных */
        LBOOT_ERR_UNPACK_NOT_COMPLETE            = -1010,
        /** Ошибка проверки целостности записанной прошивки после завершения записи */
        LBOOT_ERR_CHECK_WRITE_FIRM               = -1011,
        /** Попытка записать в сектор, где прошивки не должно быть. Возникает
            при размере прошивки, выходящем за размер отведенного для нее места в
            памяти */
        LBOOT_ERR_INVALID_SECTOR                 = -1012,
        /** Попытка записать прошивку без подписи, когда приложение в запросе этого
            не разрешило */
        LBOOT_ERR_FIRM_WITHOUT_SIGN_DISABLED     = -1013,
        /** Неверный хеш прошивки (при записи без подписи) */
        LBOOT_ERR_INVALID_HASH                   = -1014,
        /** Попытка записать прошивку для разработчиков, когда приложение этого не разрешило */
        LBOOT_ERR_DEVELOP_FIRM_DISABLE           = -1015,
        /** Неподдерживаемый код специальной команды */
        LBOOT_ERR_INVALID_CMD_CODE               = -1016,
        /** Неверные параметры специальной команды */
        LBOOT_ERR_INVALID_CMD_PARAMS             = -1017,
        /** Неверная сигнатура команды */
        LBOOT_ERR_INVALID_CMD_SIGN               = -1018,
        /** Нет резервной копии прошивки */
        LBOOT_ERR_RESERV_COPY_NOT_PRESENT        = -1019,
        /** Нет основной прошивки */
        LBOOT_ERR_APPLICATION_NOT_PRESENT        = -1020,
        /** Неверный размер записываемой прошивки (выходит за допустимые пределы) */
        LBOOT_ERR_INVALID_APPL_SIZE              = -1021,
        /** Неверный размер записываемых данных защищенных пользовательских */
        LBOOT_ERR_PROT_DATA_INVALID_SIZE         = -1022,
        /** Запись пользовательских данных запрещена аппаратно */
        LBOOT_ERR_PROT_DATA_WR_DISABLED          = -1023,
        /** Ошибка проверки правильности данных после записи */
        LBOOT_ERR_PROT_DATA_CHECK                = -1024,
        /** Неверный адрес регистра при чтении информации загрузчика */
        LBOOT_ERR_INVALID_REG_ADDR               = -1025,
        /** Операция отменена другим запросом */
        LBOOT_ERR_ABORTED_BY_ANOTHER_REQ         = -1026,
        /** Неверный формат упакованного файла */
        LBOOT_ERR_INVALID_XZ_FORMAT              = -1027,
        /** Неверный формат подписи */
        LBOOT_ERR_INVALID_SIGN_FORMAT            = -1028,
        /** Ошибка при сравнении записанного содержимого во flash-память */
        LBOOT_ERR_FLASH_COMPARE                  = -1029,
        /** Ошибка стирания flash-памяти */
        LBOOT_ERR_FLASH_ERASE                    = -1030,
        /** Не выбрана прошивка для записи/чтения */
        LBOOT_ERR_CUR_FIRMWARE_NOT_SELECTED      = -1031,
        /** Неизвествный тип прошивки */
        LBOOT_ERR_UNKNOWN_FIRMWARE_TYPE          = -1032,

        LCRYPT_ERR_PGP_UNSUF_DATA                    = -10000,
        LCRYPT_ERR_PGP_SIGN_FORMAT                   = -10001,  /**< неверный формат PGP файла */
        LCRYPT_ERR_PGP_SIGN_UNSUPPORT_CRYPT_ALG      = -10002,  /**< неподдерживаемый алгоритм шифрования для подписи */
        LCRYPT_ERR_PGP_SIGN_UNSUPPORT_HASH_ALG       = -10003  /**< неподдерживаемый алгоритм вычисления hash-функции */
    } ErrorCodes;


    QString stageName();

    bool valid() const {return !m_stage.isEmpty();}
    QString stage() const {return m_stage;}
    QString status() const {return m_status;}
    QString param(QString name) const {return m_params[name];}

    QString errorDescr() const;
    QString errorTypeDescr() const;
    int errorCode() const;

private:

    QString m_stage;
    QString m_status;
    QHash<QString, QString> m_params;
};

#endif // LBOOTOUTRECORD_H
