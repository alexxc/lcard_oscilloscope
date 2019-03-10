#include "Log.h"
#include "lqmeas/devs/Device.h"

namespace LQMeas {

    Log *Log::instance() {
        static Log log_instance;
        return &log_instance;
    }

    void Log::retranslate() {
        m_lvlsCfg[LogLevel::FatalError].setText(tr("Critical Errors"));
        m_lvlsCfg[LogLevel::Error].setText(tr("Errors"));
        m_lvlsCfg[LogLevel::Warning].setText(tr("Warnings"));
        m_lvlsCfg[LogLevel::Info].setText(tr("Info"));
        m_lvlsCfg[LogLevel::Detail].setText(tr("Details"));
        m_lvlsCfg[LogLevel::DebugHigh].setText(tr("Debug 1"));
        m_lvlsCfg[LogLevel::DebugMedium].setText(tr("Debug 2"));
        m_lvlsCfg[LogLevel::DebugLow].setText(tr("Debug 3"));
    }

    QList<LogLevel> Log::logLevels() const {
        QList<LogLevel> ret;
        Q_FOREACH (LogLevel::Number num, m_lvls) {
            ret.append(m_lvlsCfg[num]);
        }
        return ret;
    }

    const LogLevel Log::level(LogLevel::Number num) const {
        return m_lvlsCfg[num];
    }

    void Log::report(LogLevel::Number lvl, QString msg, Device *dev, Error err) {
        if (dev)
            msg.prepend(dev->devStr() + ": ");
        Q_EMIT message(QSharedPointer<LogMessage>(new LogMessage(QDateTime::currentDateTime(),
                                                  m_lvlsCfg[lvl], msg, err)));
    }

    Log::Log() {
        qRegisterMetaType<QSharedPointer<LogMessage> >("QSharedPointer<LogMessage>");

        initLevel(LogLevel::FatalError, Qt::darkRed);
        initLevel(LogLevel::Error, Qt::red);
        initLevel(LogLevel::Warning, Qt::darkYellow);
        initLevel(LogLevel::Info, Qt::darkGreen);
        initLevel(LogLevel::Detail, Qt::black);
        initLevel(LogLevel::DebugHigh, Qt::gray);
        initLevel(LogLevel::DebugMedium, Qt::gray);
        initLevel(LogLevel::DebugLow, Qt::gray);

        retranslate();
    }

    void Log::initLevel(LogLevel::Number lvl, Qt::GlobalColor col)     {
        m_lvlsCfg[lvl] = LogLevel(lvl, col);
        m_lvls.append(lvl);
    }
}

