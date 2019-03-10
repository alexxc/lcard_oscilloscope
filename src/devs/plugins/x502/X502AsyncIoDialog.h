#ifndef X502ASYNCIODIALOG_H
#define X502ASYNCIODIALOG_H

#include <QDialog>
#include "lqmeas/devs/x502/x502.h"

class QLedIndicator;
class QPushButton;
class QDoubleSpinBox;

namespace Ui {
class X502AsyncIoDialog;
}

class X502AsyncIoDialog : public QDialog {
    Q_OBJECT

public:
    explicit X502AsyncIoDialog(QWidget *parent, const QSharedPointer<LQMeas::x502>& dev);
    ~X502AsyncIoDialog();



protected:
    void changeEvent(QEvent *e);
private Q_SLOTS:
    void asyncOut();
    void asyncIn();
    void asyncOutHexChanged();
    void asyncOutPinChanged();
    void asyncOutDacPressed();
private:
    void addOutPin(int pos, int pin_num, const QString& name);
    void addInPin(int pos, int pin_num, const QString& name);
    void addDac(int dac_num);
	void updateOutIndicators(signed val);

    struct AsyncPinState {
        int pin_num;
        QLedIndicator *led;

        AsyncPinState(int init_pin, QLedIndicator *init_led) :
            pin_num(init_pin), led(init_led) {}
    } ;

    struct AsyncDacState {
        int dac_ch;
        QPushButton *btn;
        QDoubleSpinBox *valBox;
    };

    QSharedPointer<LQMeas::x502> m_dev;

    QList<AsyncPinState> m_outStates;
    QList<AsyncPinState> m_inStates;
    QList<AsyncDacState> m_dacStates;

    Ui::X502AsyncIoDialog *ui;
	signed m_last_out_val{};
    bool m_out_async_update;
};

#endif // X502ASYNCIODIALOG_H
