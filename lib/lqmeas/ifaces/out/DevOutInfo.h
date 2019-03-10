#ifndef LQMEAS_DEVOUTINFO_H
#define LQMEAS_DEVOUTINFO_H

#include "lqmeas/units/Units.h"

namespace LQMeas {
    /* Данный класс содержит информацию о возможностях модуля вывода */


    class DevOutInfo {
    public:
        /* поддерживается ли синхронный вывод на ЦАП */
        virtual bool outDacSyncSupport() const = 0;
        /* поддерживается ли синхронный вывод на цифровые линии */
        virtual bool outDigSyncSupport() const = 0;

        /* поддерживается ли асинхронный вывод на ЦАП */
        virtual bool outDacAsyncSupport() const = 0;
        /* поддерживается ли асинхронный вывод на цифровые линии */
        virtual bool outDigAsyncSupport() const = 0;
        /* признак, можно ли настраивать синхронный/асинхронный вывод на ЦАП
           индивидуально по каналам (true) или это общая настройка не весь
           вывод (false) */
        virtual bool outDacSyncModeCfgPerCh() const = 0;

        /* можно ли настраивать режим генерации сигнала из RAM (потоковый/циклический) */
        virtual bool outSyncRamModeIsConfigurable() const = 0;
        /* поддерживается ли настройка режима генерации на канал (RAM/HARD) */
        virtual bool outSyncChGenModeIsConfigurable() const = 0;

        /* Максимальное значение частоты в Гц для данного модуля */
        virtual double outSyncGenFreqMax() const = 0;
        /* поддерживается ли установка предварительных данных */
        virtual bool outSyncPresetSupport() const = 0;

        /* Количество каналов ЦАП в данном экземпляре модуля */
        virtual int outDacChannelsCnt() const = 0;
        /* Количество цифровых линий в данном экземпляре модуля */
        virtual int outDigChannelsCnt() const = 0;

        /* Единицы измерения величин, выдаваемых на данном канале ЦАП */
        virtual const Unit *outDacUnit(int ch) const {return Units::V();}
        /* Максимальное пиковое значение для данного канала ЦАП */
        virtual double outDacMaxVal(int ch) const = 0;
        /* Минимальное пиковое значение для данного канала ЦАП */
        virtual double outDacMinVal(int ch) const = 0;
    };
}

#endif // LQMEAS_DEVOUTINFO_H
