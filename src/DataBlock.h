#ifndef LQMEASSTUDIO_DATABLOCK_H
#define LQMEASSTUDIO_DATABLOCK_H

#include "in/ChannelInfo.h"
#include <QSharedPointer>
#include <QVector>
#include <QVariant>

namespace LQMeasStudio {
    struct DataBlock {
    public:
        DataBlock(QSharedPointer<ChannelInfo> ch, QVector<double> vals, double startx, double dt) :
			m_ch(ch), m_startX(startx), m_dt(dt), m_vals(vals) {}

        QSharedPointer<ChannelInfo> channel() const {return m_ch;}
        double startx() const {return m_startX;}
        double dt() const {return m_dt;}        
        QVector<double> values() const {return m_vals;}


        QVariant parameter(QString name) const {return m_properties[name];}
        void setParameter(QString name, QVariant value) {m_properties[name] = value;}
        bool hasParameter(QString name) const {return m_properties.keys().contains(name);}
        void removeParameter(QString name) {m_properties.remove(name); }
    private:
        QSharedPointer<ChannelInfo> m_ch;
        double m_startX;
        double m_dt;
        QVector<double> m_vals;

        QHash<QString, QVariant> m_properties;

    };
}



#endif // LQMEASSTUDIO_DATABLOCK_H

