#ifndef X502DEVPLUGIN_H
#define X502DEVPLUGIN_H

#include "devs/plugins/LQDevPlugin.h"

class X502DevPlugin : public LQDevPlugin {
    Q_OBJECT
public:
	explicit X502DevPlugin(QObject *parent = nullptr);

    LQMeas::DevicesResolver *resolver() const;
    QStringList devnames() const;

    void actionExec(ActionTypes type, QSharedPointer<LQMeas::Device> dev, QWidget *parent) const;
    bool actionEnabled(ActionTypes type, QSharedPointer<LQMeas::Device> dev) const;

private:
    LQMeas::Error devIfaceConfig(const QSharedPointer<LQMeas::Device>& dev, QWidget *parent) const;
    LQMeas::Error devUpdateFirmware(const QSharedPointer<LQMeas::Device>& dev, QWidget *parent) const;
};

#endif // X502DEVPLUGIN_H
