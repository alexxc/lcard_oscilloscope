#ifndef LQMEAS_X502CONFIG_H
#define LQMEAS_X502CONFIG_H

#include "lqmeas/devs/DeviceConfig.h"
#include "lqmeas/ifaces/in/DevAdcStdSeq32Config.h"
#include "lqmeas/ifaces/out/DevOutSyncConfig.h"
#include "x502api.h"

namespace LQMeas {
    class x502Config : public DeviceConfig, public DevAdcStdSeq32Config, public DevOutSyncConfig {
    public:
        static QString typeConfigName() {return "x502";}

        const DevAdcConfig *adcConfig() const {
            return static_cast<const DevAdcConfig*>(this);
        }
        const DevOutSyncConfig *outSyncConfig() const {
            return static_cast<const DevOutSyncConfig*>(this);
        }


        QString configDevTypeName() const {return typeConfigName();}
		x502Config *clone() const {
            return new x502Config(this);
        }

        x502Config();
        x502Config(const x502Config *cfg);

        bool adcExternalStart() const;

		void adcSetChAvg(unsigned int ch, unsigned int avg_cnt);
		void adcSetChSoftAvg(unsigned int ch, bool avg);
		unsigned int adcChAvg(unsigned int ch) const {return params.Adc.Ch[ch].Avg;}
		bool adcChSoftAvg(unsigned int ch) const {return params.Adc.Ch[ch].softAvg;}

        t_x502_sync_mode freqMode() const {return params.Sync.FreqMode;}
        t_x502_sync_mode startMode() const {return params.Sync.StartMode;}
        t_x502_ref_freq  refFreqInternal() const {return params.Sync.RefFreqInternal;}
        double           refFreqExternal() const {return params.Sync.RefFreqExternal;}

        void setFreqMode(t_x502_sync_mode mode) {params.Sync.FreqMode = mode;}
        void setStartMode(t_x502_sync_mode mode) {params.Sync.StartMode = mode;}
        void setRefFreqInternal(t_x502_ref_freq val) {params.Sync.RefFreqInternal = val;}
        void setRefFreqExternal(double val);

        /* текущее значение опорной частоты (на основе настроек
         * внутренней или внешней опорной частоты в зависимости от freqMode) */
        double refFreq() const;


        void setOutSyncGenFreq(double val) { params.Out.Freq = val;}
        double outSyncGenFreq() const {return results.OutFreq;}


        uint32_t pullups() const {return params.Pullups;}
        void setPullups(uint32_t pullups_flags) {params.Pullups = pullups_flags;}

        bool bfIsEnabled() const {return params.Blackfin.Enabled;}
        void setBfEnalbed(bool en) { params.Blackfin.Enabled = en;}

        QString bfFirmwareFilename() const { return params.Blackfin.FileName;}
        void setBfFirmwareFilename(QString filename) { params.Blackfin.FileName = filename;}

    protected:
        Error protLoadConfig(QSettings &set);
        Error protSaveConfig(QSettings &set) const;
        Error protUpdateConfig();
        void adcAdjustFreq(double *adcFreq, double *AdcChFreq) const;
    private:
		char padding[7];
		struct params {
			struct Adc {
                struct {
					unsigned Avg;
					bool softAvg;
					char padding[3];
				} Ch[adc_channels_cnt];
            } Adc;

			struct Sync {
				double           RefFreqExternal;
				t_x502_sync_mode FreqMode;
                t_x502_sync_mode StartMode;
                t_x502_ref_freq  RefFreqInternal;
				char padding[4];
			} Sync;

			struct Out {
				double Freq;
			} Out;

			struct Blackfin {
				QString FileName;
				bool Enabled;
				char padding[7];
			} Blackfin;

			uint32_t Pullups;
			char padding[4];
		} params;

		struct results {
            double DinFreq;
            double OutFreq;
        } results;
    };
}

#endif // LQMEAS_X502CONFIG_H
