#ifndef LTRDLOGMODEL_H
#define LTRDLOGMODEL_H

#include <QAbstractTableModel>
#include <QMutex>

#include "LogMessage.h"


namespace LQMeas {
    class LogModel : public QAbstractTableModel {
        Q_OBJECT
    public:
        explicit LogModel(QObject *parent = 0);
        ~LogModel();


        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QVariant headerData(int section, Qt::Orientation orientation,
                             int role = Qt::DisplayRole ) const;

        LogLevel curLogLevel() const {return m_lvl; }

    public slots:
        void setLogLevel(LogLevel lvl);

        void clear();
    private slots:

        void addNewMessage(QSharedPointer<LogMessage> msg);
        void addMessageList(QList<QSharedPointer<LogMessage> > msgList);


        void redrawLogRecords();
    private:
        static const int COLUMN_TIME   = 0;
        static const int COLUMN_LEVEL  = 1;
        static const int COLUMN_MSG    = 2;
        static const int COLUMN_CNT    = 3;

        bool showErrRequired(QSharedPointer<LogMessage> msg) const;

        QList<QSharedPointer<LogMessage> > m_showMsgList;
        QList<QSharedPointer<LogMessage> > m_allMsgList;
        LogLevel m_lvl;
        QMutex m_lock;

        friend class LogPanel;
    };
}

#endif // LTRDLOGMODEL_H
