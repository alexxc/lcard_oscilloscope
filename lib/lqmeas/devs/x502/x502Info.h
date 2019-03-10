#ifndef LQMEAS_X502INFO_H
#define LQMEAS_X502INFO_H

#include "lqmeas/devs/DeviceInfo.h"
#include "x502TypeInfo.h"

namespace LQMeas {
    class x502Info : public DeviceInfo {
    public:
        x502Info(const x502TypeInfo *type, QString serial, unsigned fpga_ver, unsigned plda_ver, unsigned mcu_ver)
            : DeviceInfo(type, serial), m_fpga_ver(fpga_ver), m_plda_ver(plda_ver), m_mcu_ver(mcu_ver) {
        }

        unsigned pldaVer() const {return m_plda_ver;}
        unsigned fpgaVer() const {return m_fpga_ver;}
        unsigned mcuVer() const {return  m_mcu_ver;}

        virtual bool mcuPresent() const = 0;

        QString pldaVerStr() const {return QString::number(m_plda_ver);}
        QString fpgaVerStr() const {return QString("%0.%1").arg(QString::number((m_fpga_ver >> 8) & 0xFF))
                                                        .arg(QString::number(m_fpga_ver&0xFF));}
        QString mcuVerStr() const {return QString("%0.%1.%2")
                                        .arg(QString::number((m_mcu_ver >> 24) & 0xFF))
                                        .arg(QString::number((m_mcu_ver >> 16) & 0xFF))
                                        .arg(QString::number((m_mcu_ver >>  8) & 0xFF));}


        bool galPresent() const {return devspecTypeInfo()->galPresent();}
        bool dacPresent() const {return devspecTypeInfo()->dacPresent();}
        bool bfPresent() const  {return devspecTypeInfo()->bfPresent();}
        bool ethPresent() const {return devspecTypeInfo()->ethPresent();}
        bool industrial() const {return devspecTypeInfo()->industrial();}

        const x502TypeInfo *devspecTypeInfo() const {return static_cast<const x502TypeInfo *>(type());}

    protected:
        x502Info(const x502Info &info) : DeviceInfo(info), m_fpga_ver(info.m_fpga_ver),
            m_plda_ver(info.m_plda_ver), m_mcu_ver(info.m_mcu_ver) {
        }
    private:
        unsigned m_fpga_ver;
        unsigned m_plda_ver;
        unsigned m_mcu_ver;
    };
}


#endif // LQMEAS_X502INFO_H
