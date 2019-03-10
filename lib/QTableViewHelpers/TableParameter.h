#ifndef TABLEPARAMETER_H
#define TABLEPARAMETER_H

#include <QVariant>
#include "TableViewSettings.h"

class QAbstractItemDelegate;

/* Базовый класс для представления логики работы с параметрами на основе
   Qt-ной концепции модуль-представление.  Кроме того, класс может предоставлять
   делегат для редактирования параметра и рекомендуемые параметры отображения
   (ширина столбца).

   Класс стандартную обработку флагов и фона отображения на основе
   состояний enabled(), readOnly(), notAssigned().

   Унаследованный класс должен при необходимости переопределить эти функции, а
   также реализовать protData() для отображения данных, а также для не readOnly()
   параметров функцию protSetData(). Вызов protData() и protSetData() выполняется
   только в случае, если элемент разрешен (не разрешенные отображаются одинаково) */
template <typename ParamItem> class TableParameter {
public:
    /* Имя параметра (которое будет отображаться в заголовке) */
    virtual QString name() const = 0;
    /* получение данных с обслуживанием некоторых стандартных запросов.
       Как правило не переопределяется, а паереопределяется protData(),
       которая вызывается только для разрешенного элемента, если данный
       запрос не обрабатывается стандартным образом */
    virtual QVariant data(ParamItem item, int role) const {
        QVariant ret;
        bool processed = false;

        if (!enabled(item)) {
            processed = true;
            if (role == Qt::BackgroundRole) {
                ret = TableViewSettings::instance()->disabledTableElementBrush();
            }
        } else if (readOnly(item) && (role == Qt::BackgroundRole) && !customReadonlyColor(item)) {
            processed = true;
            ret = TableViewSettings::instance()->readonlyTableElementBrush();
        } else if (notAssigned(item) && (role == Qt::BackgroundRole)) {
            processed = true;
            ret = TableViewSettings::instance()->notAssignedTableElementBrush();
        }

        if (!processed) {
            ret = protData(item, role);
        }
        return ret;
    }

    /* установка данных с проверкой readOnly() и enabled(). Как правило
       не переопределяется, а в унаследованном классе определяется
       setProtData() */
    virtual bool setData(ParamItem item, const QVariant &value, int role) const {
        return (enabled(item) && !readOnly(item)) ? protSetData(item, value, role) : false;
    }

    /* Флаги элемента. Определяются на основе других методов, а этот как правило
       не переопределяется */
    virtual Qt::ItemFlags flags(ParamItem item) const {
        Qt::ItemFlags ret = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        if (enabled(item) && !readOnly(item))
            ret |= Qt::ItemIsEditable;
        return ret;
    }

    virtual QVariant paramId() const {return 0;}

    virtual QString toolTip() const {return "";}

    /* Функция создания нестандартного делегата для редоктирования параметра */
    virtual QAbstractItemDelegate *createDelegate(QObject *parent) const {return 0;}
    /* Рекомендуемая ширина столбца для параметра (если 0 - по-умолчанию) */
    virtual unsigned columnWidth() const {return 0;}
    /* признак, что при изменении параметра необходимо обновить отображения
       всех параметров элемента */
    virtual bool itemUpdateRequired() const {return false;}
    /* признак, что при изменении нужно обновить только отображение изменненного
       параметра элемента */
    virtual bool itemParamUpdateReqired() const {return false;}
    /* признак, разрешен ли параметр для заданного элемента. если нет, то
       не ведется работа с данными, а просто ячейка отображается стандартным
       способом. По умолчанию все разрешены  */
    virtual bool enabled(ParamItem item) const {return true;}
    /* признак, что данный параметр доступен только для чтения.
       По умолчанию всегда false.
       Функция вызывается только для разрешенных элеметнов (т.е. при
       реализации можно считать, что enabled() выполняется).
       Если true, то данные отображаются, но их нельзя изменить и используется
       стандартный фон, если только customReadonlyColor() не возвращает true */
    virtual bool readOnly(ParamItem item) const {return false;}
    /* Признак, что элемент нужно выделить спец. образом, чтобы показать,
       что параметр не назначен */
    virtual bool notAssigned(ParamItem item) const {return false;}
    /* Признак, что для readOnly() элемента не нужно обрабатывать фон стандартным
       образом */
    virtual bool customReadonlyColor(ParamItem item) const {return false;}
protected:
    /* Получение данных, которые не обрабатываются стандартным образом.
       Вызывается только для разрешенного элемента.
       Должно всегда определяться в реализации */
    virtual QVariant protData(ParamItem item, int role) const = 0;
    /* Установка данных. Вызывается после проверки резрешения элемента и что
       он не в состоянии readOnly(). Должен быть переопределен для всех параметров,
       кроме тех, что для всех элементов возвращают в readOnly() true */
    virtual bool protSetData(ParamItem item, const QVariant &value, int role) const {return false;}
};

#endif // TABLEPARAMETER_H

