#ifndef OUTSIGNALCONFIGCTL_H
#define OUTSIGNALCONFIGCTL_H

#include <QObject>
#include <QHash>
#include "lqmeas/ifaces/out/DevOutInfo.h"
#include "lqmeas/ifaces/out/DevOutSyncConfig.h"

class QLineEdit;
class QCheckBox;
class QWidget;

class OutSignalConfigCtl : QObject {
    Q_OBJECT
public:
    OutSignalConfigCtl(QWidget *parent, QSharedPointer<LQMeas::DevOutSyncConfig> cfg,
                       const LQMeas::DevOutInfo *devInfo, QList<QCheckBox*> chEnBoxList,
                       QList<QLineEdit *> chSigEditList, bool change_enabled);
    ~OutSignalConfigCtl();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
private Q_SLOTS:
    void onEnabledChanged(int state);
    void onTextChanged(void);
private:
    void showSigText(int ch);


    int m_dacChCnt;



    QWidget *m_parentWgt;
    QSharedPointer<LQMeas::DevOutSyncConfig> m_cfg;
    const LQMeas::DevOutInfo *m_info;
    QList<QCheckBox *> m_chEnBoxList;
    QList<QLineEdit *> m_chSigEditList;
};

#endif // OUTSIGNALCONFIGCTL_H
