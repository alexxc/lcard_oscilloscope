#ifndef LQMEAS_LCOMPERRORS_H
#define LQMEAS_LCOMPERRORS_H

#include "lqmeas/Error.h"

namespace LQMeas {
    class lcompErrors : public QObject {
        Q_OBJECT
    public:
        static Error CreateInstance()  {
            return lcompError(-1, lcompErrors::tr("Create instance error"));
        }
        static Error DeviceOpen()  {
            return lcompError(-2, lcompErrors::tr("Cannot open device"));
        }
        static Error LoadBios()  {
            return lcompError(-3, lcompErrors::tr("Cannot load device firmware"));
        }
        static Error BoardTest()  {
            return lcompError(-4, lcompErrors::tr("Board test error"));
        }
        static Error ReadBoardDescr()  {
            return lcompError(-5, lcompErrors::tr("Cannot read board description"));
        }
        static Error DeviceClose()  {
            return lcompError(-6, lcompErrors::tr("Cannot close device"));
        }
        static Error AdcFillParams()  {
            return lcompError(-7, lcompErrors::tr("Cannot fill ADC parameters"));
        }
        static Error AdcReqBuffer()  {
            return lcompError(-8, lcompErrors::tr("Request ADC buffer error"));
        }
        static Error AdcSetParams()  {
            return lcompError(-9, lcompErrors::tr("Cannot setup ADC parameters"));
        }
        static Error AdcStartInit()  {
            return lcompError(-10, lcompErrors::tr("Cannot initialize ADC start"));
        }
        static Error AdcStart()  {
            return lcompError(-11, lcompErrors::tr("Cannot start ADC"));
        }
        static Error AdcStop()  {
            return lcompError(-12, lcompErrors::tr("Cannot stop ADC"));
        }
        static Error RecvBufOverflow()  {
            return lcompError(-13, lcompErrors::tr("Receive buffer overflow"));
        }
    private:
        static Error lcompError(int err_code, QString msg)  {
            return Error(err_code, msg, Error::ERRTYPE_DEVNATIVE_LCOMP);
        }
    };
}

#endif // LQMEAS_LCOMPERRORS_H
