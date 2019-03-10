#ifndef LTRDEVPLUGIN_H
#define LTRDEVPLUGIN_H

#include "devs/plugins/LQDevPlugin.h"

class LtrDevPlugin : public LQDevPlugin {
    Q_OBJECT
public:
    explicit LtrDevPlugin(QObject *parent = 0);

    LQDevResolver *resolver() const;
    virtual QStringList devnames() const;

    void actionExec(ActionTypes type, QSharedPointer<LQMeasDevice> dev, QWidget *parent) const;
    bool actionEnabled(ActionTypes type, QSharedPointer<LQMeasDevice> dev) const;
};

#endif // X502DEVPLUGIN_H
