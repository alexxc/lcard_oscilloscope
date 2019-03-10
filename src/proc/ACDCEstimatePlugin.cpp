#include "ACDCEstimatePlugin.h"
#include "LQMeasStudio.h"
#include "ChannelSignalParamPanel.h"
#include "lmath.h"

namespace LQMeasStudio {
    ACDCEstimatePlugin::ACDCEstimatePlugin(QObject *parent) :
        DataBlockProcessPlugin(typePluginName(), true, parent) {

        m_param_id_dc  = LQMeasStudioEnv->chSignalParamsPanel()->addParam("DC");
        m_param_id_ac  = LQMeasStudioEnv->chSignalParamsPanel()->addParam("AC");
    }

    void ACDCEstimatePlugin::blockProcess(QSharedPointer<DataBlock> block) {
        double ac, dc;
        lmath_acdc_estimation(block->values().data(), block->values().size(),
                                    &dc, &ac);
        block->setParameter(param_ac(), ac);
        block->setParameter(param_dc(), dc);
    }

    void ACDCEstimatePlugin::blockShowResult(QSharedPointer<DataBlock> block) {
        double ac = block->parameter(param_ac()).toDouble();
        double dc = block->parameter(param_dc()).toDouble();
        LQMeasStudioEnv->chSignalParamsPanel()->setParam(block->channel(), m_param_id_dc, dc, 'f', 6);
        LQMeasStudioEnv->chSignalParamsPanel()->setParam(block->channel(), m_param_id_ac, ac, 'f', 6);
    }
}
