#include "E2010.h"

QString e2010::typeDeviceName() {
    return "E20-10";
}


e2010::e2010(__SLOT_PARAM *par, unsigned slot) : lcompDevice(par, slot) {

}
