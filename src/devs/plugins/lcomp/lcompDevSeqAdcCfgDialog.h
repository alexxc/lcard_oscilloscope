#ifndef LCOMPDEVSEQADCCFGDIALOG_H
#define LCOMPDEVSEQADCCFGDIALOG_H

#include <QDialog>
#include "lqmeas/devs/lcomp/lcompStreamDevice.h"


class QTableWidgetItem;
class QComboBox;

namespace Ui {
class lcompDevSeqAdcCfgDialog;
}

class lcompDevSeqAdcCfgDialog : public QDialog {
    Q_OBJECT

public:
    explicit lcompDevSeqAdcCfgDialog(QSharedPointer<lcompStreamDevice> dev, QWidget *parent = 0);
    ~lcompDevSeqAdcCfgDialog();
public slots:
    void accept();
private slots:
    LQMeasError setConfig();
private:
    Ui::lcompDevSeqAdcCfgDialog *ui;

    void setComboBoxItemByData(QComboBox *box, int data);

    struct ChUi {
        QTableWidgetItem *chItem;
        QComboBox *mode;
        QComboBox *range;
    };


    QSharedPointer<lcompStreamDevice> m_dev;
    QList<ChUi> m_chUis;
};

#endif // LCOMPDEVSEQADCCFGDIALOG_H
