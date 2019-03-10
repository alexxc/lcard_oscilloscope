#ifndef MEASUNIT_H
#define MEASUNIT_H

#include <QString>
#include "lqmeas/EnumNamedValue.h"

namespace LQMeas {
    /* Класс для обозначания едениц измерения.
       Сопоставлет некий код и переводимое название еденицы измерения отображения.
       Также содержит коэффициент перевода относительно базовых единиц измерения,
       чтобы можно было перводить значения в разных единицах измерения, но заданных
       относительно одной базовой */
    class Unit : public EnumNamedValue<int> {
    public:
        /* Указывает соотношение между данной величиной и базовой.
           Чтобы перевести из данных единиц в базовые, нужно значение
           умножить на multipler() */
        virtual double multipler() const = 0;
        /* базовая единица измерения, относительно которой задан multipler() */
        virtual const Unit *baseUnit() const = 0;
    };
}

#endif // MEASUNIT_H

