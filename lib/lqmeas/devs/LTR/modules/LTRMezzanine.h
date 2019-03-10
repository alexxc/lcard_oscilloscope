#ifndef LQMEAS_LTRMEZZANINE_H
#define LQMEAS_LTRMEZZANINE_H

#include <QString>
#include <QObject>
#include "LtrModule.h"
#include "lqmeas/units/Units.h"

namespace LQMeas {
    class LTRMezzanine {
    public:
        QString name() const {return m_name;}
        QString serial() const {return m_serial;}
        unsigned chCnt() const {return m_chCnt;}
        virtual Unit unit() const {return Units::V();}

    protected:
        LTRMezzanine(QString name, QString serial, unsigned chCnt) :
            m_name(name), m_serial(serial), m_chCnt(chCnt) {}
    private:
        QString m_name;
        QString m_serial;
        unsigned m_chCnt;
    };
}

#endif // LQMEAS_LTRMEZZANINE_H
