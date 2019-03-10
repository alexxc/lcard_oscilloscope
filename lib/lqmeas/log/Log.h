#ifndef LQMEAS_LOG_H
#define LQMEAS_LOG_H

#include <QObject>
#include <QHash>
#include "LogMessage.h"



#define LQMeasLog LQMeas::Log::instance()

namespace LQMeas {
    class Device;

    class Log : public QObject {
        Q_OBJECT
    public:
        static Log *instance();


        void retranslate();
        QList<LogLevel> logLevels() const;
        const LogLevel level(LogLevel::Number num) const;

    Q_SIGNALS:
        void message(QSharedPointer<LogMessage> msg);
    public Q_SLOTS:


        void report(LogLevel::Number lvl, QString msg,
                    Device *dev = 0, Error err = Error::Success());

        void info(QString msg, Device *dev = 0) {
            report(LogLevel::Info, msg, dev, Error::Success());
        }

        void detail(QString msg, Device *dev = 0) {
            report(LogLevel::Detail, msg, dev, Error::Success());
        }

        void warning(QString msg, Device *dev = 0) {
            report(LogLevel::Warning, msg, dev);
        }

        void error(QString msg, Error err, Device *dev = 0) {
            report(LogLevel::Error, msg, dev, err);
        }


    private:
        explicit Log();
        void initLevel(LogLevel::Number lvl, Qt::GlobalColor col);

        QHash<LogLevel::Number, LogLevel> m_lvlsCfg;
        /* уровни журнала в заданном порядке от более критического
           к менее - чтобы сохранять порядок при возврате, т.к. ключив в
           QHash могут менять порядок */
        QList<LogLevel::Number> m_lvls;
    };
}

#endif // LQMEAS_LOG_H
