#include "BlockTimeGraphPlugin.h"
#include "LQMeasStudio.h"
#include "LQMeasStudioPlot.h"

namespace LQMeasStudio {
    BlockTimeGraphPlugin::BlockTimeGraphPlugin(QObject *parent) :
        DataBlockProcessPlugin(typePluginName(), true, parent) {

		m_plot = new Plot();
		processing = false;

		LQMeasStudioEnv->mainWidgetLayout()->addWidget(m_plot);
    }

    void BlockTimeGraphPlugin::procStartPrepare() {
        foreach (QSharedPointer<ChannelInfo> ch, LQMeasStudioEnv->channelsConfig()->channels()) {
            QCPGraph *timeGraph = m_plot->addGraph();
            timeGraph->setName(ch->uniqueName());
            timeGraph->setPen(ch->color());

            connect(ch.data(), SIGNAL(visibilityChanged(bool)),
                    SLOT(onChannelVisabilityChanged(bool)));
        }
    }

    void BlockTimeGraphPlugin::procStop() {

    }

    void BlockTimeGraphPlugin::procClear() {
        m_plot->clearGraphs();
		m_plot->requestReplot();
    }

    void BlockTimeGraphPlugin::retranslate() {
        m_plot->xAxis->setLabel(tr("Time, ms"));
        m_plot->yAxis->setLabel(tr("Amplitude, V"));
		m_plot->requestReplot();
    }

    void BlockTimeGraphPlugin::blockProcess(QSharedPointer<DataBlock> block) {
		if (processing) {
			return;
		}
		processing = true;
		QCPDataMap *data = new QCPDataMap();
		int n = block->values().size();
		int m = n / 128;
		int o = n - m * 128;
		for (int i = 0; i < n - m - o; ++i) {
			double y = block->values().at(i);
			for (int j = 1; j < m && i + j < n; ++j) {
				y += block->values().at(i + j);
			}
			y /= m;
			double x = 1000.*(block->dt()*(i + m/2) + block->channel()->timeShift());
			data->insert(x, QCPData(x, y));
			i += m;
        }
		block->setParameter(param_timeGraphData(), QVariant::fromValue(static_cast<void*>(data)));
		processing = false;
    }

    void BlockTimeGraphPlugin::blockShowResult(QSharedPointer<DataBlock> block) {
		if (processing) {
			return;
		}
        if (block->hasParameter(param_timeGraphData())) {
            QCPDataMap *data = static_cast<QCPDataMap *>(block->parameter(param_timeGraphData()).value<void*>());
            int idx = block->channel()->num();
            if (idx < m_plot->graphCount()) {
				m_plot->graph(idx)->clearData();
                m_plot->graph(idx)->setData(data);
                m_plot->updateAxisAutoScale();
				m_plot->requestReplot();
            } else {
				delete data;
			}
			block->removeParameter(param_timeGraphData());
        }
    }

    void BlockTimeGraphPlugin::blockDropResult(QSharedPointer<DataBlock> block) {
        if (block->hasParameter(param_timeGraphData())) {
            QCPDataMap *data = static_cast<QCPDataMap *>(block->parameter(param_timeGraphData()).value<void*>());
            delete data;
            block->removeParameter(param_timeGraphData());
        }
    }

    void BlockTimeGraphPlugin::procProtLoadSettings(QSettings &set) {
        PlotConfigDialog::Config defcfg;
        defcfg.x.autoScale = true;
        defcfg.x.min = 0;
        defcfg.x.max = 100;
        defcfg.y.autoScale = true;
        defcfg.y.min = -10;
        defcfg.y.max = 10;
        m_plot->loadConfig(set, "timePlot", &defcfg);
		m_plot->setCfgDecimals(3, 5);

        m_plot->xAxis->grid()->setSubGridVisible(true);
    }

    void BlockTimeGraphPlugin::procProtSaveSettings(QSettings &set) {
        m_plot->saveConfig(set);
    }

    void BlockTimeGraphPlugin::onChannelVisabilityChanged(bool visible) {
        ChannelInfo *info = qobject_cast<ChannelInfo*>(sender());
        if (info) {
            int idx = info->num();
            if (idx < m_plot->graphCount()) {
                m_plot->graph(idx)->setVisible(visible);
				m_plot->requestReplot();
            }
        }
    }

}

