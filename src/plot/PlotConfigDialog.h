#ifndef LQMEASSTUDIO_PLOTCONFIGDIALOG_H
#define LQMEASSTUDIO_PLOTCONFIGDIALOG_H

#include <QDialog>


namespace Ui {
class GraphConfigDialog;
}

namespace LQMeasStudio {
    class PlotConfigDialog : public QDialog {
        Q_OBJECT

    public:
        struct AxeConfig {
            bool autoScale;
            double min;
            double max;
        };

        struct Config {
            AxeConfig x;
            AxeConfig y;
        };



        explicit PlotConfigDialog(QWidget *parent, Config cfg, int x_dec=5, int y_dec=5);
        ~PlotConfigDialog();

        Config config() const {return m_cfg;}

    public slots:
        virtual void accept();

    private:
        Ui::GraphConfigDialog *ui;
        Config m_cfg;
    };
}

#endif // LQMEASSTUDIO_PLOTCONFIGDIALOG_H
