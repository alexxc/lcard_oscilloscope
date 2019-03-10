#include "ChannelInfo.h"

#include <utility>

#include <utility>


namespace LQMeasStudio {
    QString ChannelInfo::uniqueName() const {
        return m_dev->devStr() + " - " + m_name;
    }

    void ChannelInfo::setVisible(bool visible) {
        if (visible!=m_visible) {
             m_visible = visible;
             emit visibilityChanged(visible);
        }
    }

    ChannelInfo::ChannelInfo(int num, QSharedPointer<LQMeas::Device> dev, int dev_ch_num, QString name, QColor color) :
        m_ch_num(num), m_dev(std::move(std::move(dev))), m_dev_ch_idx(dev_ch_num), m_name(std::move(std::move(name))), m_color(std::move(std::move(color))), m_visible(true) {

    }
}

