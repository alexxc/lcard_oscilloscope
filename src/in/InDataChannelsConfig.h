#ifndef LQMEASSTUDIO_INDATACHANNELSCONFIG_H
#define LQMEASSTUDIO_INDATACHANNELSCONFIG_H

#include <QObject>
#include <QVector>

#include "ChannelInfo.h"


namespace LQMeasStudio {
    class InDataChannelsConfig : public QObject {
        Q_OBJECT
    public:
        QVector<QSharedPointer<ChannelInfo> > channels() const {return m_channels;}
    signals:
        void append(QSharedPointer<ChannelInfo> );
        void cleared();
    public slots:
        QSharedPointer<ChannelInfo> add(QSharedPointer<LQMeas::Device> dev, int dev_ch_idx, QString name);
        void clear();

    private:
		InDataChannelsConfig(QObject *parent = nullptr);
        QVector<QSharedPointer<ChannelInfo> > m_channels;

        friend class Environment;
    };
}

#endif // LQMEASSTUDIO_INDATACHANNELSCONFIG_H
