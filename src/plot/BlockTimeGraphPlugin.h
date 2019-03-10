#ifndef LQMEASSTUDIO_BLOCKTIMEGRAPHPLUGIN_H
#define LQMEASSTUDIO_BLOCKTIMEGRAPHPLUGIN_H


#include "DataBlockProcessPlugin.h"

class QCPGraph;

namespace LQMeasStudio {
    class Plot;
    /* Плагин для вывода на временной график блока данных */
	class BlockTimeGraphPlugin : public DataBlockProcessPlugin {
        Q_OBJECT
    public:
		QString typePluginName() const {return "BlockTimeGraphPlugin";}

        static QString param_timeGraphData() {return "timeGraphData";}

		BlockTimeGraphPlugin(QObject *parent = nullptr);

        void procStartPrepare();
        void procStop();
        void procClear();
        void retranslate();

        void blockProcess(QSharedPointer<DataBlock> block);
        void blockShowResult(QSharedPointer<DataBlock> block);
        void blockDropResult(QSharedPointer<DataBlock> block);
    protected:
        void procProtLoadSettings(QSettings &set);
        void procProtSaveSettings(QSettings &set);

    private slots:
        void onChannelVisabilityChanged(bool visible);
    private:
        Plot *m_plot;
		bool processing;
//        QList<QCPGraph *> m_graphs;
    };
}
#endif // LQMEASSTUDIO_BLOCKTIMEGRAPHPLUGIN_H
