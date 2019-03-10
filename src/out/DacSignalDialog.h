#ifndef DACSIGNALDIALOG_H
#define DACSIGNALDIALOG_H

#include <QDialog>

#include "lqmeas/ifaces/out/DacSignals/DacSignal.h"

namespace Ui {
class DacSignalDialog;
}
namespace LQMeas {
    class DevOutSyncConfig;
    class DevOutInfo;
}

class DacSignalParamUi;


class DacSignalDialog : public QDialog {
    Q_OBJECT

public:
    explicit DacSignalDialog(QWidget *parent, const LQMeas::DevOutInfo *info, LQMeas::DevOutSyncConfig *cfg, int ch);
    ~DacSignalDialog();

    QSharedPointer<LQMeas::DacSignal> getSignal();
public Q_SLOTS:
    void accept();
private Q_SLOTS:
    void setUiByIdx(int idx);
    void updatePresetValState();
private:
    void removeRow(int row);

    int m_const_item_cnt;
    Ui::DacSignalDialog *ui;
    QList<DacSignalParamUi*> m_sigUi;
    LQMeas::DevOutSyncConfig *m_cfg;
    const LQMeas::DevOutInfo *m_info;
    int m_ch;
    DacSignalParamUi *m_curSigUi;
    QSharedPointer<LQMeas::DacSignal>  m_curSignal;

};

#endif // DACSIGNALDIALOG_H
