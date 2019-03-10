#include "SvcInstanceNameValidator.h"

QValidator::State SvcInstanceNameValidator::validate(QString &input, int &pos) const {
    /* максимальная длина имени в DNS - 63. Также в текущей реализации
       не поддерживается использование точки */
    return (input.toUtf8().size() <= 63) && !(input.contains('.')) ? Acceptable : Invalid;
}
