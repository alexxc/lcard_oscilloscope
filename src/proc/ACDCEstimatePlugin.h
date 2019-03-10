#ifndef ACDCESTIMATEPLUGIN_H
#define ACDCESTIMATEPLUGIN_H

#include "DataBlockProcessPlugin.h"
/* Плагин рассчитывает AC и DC составляющую в блоке данных и устанавливает
   значение в панеле параметров */
namespace LQMeasStudio {

    class ACDCEstimatePlugin : public DataBlockProcessPlugin {
        Q_OBJECT
    public:
		QString typePluginName() const {return "ACDCEstimatePlugin";}

        static QString param_ac() {return "AC";}
        static QString param_dc() {return "DC";}

		explicit ACDCEstimatePlugin(QObject *parent = nullptr);

        virtual void blockProcess(QSharedPointer<DataBlock> block);
        virtual void blockShowResult(QSharedPointer<DataBlock> block);
    private:
        int m_param_id_dc, m_param_id_ac;
    };
}

#endif // ACDCESTIMATEPLUGIN_H
