#include "DevAdc.h"
#include "DevAdcConfig.h"
#include "lqmeas/devs/Device.h"
#include "lqmeas/devs/DeviceConfig.h"
#include "lqmeas/log/Log.h"

namespace LQMeas {
    DevAdc::DevAdc(Device *dev, unsigned max_channels) : m_run(false), m_dev(dev) {
        m_ch_status.resize(max_channels);
    }

    Error DevAdc::adcStart() {
        Error err;

		for (int ch=0; ch < m_ch_status.size(); ++ch) {
            m_ch_status[ch] = adcConfig()->adcChEnabled(ch) ?
                        DevAdc::ChStatusNoData : DevAdc::ChStatusDisabled;
        }
        err = adcEnable();
        if (err.isSuccess())
            err = protAdcStart();

        if (err.isSuccess()) {
            m_run = true;
            LQMeasLog->detail(Device::tr("Data acquisition started successfully"), m_dev);
        } else {
            LQMeasLog->error(Device::tr("Data acquisition start error"), err, m_dev);
        }
        return err;
    }

    Error DevAdc::adcStop() {
        Error err, stop_err;
        err = adcDisable();
        stop_err = protAdcStop();
        if (err.isSuccess())
            err = stop_err;

        if (err.isSuccess()) {
            m_run = false;
            LQMeasLog->detail(Device::tr("Data acquisition stoped successfully"), m_dev);
        } else {
            LQMeasLog->error(Device::tr("Data acquisition stop error"), err, m_dev);
        }
        return err;
    }

    DevAdc::ChannelStatus DevAdc::adcGetChStatus(int ch, bool clear) {
        DevAdc::ChannelStatus ret;
        if (ch >= m_ch_status.size()) {
            ret = DevAdc::ChStatusError;
        } else {
			ret = m_ch_status.at(ch);
            if (clear && (ret!=DevAdc::ChStatusDisabled)) {
                m_ch_status[ch] = DevAdc::ChStatusNoData;
            }
        }
        return ret;
    }

    int DevAdc::adcChNum(int idx) const {
        return adcConfig()->adcChNum(idx);
    }

    int DevAdc::adcChIdx(int ch) const {
        return adcConfig()->adcChIdx(ch);
    }

    void DevAdc::adcSetChStatus(int ch, DevAdc::ChannelStatus status) {
        if (ch < m_ch_status.size()) {
            /* состояние ошибочной ситуации по каналу должно сохраняться до сброса статуса */
			if ((m_ch_status.at(ch)==ChStatusNoData) || (status != ChStatusOk))
				m_ch_status[ch] = status;
        }
    }

    const DevAdcConfig *DevAdc::adcConfig() const {
        return m_dev->currentConfig()->adcConfig();
    }

    Error DevAdc::adcEnable() {
        return Error::Success();
    }

    Error DevAdc::adcDisable() {
        return Error::Success();
    }
}
