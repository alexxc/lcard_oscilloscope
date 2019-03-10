#ifndef CHANNELINFO_H
#define CHANNELINFO_H

#include <QObject>
#include <QColor>
#include <QSharedPointer>
#include "lqmeas/devs/Device.h"
#include "lqmeas/devs/DeviceConfig.h"
#include "lqmeas/ifaces/in/DevAdcConfig.h"

namespace LQMeasStudio {
    class ChannelInfo : public QObject {
        Q_OBJECT
    public:
         int num() const {return m_ch_num;}
         int devChIdx() const {return m_dev_ch_idx;}
         QString name() const {return m_name;}
         QSharedPointer<LQMeas::Device> device() const {return m_dev;}
         QColor color() const {return m_color;}
         bool visible() const {return m_visible;}

         double timeShift() const {return m_dev->currentConfig()->adcConfig()->adcChTimeShift(m_ch_num);}

         QString uniqueName() const;

    public slots:
        void setVisible(bool visible);
    signals:
        void visibilityChanged(bool visible);

    private:
        ChannelInfo(int num, QSharedPointer<LQMeas::Device> dev, int dev_ch_num, QString name, QColor color);

        int m_ch_num;
        int m_dev_ch_idx;
		QSharedPointer<LQMeas::Device> m_dev;
        QString m_name;
        QColor m_color;
        bool m_visible;

        friend class InDataChannelsConfig;
    };
}

#endif // CHANNELINFO_H
