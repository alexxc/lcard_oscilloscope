#include "Agilent3458.h"
#include "lqmeas/lqtdefs.h"



Agilent3458::Agilent3458() : LQMeasDevice() {
    A3458_Init(&m_hnd);
}


LQMeasError Agilent3458::getValue(double *val) {


    return error(A3458_GetValue(&m_hnd, val));
}

LQMeasError Agilent3458::startSingle() {
    return error(A3458_TrigSingle(&m_hnd));
}

LQMeasError Agilent3458::error(int err) const {
    return err == 0 ? LQMeasError::Success() :
                      LQMeasError(err, QSTRING_FROM_CSTR(A3458_GetErrorString(err)),
                                  LQMeasError::ERRTYPE_DEVNATIVE_GPIB);
}

LQMeasError Agilent3458::protOpen() {
    LQMeasError err = error(A3458_Open(&m_hnd));
    if (err.isSuccess())
        err = error(A3458_Reset(&m_hnd));
    if (err.isSuccess())
        m_is_opened = true;
    return err;
}

LQMeasError Agilent3458::protClose() {
    LQMeasError err = error(A3458_Close(&m_hnd));
    if (err.isSuccess()) {
        m_is_opened = false;
    }
    return err;
}

LQMeasError Agilent3458::protConfigure() {
    return error(A3458_Configure(&m_hnd, A3458_CFLAGS_TIMEEVENTS | A3458_CFLAGS_MEAS |
                              A3458_CFLAGS_MEMOUT | A3458_CFLAGS_SERVICES));
}
