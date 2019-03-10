#include "InDataChannelsConfig.h"

#include <utility>


static QColor f_colors[] = {Qt::blue, Qt::red, Qt::green, Qt::magenta,
                            Qt::darkCyan, Qt::yellow, Qt::darkGreen, Qt::black};

#define COLORS_CNT (sizeof(f_colors)/sizeof(f_colors[0]))


namespace LQMeasStudio {
    QSharedPointer<ChannelInfo> InDataChannelsConfig::add(QSharedPointer<LQMeas::Device> dev, int dev_ch_idx, QString name) {
        int num = m_channels.size();
        QSharedPointer<ChannelInfo>  chInfo = QSharedPointer<ChannelInfo> (
                    new ChannelInfo(num, std::move(dev), dev_ch_idx, std::move(name), f_colors[num % COLORS_CNT]));

        m_channels.append(chInfo);
        emit append(chInfo);
        return chInfo;
    }

    void InDataChannelsConfig::clear() {
        m_channels.clear();
        emit cleared();
    }

    InDataChannelsConfig::InDataChannelsConfig(QObject *parent) :
        QObject(parent) {
        qRegisterMetaType< QSharedPointer<ChannelInfo>  >("QSharedPointer<ChannelInfo>");
    }
}



