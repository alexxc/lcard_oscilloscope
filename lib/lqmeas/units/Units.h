#ifndef STDUNITS
#define STDUNITS

#include "Unit.h"
#include <QObject>

namespace LQMeas {
    class Units : public QObject {
        Q_OBJECT
    public:
        static const Unit *V();  /* вольты */
        static const Unit *mV(); /* миливольты */
        static const Unit *uV(); /* микровольты */

        static const Unit *A();  /* амперы */
        static const Unit *mA(); /* милиамперы */
        static const Unit *uA(); /* микроамперы */

        static const Unit *m();  /* метры */
        static const Unit *mm(); /* милиметры */
        static const Unit *um(); /* микрометры */


        static const Unit *Hz();  /* герцы */
        static const Unit *KHz(); /* килогерцы */
        static const Unit *MHz(); /* мгагерц */
        static const Unit *rpm(); /* обороты в минуту */

        static const Unit *ohm(); /* Омы */

        static const Unit *degreeC(); /* градусы Цельсия */

        static const Unit *degree(); /* градусы (угол) */

        static const Unit *sec();  /* секунды */
        static const Unit *msec(); /* милисекунды */
        static const Unit *usec(); /* микросекунды */

        static const Unit *m_s();   /* м/c */
        static const Unit *mm_s();  /* мм/с */

        static const Unit *m_s2(); /* м/с^2 */
        static const Unit *g();    /* g */

        static const Unit *bit(); /* бит */
        static const Unit *Kbit(); /* килобит */
        static const Unit *Mbit(); /* мегабит */

        static const Unit *byte(); /* байт */
        static const Unit *Kbyte(); /* Кбайт */
        static const Unit *Mbyte(); /* Мбайт */


        static const Unit *percentage(); /* проценты */

    };
}

#endif // STDUNITS

