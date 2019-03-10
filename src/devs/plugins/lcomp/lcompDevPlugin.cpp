#include "lcompDevPlugin.h"
#include "lqmeas/devs/lcomp/lcompResolver.h"
#include "lqmeas/devs/lcomp/e14_440.h"
#include "lcompDevSeqAdcCfgDialog.h"

lcompDevPlugin::lcompDevPlugin(QObject *parent) {

}

LQDevResolver *lcompDevPlugin::resolver() const {
    return lcompResolver::resolver();
}

QStringList lcompDevPlugin::devnames() const {
    return QStringList() << e14_440::typeDeviceName();
}

void lcompDevPlugin::showCfgDialog(QSharedPointer<LQMeasDevice> dev, QWidget *parent) const {
    QSharedPointer<lcompStreamDevice> lcompDev = qSharedPointerObjectCast<lcompStreamDevice>(dev);
    if (lcompDev) {
        lcompDevSeqAdcCfgDialog dialog(lcompDev, parent);
        dialog.exec();
    }
}
