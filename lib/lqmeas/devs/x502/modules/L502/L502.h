#ifndef LQMEAS_L502_H
#define LQMEAS_L502_H

#include "lqmeas/devs/x502/x502.h"
#include <QHostAddress>

class x502Resolver;

namespace LQMeas {
    class L502 : public x502 {
        Q_OBJECT
    public:
        size_t outCycleGenMaxSize(const OutRamSignalGenerator *generator) const;
    protected:
        const x502Info *createInfo(const t_x502_info *info) const;
    private:
        explicit L502(const t_x502_devrec *info);

        friend class x502Resolver;
    };
}


#endif // LQMEAS_L502_H
