#include "SpectrumGraphPlugin.h"
#include "LQMeasStudio.h"
#include "LQMeasStudioPlot.h"
#include "proc/SpectrumPlugin.h"
#include <QMetaType>

Q_DECLARE_METATYPE (QSharedPointer<QVector<double> >)

namespace LQMeasStudio {
    SpectrumGraphPlugin::SpectrumGraphPlugin(QObject *parent) :
        DataBlockProcessPlugin(typePluginName(), true, parent) {

        m_plot = new Plot();

        LQMeasStudioEnv->mainWidgetLayout()->addWidget(m_plot);
    }

    QStringList SpectrumGraphPlugin::procPluginDependencyNames() const {
		return QStringList() << typePluginName();
    }

    void SpectrumGraphPlugin::procStartPrepare() {
        foreach (QSharedPointer<ChannelInfo> ch, LQMeasStudioEnv->channelsConfig()->channels()) {
            QCPGraph *graph = m_plot->addGraph();
            graph->setName(ch->uniqueName());
            graph->setPen(ch->color());

            connect(ch.data(), SIGNAL(visibilityChanged(bool)),
                    SLOT(onChannelVisabilityChanged(bool)));
        }
    }

    void SpectrumGraphPlugin::procStop() {

    }

    void SpectrumGraphPlugin::procClear() {
        m_plot->clearGraphs();
        m_plot->requestRelpot();
    }

    void SpectrumGraphPlugin::retranslate() {
        m_plot->xAxis->setLabel(tr("Frequency, Hz"));
        m_plot->yAxis->setLabel(tr("Spectrum, dB"));

        m_plot->requestRelpot();
    }

    void SpectrumGraphPlugin::blockProcess(QSharedPointer<DataBlock> block) {
        if (block->hasParameter(SpectrumPlugin::param_spectrum())) {
            QSharedPointer< QVector<double> > fft = block->parameter(SpectrumPlugin::param_spectrum()).
                    value<QSharedPointer< QVector<double> > >();
            double df = block->parameter(SpectrumPlugin::param_df()).toDouble();
            LQMeas::DevAdc *adc = block->channel()->device()->devAdc();
            if (adc) {
                double range_val = adc->adcConfig()->adcChRangeMaxVal(adc->adcConfig()->adcChNum(block->channel()->devChIdx()));
                QCPDataMap *data = new QCPDataMap();
                for (int i=0; i < fft->size(); i++) {
                    double y = 20.0*log10(fft->at(i)/(range_val/sqrt(2.)));
                    double x = df*i;
                    data->insert(x, QCPData(x,y));
                }
                block->setParameter(param_spectrumGraphData(), QVariant::fromValue((void*)data));
            }
        }
    }

    void SpectrumGraphPlugin::blockShowResult(QSharedPointer<DataBlock> block) {
        if (block->hasParameter(param_spectrumGraphData())) {
            QCPDataMap *data = static_cast<QCPDataMap *>(block->parameter(param_spectrumGraphData()).value<void*>());
            int idx = block->channel()->num();
            if (idx < m_plot->graphCount()) {
                m_plot->graph(idx)->setData(data);
                m_plot->updateAxisAutoScale();
                m_plot->requestRelpot();
            } else {
                delete data;
            }
            block->removeParameter(param_spectrumGraphData());
        }
    }

    void SpectrumGraphPlugin::blockDropResult(QSharedPointer<DataBlock> block) {
        if (block->hasParameter(param_spectrumGraphData())) {
            QCPDataMap *data = static_cast<QCPDataMap *>(block->parameter(param_spectrumGraphData()).value<void*>());
            delete data;
            block->removeParameter(param_spectrumGraphData());
        }
    }

    void SpectrumGraphPlugin::procProtLoadSettings(QSettings &set)  {
        PlotConfigDialog::Config defcfg;
        defcfg.x.autoScale = true;
        defcfg.x.min = 0;
        defcfg.x.max = 100000;
        defcfg.y.autoScale = false;
        defcfg.y.min = -140;
        defcfg.y.max = 0;
        m_plot->loadConfig(set, "fftPlot", &defcfg);
        m_plot->setCfgDecimals(3,2);
        m_plot->xAxis->grid()->setSubGridVisible(true);
    }

    void SpectrumGraphPlugin::procProtSaveSettings(QSettings &set) {
        m_plot->saveConfig(set);
    }

    void SpectrumGraphPlugin::procProtSetEnabled(bool enabled) {
        m_plot->setVisible(enabled);
    }

    void SpectrumGraphPlugin::onChannelVisabilityChanged(bool visible) {
        ChannelInfo *info = qobject_cast<ChannelInfo*>(sender());
        if (info) {
            int idx = info->num();
            if (idx < m_plot->graphCount()) {
                m_plot->graph(idx)->setVisible(visible);
                m_plot->requestRelpot();
            }
        }
    }

}
