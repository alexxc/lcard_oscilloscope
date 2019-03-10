#ifndef LQDEVPLUGIN_H
#define LQDEVPLUGIN_H

#include <QObject>
#include "lqmeas/devs/resolver/DevicesResolver.h"

class LQDevPlugin : public QObject {
    Q_OBJECT
public:
    explicit LQDevPlugin(QObject *parent = 0);

    enum ActionTypes {
        ActionTypeDevConfig,
        ActionTypeDevIfaceConfig,
        ActionTypeDevUpdateFirmware,
        ActionTypeAsyncIO
    };




    virtual LQMeas::DevicesResolver* resolver() const = 0;
    virtual QStringList devnames() const = 0;
    virtual void actionExec(ActionTypes type, QSharedPointer<LQMeas::Device> dev, QWidget *parent) const = 0;
    virtual bool actionEnabled(ActionTypes type, QSharedPointer<LQMeas::Device> dev) const = 0;
    virtual ActionTypes defaultAction() const {return ActionTypeDevConfig;}
};

#endif // LQDEVPLUGIN_H
