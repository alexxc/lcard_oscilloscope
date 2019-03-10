#ifndef LQMEAS_LOGLEVEL_H
#define LQMEAS_LOGLEVEL_H

#include <QString>
#include "lqmeas_config.h"

#ifndef LQMEAS_NO_GUI
    #include <QColor>
#endif

namespace LQMeas {
    class LogLevel {
    public:
        enum Number {
            Invalid,
            FatalError,
            Error,
            Warning,
            Info,
            Detail,
            DebugHigh,
            DebugMedium,
            DebugLow
        } ;

        Number number() const {return m_lvl;}
#ifndef LQMEAS_NO_GUI
        QColor color() const {return m_color;}
#endif
        QString name() const {return m_text;}


        const bool isLessCritical(const LogLevel *lvl) const {return m_lvl > lvl->number();}

        LogLevel() : m_lvl(Invalid) {}
    private:
        LogLevel(Number num, Qt::GlobalColor col) : m_lvl(num) {
#ifndef LQMEAS_NO_GUI
            m_color = col;
#endif
        }

        LogLevel(Number num) : m_lvl(num) {}
        void setText(QString txt) {m_text = txt;}

        Number  m_lvl;
        QString m_text;
#ifndef LQMEAS_NO_GUI
        QColor  m_color;
#endif


        friend class Log;
    };
}


#endif // LQMEAS_LOGLEVEL_H

