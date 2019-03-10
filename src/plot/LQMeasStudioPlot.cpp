#include "LQMeasStudioPlot.h"
#include <QSettings>


namespace LQMeasStudio {
    Plot::Plot(QWidget *parent) :
        QCustomPlot(parent), m_x_dec(5), m_y_dec(5) {
        m_cfg.x.autoScale = m_cfg.y.autoScale = true;

		m_updateTimer = new QTimer();
		m_updateTimer->setSingleShot(false);
		m_updateTimer->moveToThread(this->thread());
		connect(m_updateTimer, SIGNAL(timeout()), SLOT(processingReplotRequest()));
		m_updateTimer->setInterval(250);
		m_updateTimer->start();
	}

    Plot::~Plot() {
		delete m_updateTimer;
    }

    void Plot::loadConfig(QSettings &set, QString key, PlotConfigDialog::Config *def_cfg) {
		if (def_cfg) {
            m_cfg = *def_cfg;
		}

        m_key = key;

        set.beginGroup(key);
        set.beginGroup("x");
        loadAxeSettings(set, m_cfg.x);
        set.endGroup();
        set.beginGroup("y");
        loadAxeSettings(set, m_cfg.y);
        set.endGroup();
        set.endGroup();
        updateCfg();
    }

    void Plot::saveConfig(QSettings &set) {
        set.beginGroup(m_key);
        set.beginGroup("x");
        saveAxeSettings(set, m_cfg.x);
        set.endGroup();
        set.beginGroup("y");
        saveAxeSettings(set, m_cfg.y);
        set.endGroup();
        set.endGroup();
    }

    void Plot::setCfgDecimals(int x_dec, int y_dec) {
        m_x_dec = x_dec;
        m_y_dec = y_dec;
    }

    void Plot::updateAxisAutoScale() {
        if (xAutoScale())
            xAxis->rescale();
        if (yAutoScale())
            yAxis->rescale();
    }

	void Plot::requestReplot() {
		if (!replotting) {
			replotRequested = true;
		}
    }

	void Plot::processingReplotRequest() {
		if (replotRequested && !replotting) {
			replotRequested = false;
			replotting = true;
			replot();
			replotting = false;
		}
	}

    void Plot::mouseDoubleClickEvent(QMouseEvent *event) {
        QCustomPlot::mouseDoubleClickEvent(event);

        PlotConfigDialog dlg(this, m_cfg, m_x_dec, m_y_dec);
		if (dlg.exec() == QDialog::Accepted) {
            m_cfg = dlg.config();
            updateCfg();
        }
    }

    void Plot::loadAxeSettings(QSettings &set, PlotConfigDialog::AxeConfig &cfg) {
        QVariant val;
        double dval;
        bool ok;

        dval = set.value("min").toDouble(&ok);
		if (ok) {
            cfg.min = dval;
		}
        dval = set.value("max").toDouble(&ok);
		if (ok) {
            cfg.max = dval;
		}
        val = set.value("autoscale");
		if (val.isValid()) {
            cfg.autoScale = val.toBool();
		}
    }

    void Plot::saveAxeSettings(QSettings &set, PlotConfigDialog::AxeConfig cfg) {
        set.setValue("min", cfg.min);
        set.setValue("max", cfg.max);
        set.setValue("autoscale", cfg.autoScale);
    }

    void Plot::updateCfg() {
        if (m_cfg.x.autoScale) {
            xAxis->rescale();
        } else {
            xAxis->setRange(m_cfg.x.min, m_cfg.x.max);
        }
        if (m_cfg.y.autoScale) {
            yAxis->rescale();
        } else {
            yAxis->setRange(m_cfg.y.min, m_cfg.y.max);
        }
		requestReplot();
    }
}
