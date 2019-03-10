#ifndef LQMEASSTUDIO_SPECTRUMGRAPHPLUGIN_H
#define LQMEASSTUDIO_SPECTRUMGRAPHPLUGIN_H

#include "DataBlockProcessPlugin.h"

class QCPGraph;

namespace LQMeasStudio {
    class Plot;
    /* Плагин для вывода на график спектра сигнала, рассчитанного SpectrumPlugin */
    class SpectrumGraphPlugin : public DataBlockProcessPlugin {
        Q_OBJECT
    public:
		QString typePluginName() const {return "SpectrumGraphPlugin";}

        static QString param_spectrumGraphData() {return "SpectrumGraphData";}

		SpectrumGraphPlugin(QObject *parent = nullptr);

        QStringList procPluginDependencyNames() const;

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
        void procProtSetEnabled(bool enabled);

    private slots:
        void onChannelVisabilityChanged(bool visible);
    private:
        Plot *m_plot;
        QList<QCPGraph *> m_graphs;
    };
}

#endif // LQMEASSTUDIO_SPECTRUMGRAPHPLUGIN_H
