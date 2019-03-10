#include "LogModel.h"
#include <QBrush>
#include <QMutexLocker>
#include "Log.h"
#include <QSettings>

namespace LQMeas {
    LogModel::LogModel(QObject *parent) : QAbstractTableModel(parent) {
        QSettings set;
        set.beginGroup("LogModel");
        setLogLevel(Log::instance()->level((LogLevel::Number)set.value("loglvl", LogLevel::Detail).toInt()));
        set.endGroup();

        connect(Log::instance(), SIGNAL(message(QSharedPointer<LogMessage>)),
                SLOT(addNewMessage(QSharedPointer<LogMessage>)));
    }

    LogModel::~LogModel() {
        QSettings set;
        set.beginGroup("LogModel");
        set.setValue("loglvl", m_lvl.number());
        set.endGroup();
    }

    int LogModel::rowCount(const QModelIndex &parent) const {
        return m_showMsgList.size();
    }

    int LogModel::columnCount(const QModelIndex &parent) const {
        return COLUMN_CNT;
    }

    QVariant LogModel::data(const QModelIndex &index, int role) const {
        QVariant ret;
        if (index.isValid()) {
            QSharedPointer<LogMessage> msg = m_showMsgList.at(index.row());
            if (role == Qt::ForegroundRole) {
                ret = QBrush(msg->level().color());
            } else if (role == Qt::DisplayRole) {
                switch (index.column()) {
                    case COLUMN_TIME:
                        ret = msg->time().toString("h:mm:ss, d.MM.yyyy");
                        break;
                    case COLUMN_LEVEL: {
                            QString levelStr = msg->level().name();
                            if (showErrRequired(msg)) {
                                levelStr.append(QString(" (%1)").arg(QString::number(msg->error().errorCode())));
                            }
                            ret = levelStr;
                        }
                        break;
                    case COLUMN_MSG: {
                            QString txt;
                            txt.append(msg->text());
                            if (showErrRequired(msg)) {
                                txt.append(QString(": %1").arg(msg->error().msg()));
                            }
                            ret = txt;
                        }
                        break;

                }
            } else if (role == Qt::ToolTipRole) {
                ret = msg->text();
            }
        }

        return ret;
    }

    Qt::ItemFlags LogModel::flags(const QModelIndex &index) const {
        if (!index.isValid())
            return 0;

        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    QVariant LogModel::headerData(int section, Qt::Orientation orientation, int role) const {
        QVariant ret;
        if ((role == Qt::DisplayRole) && (orientation==Qt::Horizontal)) {
            switch (section) {
                case COLUMN_TIME:
                    ret = tr("Record time");
                    break;
                case COLUMN_LEVEL:
                    ret = tr("Level");
                    break;
                case COLUMN_MSG:
                    ret = tr("Text");
                    break;
                default:
                    break;
            }
        }
        return ret;
    }

    void LogModel::setLogLevel(LogLevel lvl) {
        m_lvl = lvl;
        redrawLogRecords();
    }


    void LogModel::addNewMessage(QSharedPointer<LogMessage> msg) {
        QMutexLocker lock(&m_lock);
        addMessageList(QList<QSharedPointer<LogMessage> >() << msg);
        m_allMsgList.append(msg);
    }

    void LogModel::addMessageList(QList<QSharedPointer<LogMessage> > msgList) {
        QList<QSharedPointer<LogMessage> > newMsgList;
        foreach (QSharedPointer<LogMessage> msg, msgList) {
            if (!msg->level().isLessCritical(&m_lvl)) {
                newMsgList.append(msg);
            }
        }

        if (newMsgList.size()) {
            beginInsertRows(QModelIndex(), m_showMsgList.size(), m_showMsgList.size()+newMsgList.size()-1);
            m_showMsgList += newMsgList;
            endInsertRows();
        }
    }

    void LogModel::redrawLogRecords() {
        QMutexLocker lock(&m_lock);

        if (m_showMsgList.size()!=0) {
            beginRemoveRows(QModelIndex(), 0, m_showMsgList.size()-1);
            m_showMsgList.clear();
            endRemoveRows();
        }

        addMessageList(m_allMsgList);
    }

    bool LogModel::showErrRequired(QSharedPointer<LogMessage> msg) const {
        return ((msg->level().number() == LogLevel::FatalError)  ||
                    (msg->level().number() == LogLevel::Error) ||
                    (msg->level().number() == LogLevel::Warning)) &&
                    !msg->error().isSuccess();
    }

    void LogModel::clear() {
        m_allMsgList.clear();
        redrawLogRecords();
    }
}
