#ifndef LQMEAS_E502_H
#define LQMEAS_E502_H

#include "lqmeas/devs/x502/x502.h"
#include <QHostAddress>



namespace LQMeas {
    class x502Resolver;

    class E502 : public x502 {
        Q_OBJECT
    public:
        QHostAddress ipAddr() const;

        size_t outCycleGenMaxSize(const OutRamSignalGenerator *generator) const;

    protected:
        const x502Info *createInfo(const t_x502_devrec *devrec) const;
        const x502Info *createInfo(const t_x502_info *info) const;
    private:
        explicit E502(const t_x502_devrec *info);

        friend class x502Resolver;
    };
}
#endif // E502_H
