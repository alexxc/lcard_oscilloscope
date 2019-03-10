#ifndef E502NETBROWSER_H
#define E502NETBROWSER_H

#include "lqmeas/devs/resolver/DeviceNetworkBrowser.h"
namespace LQMeas {
    class e502NetBrowser : public DeviceNetworkBrowser    {
        Q_OBJECT
    public:
        e502NetBrowser();

    protected:
        void run();
    };
}

#endif // E502NETBROWSER_H
