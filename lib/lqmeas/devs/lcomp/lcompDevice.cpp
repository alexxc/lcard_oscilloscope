#include "lcompDevice.h"
#include "lcomp_headers.h"
#include "lqmeas/lqtdefs.h"

namespace LQMeas {
    QString lcompDevice::firmwareDir() {
        return "firmware";
    }

    lcompDevice::~lcompDevice() {
        m_daqIface->Release();
    }

    Error lcompDevice::protOpen() {
        HANDLE hVxd = m_daqIface->OpenLDevice(); // открываем устройство
        Error err = Error::Success();

        if(hVxd == INVALID_HANDLE_VALUE) {
            err = lcompErrors::DeviceOpen();
        } else {
            QString fw_name = firmwareName();
            if (!fw_name.isEmpty()) {
                fw_name = firmwareDir() + "/" + fw_name;
                if (m_daqIface->LoadBios(QSTRING_TO_CSTR(fw_name).data())!=L_SUCCESS) {
                    err = lcompErrors::LoadBios();
                }
            }
        }

        if (err.isSuccess() && (m_daqIface->PlataTest()!=L_SUCCESS)) {
            err = lcompErrors::BoardTest();
        }

        if (err.isSuccess()) {
            PLATA_DESCR_U2 pd;
            if (m_daqIface->ReadPlataDescr(&pd)!=L_SUCCESS) {
                err = lcompErrors::ReadBoardDescr();
            } else {
                err = lcompFillDescr(&pd);
            }
        }

        if (!err.isSuccess()) {
            protClose();
        } else {
            m_is_opened = true;
        }

        return err;
    }

    Error lcompDevice::protClose() {
        Error err = Error::Success();
        if (m_daqIface->CloseLDevice()==L_SUCCESS) {
            m_is_opened = false;
        } else {
            err = lcompErrors::DeviceClose();
        }
        return err;
    }

    Error lcompDevice::lcompDeviceStart() {
        Error err = Error::Success();
        if (m_daqIface->InitStartLDevice()!=L_SUCCESS) {
            err = lcompErrors::AdcStartInit();
        } else if (m_daqIface->StartLDevice()!=L_SUCCESS) {
            err = lcompErrors::AdcStart();
        }
        return err;
    }

    lcompDevice::lcompDevice(IDaqLDevice *dev, const __SLOT_PARAM *par, unsigned slot,
                             DeviceConfig *defaultCfg, const DeviceInfo *info)
        : Device(defaultCfg, info), m_slot(slot), m_board_type(par->BoardType), m_daqIface(dev), m_is_opened(false) {

    }
}
