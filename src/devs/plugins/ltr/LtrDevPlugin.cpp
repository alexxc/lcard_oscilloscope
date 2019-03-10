#include "LtrDevPlugin.h"
#include "lqmeas/devs/ltr/LtrResolver.h"

#include "lqmeas/devs/ltr/modules/ltr25.h"


LtrDevPlugin::LtrDevPlugin(QObject *parent) :
    LQDevPlugin(parent) {

}

LQDevResolver *LtrDevPlugin::resolver() const {
    return LtrResolver::resolver();
}

QStringList LtrDevPlugin::devnames() const {
    return QStringList() << LTR25::typeModuleName();
}

void LtrDevPlugin::actionExec(LQDevPlugin::ActionTypes type, QSharedPointer<LQMeasDevice> dev, QWidget *parent) const {

}

bool LtrDevPlugin::actionEnabled(LQDevPlugin::ActionTypes type, QSharedPointer<LQMeasDevice> dev) const {
    return false;
}
