#ifndef ENUMVALUE_H
#define ENUMVALUE_H

#include <QString>
#include <QStringList>

namespace LQMeas {
    /* Сопоставлет некое значение кода из перечисления и строки, описывающей
       этот код. Для возможности перевода строки на лету, она создается
       не при создании объекта а при обращении к методу получения строки */
    template <typename EnumValueType> class EnumNamedValue {
    public:
        virtual EnumValueType code() const = 0;
        virtual QString name() const = 0;

        bool operator==(const EnumNamedValue& type) {
            return code() == type->value();
        }

        bool operator==(const EnumValueType& val) {
            return code() == val;
        }

        static QStringList namesList(QList<const EnumNamedValue *> enumVals) {
            QStringList ret;
            Q_FOREACH(const EnumNamedValue * val, enumVals) {
                ret.append(val->name());
            }
            return ret;
        }

        static const EnumNamedValue *enumValue(QList<const EnumNamedValue *> enumVals, const EnumValueType &val) {
            const EnumNamedValue *ret = 0;
            Q_FOREACH(const EnumNamedValue * enumVal, enumVals) {
                if (enumVal->code() == val) {
                    ret = enumVal;
                    break;
                }
            }
            return ret;
        }

        static const EnumNamedValue *enumValue(QList<const EnumNamedValue *> enumVals, QString name) {
            const EnumNamedValue *ret = 0;
            Q_FOREACH(const EnumNamedValue * enumVal, enumVals) {
                if (enumVal->name() == name) {
                    ret = enumVal;
                    break;
                }
            }
            return ret;
        }
    };
}

#endif // ENUMVALUE_H
