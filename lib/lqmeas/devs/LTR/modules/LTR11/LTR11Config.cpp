#include "LTR11Config.h"
#include "LTR11.h"
#include "LTR11TypeInfo.h"
#include "ltr/include/ltr11api.h"
#include <QSettings>

namespace LQMeas {
    QString LTR11Config::typeConfigName() {
        return LTR11TypeInfo::defaultInfo()->deviceTypeName();
    }

    LTR11Config::LTR11Config() : DevAdcStdSeq32Config(LTR11TypeInfo::defaultInfo()) {
        m_convMode = ConvModeInternal;
        m_startMode = StartModeInternal;
    }

    LTR11Config::LTR11Config(const LTR11Config *cfg) : DevAdcStdSeq32Config(cfg) {
        m_convMode = cfg->m_convMode;
        m_startMode = cfg->m_startMode;
    }

    void LTR11Config::setStartMode(LTR11Config::StartMode mode) {
        if ((mode == StartModeInternal) ||  (mode == StartModeExtRise) || (mode == StartModeExtFall))
            m_startMode = mode;
    }

    void LTR11Config::setConvMode(LTR11Config::ConvMode mode) {
        if ((mode == ConvModeInternal) || (mode == ConvModeExtRise) || (mode == ConvModeExtFall))
            m_convMode = mode;
    }

    Error LTR11Config::protLoadConfig(QSettings &set) {
        int intval;
        bool ok;

        intval = set.value("SyncStartMode", StartModeInternal).toInt(&ok);
        if (ok && ((intval>=StartModeInternal) || (intval<=StartModeExtFall))) {
            setStartMode((StartMode)intval);
        } else {
            reportLoadWarning(LTR11::tr("Invalid sync start mode"));
        }

        intval = set.value("SyncConvMode", ConvModeInternal).toInt(&ok);
        if (ok && ((intval >= ConvModeInternal) && (intval <= ConvModeExtFall))) {
            setConvMode((ConvMode)intval);
        } else {
            reportLoadWarning(LTR11::tr("Invalid sync freq mode"));
        }

        return protAdcLoadConfig(set);
    }

    Error LTR11Config::protSaveConfig(QSettings &set) const {
        set.setValue("SyncStartMode", m_startMode);
        set.setValue("SyncConvMode", m_convMode);
        return protAdcSaveConfig(set);
    }

    Error LTR11Config::protUpdateConfig() {
        return protAdcUpdateConfig();
    }

    void LTR11Config::adcAdjustFreq(double *adcFreq, double *AdcChFreq) const {
        LTR11_FindAdcFreqParams(*adcFreq, NULL, NULL, adcFreq);
        *AdcChFreq = *adcFreq/adcEnabledChCnt();
    }
}
