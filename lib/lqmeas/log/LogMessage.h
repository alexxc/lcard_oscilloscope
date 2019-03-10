#ifndef LQMEAS_LOGMESSAGE
#define LQMEAS_LOGMESSAGE


#include <QSharedPointer>
#include <QDateTime>

#include "LogLevel.h"
#include "lqmeas/Error.h"


namespace LQMeas {
    class Log;

    class LogMessage {
    public:
        LogLevel level() const {return m_lvl;}
        Error    error() const {return m_err;}
        QString  text()  const {return m_txt;}
        QDateTime time() const {return m_time;}

    private:
        LogMessage(QDateTime time, LogLevel lvl, QString txt, Error err) :
            m_time(time), m_lvl(lvl), m_txt(txt), m_err(err) {}

        LogLevel m_lvl;
        Error m_err;
        QString m_txt;
        QDateTime m_time;

        friend class Log;
    };
}


#endif // LQMEAS_LOGMESSAGE

