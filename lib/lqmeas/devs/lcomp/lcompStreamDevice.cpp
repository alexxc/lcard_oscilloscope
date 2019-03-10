#include "lcompStreamDevice.h"
#include "lcomp_headers.h"
#include "lqmeas/ifaces/in/DevAdcConfig.h"
#include "lqmeas/devs/DeviceConfig.h"

namespace LQMeas {
    Error lcompStreamDevice::adcGetData(double *data, unsigned size, unsigned flags, unsigned tout, unsigned *recvd_size) {
        return m_adcReceiver.getData(this, data, size, flags, tout, recvd_size);
    }

    Error lcompStreamDevice::lcompProcessRecvdData(const void *src_buf, unsigned size, double *dst_buf, unsigned flags, unsigned offs) {
        const qint16 *rcv_wrds = (const qint16 *)src_buf;
        unsigned ch_cnt = adcConfig()->adcEnabledChCnt();
        int ch_idx = m_cur_ch_idx;

        for (unsigned i=0; i < size; i++) {

            dst_buf[i] = ((double)rcv_wrds[i] + m_ch_conv_b[ch_idx]) * m_ch_conv_k[ch_idx];
            if (++ch_idx == ch_cnt)
                ch_idx = 0;
        }
        m_cur_ch_idx = ch_idx;
        return Error::Success();
    }

    lcompStreamDevice::lcompStreamDevice(IDaqLDevice *daqIface, const __SLOT_PARAM *par, unsigned slot,
                                         unsigned adc_channels, DeviceConfig *defaultConfig, const DeviceInfo *info) :
            lcompDevice(daqIface, par, slot, defaultConfig, info), DevAdc(this, adc_channels) {
        m_adcReceiver.moveToThread(&m_adcReceiver);
        Q_ASSERT((adc_channels <= lcomp_max_adc_channels));
    }

    Error lcompStreamDevice::protAdcStart() {
        Error err = Error::Success();
        ADC_PAR adcPar;
        m_cur_ch_idx = 0;

        err = lcompFillAdcParams(&adcPar);
        if (err.isSuccess()) {
            if (lcompIDaqIface()->FillDAQparameters(&adcPar.t1)!=L_SUCCESS) {
                err =  lcompErrors::AdcFillParams();
            }
        }


        if (err.isSuccess()) {
            lcompIDaqIface()->EnableCorrection(1);
            err = m_adcReceiver.setStreamParams(lcompIDaqIface(),
                                                &adcPar.t1, adcPar.t1.IrqStep * adcPar.t1.Pages,
                                                adcPar.t1.NCh, L_STREAM_ADC);
        }

        if (err.isSuccess()) {
            err = lcompDeviceStart();
        }

        if (err.isSuccess()) {
            m_adcReceiver.prestartInit();
            m_adcReceiver.start();
        }
        return err;
    }

    Error lcompStreamDevice::protAdcStop() {
        Error err = Error::Success();

        m_adcReceiver.stop();
        m_adcReceiver.wait();

        if (lcompIDaqIface()->StopLDevice()!=L_SUCCESS) {
            err = lcompErrors::AdcStop();
        }

        return err;
    }

    Error lcompStreamDevice::protConfigure(const DeviceConfig *cfg) {
        const DevAdcConfig *adcCfg = cfg->adcConfig();
        if (adcCfg) {
            /* после конфигурации рассчитываем коэффициенты перевода из кодов в
             * вольты по каждому каналу */
            for (unsigned ch_idx = 0; ch_idx < adcCfg->adcEnabledChCnt(); ch_idx++) {
                int ch_num = adcCfg->adcChNum(ch_idx);
                double drange = adcCfg->adcChRangeMaxVal(ch_num) - adcCfg->adcChRangeMinVal(ch_num);
                double dcode = adcCodeMax() - adcCodeMin();
                int range_num = adcCfg->adcChRangeNum(ch_num);
                double k = adcCbrK(ch_num, range_num);
                double b = adcCbrB(ch_num, range_num);

                m_ch_conv_b[ch_idx] = b;
                m_ch_conv_k[ch_idx] = k * drange / dcode;
            }
        }
        return Error::Success();
    }
}
