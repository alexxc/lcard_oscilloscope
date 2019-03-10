#ifndef LQMEAS_DEVADC_H
#define LQMEAS_DEVADC_H

#include <QVector>
#include "lqmeas/StdErrors.h"
#include "lqmeas/devs/Device.h"
#include "lqmeas/units/Units.h"
#include "DevAdcInfo.h"

namespace LQMeas {
    class DevAdcConfig;

    class DevAdc {
    public:

        enum ChannelStatus {
            ChStatusOk,
            ChStatusNoData,
            ChStatusDisabled,
            ChStatusOpen,
            ChStatusShort,
            ChStatusOverload,
            ChStatusError
        };

        const DevAdcConfig *adcConfig() const;

        virtual bool adcIsRunning() const {return m_run;}

        virtual Error adcEnable();
        virtual Error adcDisable();

        Error adcStart();
        Error adcStop();
		virtual Error adcGetData(double *data, unsigned size, unsigned flags,
							   unsigned tout, unsigned *recvd_size) = 0;

        virtual ChannelStatus adcGetChStatus(int ch, bool clear = false);


        int adcChNum(int idx) const;
        int adcChIdx(int ch) const;
    protected:
        explicit DevAdc(Device *dev, unsigned max_channels);


        virtual Error protAdcStart() = 0;
        virtual Error protAdcStop() = 0;


        void adcSetChStatus(int ch, ChannelStatus status);
    private:
        void adcUpdateChanngelCfg();

        QVector<ChannelStatus> m_ch_status;
        bool m_run;
        Device *m_dev;

        friend class Device;
    };
}
#endif // LQMEAS_DEVADC_H
