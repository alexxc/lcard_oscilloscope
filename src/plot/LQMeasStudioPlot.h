#ifndef LQMEASSTUDIO_PLOT_H
#define LQMEASSTUDIO_PLOT_H

#include "qcustomplot.h"
#include "PlotConfigDialog.h"
#include <QTimer>
class QSettings;

namespace LQMeasStudio {
    class Plot : public QCustomPlot {
        Q_OBJECT
    public:
		explicit Plot(QWidget *parent = nullptr);
        ~Plot();

		void loadConfig(QSettings &set, QString key, PlotConfigDialog::Config *def_cfg = nullptr);
        void saveConfig(QSettings &set);


        bool xAutoScale() const {return m_cfg.x.autoScale;}
        bool yAutoScale() const {return m_cfg.y.autoScale;}

        void setCfgDecimals(int x_dec, int y_dec);

    public slots:
        void updateAxisAutoScale();
		void requestReplot();

	private slots:
		void processingReplotRequest();

    protected:
        void mouseDoubleClickEvent(QMouseEvent * event);

    private:
        void loadAxeSettings(QSettings &set, PlotConfigDialog::AxeConfig &cfg);
        void saveAxeSettings(QSettings &set, PlotConfigDialog::AxeConfig cfg);
        void updateCfg();
        QString m_key;
        PlotConfigDialog::Config m_cfg;
        bool m_autosave;
		bool replotRequested;
		bool replotting;

		QTimer *m_updateTimer;
        int m_x_dec, m_y_dec;

    };
}

#endif // LQMEASSTUDIO_PLOT_H
