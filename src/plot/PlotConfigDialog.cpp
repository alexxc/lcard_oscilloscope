#include "PlotConfigDialog.h"
#include "ui_PlotConfigDialog.h"

namespace LQMeasStudio {
    PlotConfigDialog::PlotConfigDialog(QWidget *parent, Config cfg, int x_dec, int y_dec) :
        QDialog(parent),
        ui(new Ui::GraphConfigDialog), m_cfg(cfg) {
        ui->setupUi(this);


        ui->XScaleAuto->setChecked(m_cfg.x.autoScale);
        ui->XMin->setValue(m_cfg.x.min);
        ui->XMax->setValue(m_cfg.x.max);
        ui->XMin->setDecimals(x_dec);
        ui->XMax->setDecimals(x_dec);

        ui->YScaleAuto->setChecked(m_cfg.y.autoScale);
        ui->YMin->setValue(m_cfg.y.min);
        ui->YMax->setValue(m_cfg.y.max);
        ui->YMin->setDecimals(y_dec);
        ui->YMax->setDecimals(y_dec);
    }

    PlotConfigDialog::~PlotConfigDialog() {
        delete ui;
    }

    void PlotConfigDialog::accept() {
        m_cfg.x.autoScale = ui->XScaleAuto->isChecked();
        m_cfg.x.min       = ui->XMin->value();
        m_cfg.x.max       = ui->XMax->value();

        m_cfg.y.autoScale = ui->YScaleAuto->isChecked();
        m_cfg.y.min       = ui->YMin->value();
        m_cfg.y.max       = ui->YMax->value();
        QDialog::accept();
    }
}
