#include "x502Config.h"
#include "x502TypeInfo.h"
#include "x502.h"

namespace LQMeas {

	x502Config::x502Config() : DevAdcStdSeq32Config(x502TypeInfo::defaultInfo()->adc()),
							  DevOutSyncConfig(x502TypeInfo::defaultInfo()->out()) {
        memset(&params.Adc, 0, sizeof(params.Adc));
        memset(&params.Sync, 0, sizeof(params.Sync));
        memset(&params.Out, 0, sizeof(params.Out));
        params.Pullups = 0;
        params.Blackfin.Enabled = false;

		for (unsigned ch = 0; ch < adc_channels_cnt; ++ch) {
            params.Adc.Ch[ch].Avg = 1;
			params.Adc.Ch[ch].softAvg = false;
		}

        results.DinFreq = adcInfo()->adcFreqMax();
        params.Sync.FreqMode = params.Sync.StartMode = X502_SYNC_INTERNAL;
        params.Sync.RefFreqInternal = X502_REF_FREQ_2000KHZ;
        params.Out.Freq = results.OutFreq = adcInfo()->adcFreqMax()/2;

    }

	x502Config::x502Config(const x502Config *cfg) : DevAdcStdSeq32Config(cfg), DevOutSyncConfig(cfg) {
        memcpy(&params.Adc, &cfg->params.Adc, sizeof(params.Adc));
        memcpy(&params.Sync, &cfg->params.Sync, sizeof(params.Sync));
        memcpy(&params.Out, &cfg->params.Out, sizeof(params.Out));
        memcpy(&results, &cfg->results, sizeof(results));
        params.Pullups = cfg->params.Pullups;
        params.Blackfin.Enabled = cfg->params.Blackfin.Enabled;
        params.Blackfin.FileName = cfg->params.Blackfin.FileName;
    }

    bool x502Config::adcExternalStart() const {
        return (params.Sync.FreqMode != X502_SYNC_INTERNAL) ||
                (params.Sync.StartMode != X502_SYNC_INTERNAL);
    }

	void x502Config::adcSetChAvg(unsigned int ch, unsigned int avg_cnt) {
        if (avg_cnt > X502_LCH_AVG_SIZE_MAX)
            avg_cnt = X502_LCH_AVG_SIZE_MAX;
        params.Adc.Ch[ch].Avg = avg_cnt;
    }

	void x502Config::adcSetChSoftAvg(unsigned int ch, bool avg) {
		params.Adc.Ch[ch].softAvg = avg;
	}

	void x502Config::setRefFreqExternal(double val) {
        if (val > X502_EXT_REF_FREQ_MAX)
            val = X502_EXT_REF_FREQ_MAX;
         params.Sync.RefFreqExternal = val;
    }

    double x502Config::refFreq() const {
        return params.Sync.FreqMode == X502_SYNC_INTERNAL ?
               params.Sync.RefFreqInternal : params.Sync.RefFreqExternal;
    }

    Error x502Config::protLoadConfig(QSettings &set) {
        bool ok;
        double dval;
		int intval;
		unsigned uintval;

        set.beginGroup("Adc");
		unsigned int ch_cnt = static_cast<unsigned>(set.beginReadArray("Channels"));

		for (unsigned int ch=0; (ch < ch_cnt) && (ch < adc_channels_cnt); ++ch) {
			set.setArrayIndex(static_cast<signed>(ch));

			uintval = set.value("Avg", 1).toUInt(&ok);
			if (ok && (uintval <= X502_LCH_AVG_SIZE_MAX)) {
				params.Adc.Ch[ch].Avg = uintval;
            } else {
                reportLoadWarning(x502::tr("Invalid channel %1 average value").arg(ch+1));
            }

			params.Adc.Ch[ch].softAvg = set.value("SoftAvg", false).toBool();
		}

		for (unsigned int ch=ch_cnt; ch < adc_channels_cnt; ++ch) {
            params.Adc.Ch[ch].Avg = 1;
			params.Adc.Ch[ch].softAvg = false;
		}
        set.endArray();
        set.endGroup();

        intval = set.value("SyncFreqMode", X502_SYNC_INTERNAL).toInt(&ok);
        if (ok && (intval >= X502_SYNC_INTERNAL) && (intval <= X502_SYNC_DI_SYN2_FALL)) {
			params.Sync.FreqMode = static_cast<t_x502_sync_mode>(intval);
        } else {
            reportLoadWarning(x502::tr("Invalid sync frequency mode"));
        }

        intval = set.value("SyncStartMode", X502_SYNC_INTERNAL).toInt(&ok);
        if (ok && (intval >= X502_SYNC_INTERNAL) && (intval <= X502_SYNC_DI_SYN2_FALL)) {
			params.Sync.StartMode = static_cast<t_x502_sync_mode>(intval);
        } else {
            reportLoadWarning(x502::tr("Invalid sync start mode"));
        }

        intval = set.value("RefFreq", X502_REF_FREQ_2000KHZ).toInt(&ok);
        if (ok && ((intval == X502_REF_FREQ_1500KHZ) || (intval == X502_REF_FREQ_2000KHZ))) {
			params.Sync.RefFreqInternal = static_cast<t_x502_ref_freq>(intval);
        } else {
            reportLoadWarning(x502::tr("Invalid internal reference frequency value"));
        }

        dval =  set.value("ExtRefFreq", X502_EXT_REF_FREQ_MAX).toDouble(&ok);
        if (ok && (dval < 1.5e6)) {
            params.Sync.RefFreqExternal = dval;
        } else {
            reportLoadWarning(x502::tr("Invalid external reference frequency value"));
        }
		params.Pullups = static_cast<unsigned>(set.value("Pullups", 0).toInt());

        double max_out_freq = x502TypeInfo::defaultInfo()->outSyncGenFreqMax();
        dval = set.value("OutFreq", max_out_freq).toDouble(&ok);
        if (ok && (dval > 0) && (dval <= max_out_freq)) {
            params.Out.Freq = dval;
        } else {
            reportLoadWarning(x502::tr("Invalid output frequency value"));
        }

        params.Blackfin.Enabled = set.value("BlackfinEnable").toBool();
        params.Blackfin.FileName = set.value("BlackfinFirmFile").toString();

        protAdcLoadConfig(set);

        return outSyncLoadSignalConfig(set);
    }

    Error x502Config::protSaveConfig(QSettings &set) const {
        set.beginGroup("Adc");
        set.beginWriteArray("Channels", adc_channels_cnt);

		for (unsigned int ch=0; ch < adc_channels_cnt; ch++) {
			set.setArrayIndex(static_cast<signed>(ch));
            set.setValue("Avg", params.Adc.Ch[ch].Avg);
			set.setValue("SoftAvg", params.Adc.Ch[ch].softAvg);
		}
        set.endArray();
        set.endGroup();

        set.setValue("SyncFreqMode", params.Sync.FreqMode);
        set.setValue("SyncStartMode", params.Sync.StartMode);
        set.setValue("RefFreq", params.Sync.RefFreqInternal);
        set.setValue("ExtRefFreq", params.Sync.RefFreqExternal);
		set.setValue("Pullups", static_cast<int>(params.Pullups));

        set.setValue("OutFreq", params.Out.Freq);

        set.setValue("BlackfinEnable", params.Blackfin.Enabled);
        set.setValue("BlackfinFirmFile", params.Blackfin.FileName);

        protAdcSaveConfig(set);

        return outSyncSaveSignalConfig(set);
    }

    Error x502Config::protUpdateConfig() {
        double ref_freq = refFreq();

        results.DinFreq = ref_freq;
        results.OutFreq = params.Out.Freq;

		X502_CalcDinFreq(ref_freq, &results.DinFreq, nullptr);
		X502_CalcOutFreq(ref_freq, &results.OutFreq, nullptr);
        return protAdcUpdateConfig();
    }

    void x502Config::adcAdjustFreq(double *adcFreq, double *AdcChFreq) const {
		unsigned int max_avg = 1;
        double ref_freq = refFreq();
		for (unsigned int ch = 0; ch < adc_channels_cnt; ++ch) {
            if (adcChEnabled(ch)) {
				unsigned int avg = params.Adc.Ch[ch].Avg;
                if (avg > max_avg)
                    max_avg = avg;
            }
        }

        if (*adcFreq > ref_freq/max_avg)
            *adcFreq = ref_freq/max_avg;

		X502_CalcAdcFreq(ref_freq, adcEnabledChCnt(), adcFreq, AdcChFreq, nullptr, nullptr);
    }
}
