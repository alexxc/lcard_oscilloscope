#ifndef LCOMPDEVPLUGIN_H
#define LCOMPDEVPLUGIN_H

#include "devs/plugins/LQDevPlugin.h"

class lcompDevPlugin : public LQDevPlugin {
    Q_OBJECT
public:
    explicit lcompDevPlugin(QObject *parent = 0);



    LQDevResolver *resolver() const;
    virtual QStringList devnames() const;
    void virtual showCfgDialog(QSharedPointer<LQMeasDevice> dev, QWidget *parent) const;
};

#endif // LCOMPDEVPLUGIN_H
