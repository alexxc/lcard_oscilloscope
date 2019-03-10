#ifndef LQMEAS_LCOMPSEQADCCONFIG_H
#define LQMEAS_LCOMPSEQADCCONFIG_H

#include "lqmeas/devs/Device.h"
#include "lqmeas/ifaces/in/DevAdcStdSeq32Config.h"


namespace LQMeas {
    /* Вся функциональность данного класса вынесена в DevAdcStdSeq32Config,
     * однако он оставлен, для возможности дальнейшего введения вспомогательных
     * функций, общих для lcomp устройств с коммутацией каналов */
    class lcompSeqAdcConfig : public DevAdcStdSeq32Config {
    public:
        lcompSeqAdcConfig(const DevAdcInfo *info);
        lcompSeqAdcConfig(const lcompSeqAdcConfig *cfg);
    };
}


#endif // LCOMPSEQADCCONFIG_H
