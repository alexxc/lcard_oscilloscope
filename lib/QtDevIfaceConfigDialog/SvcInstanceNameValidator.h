#ifndef SVCINSTANCENAMEVALIDATOR_H
#define SVCINSTANCENAMEVALIDATOR_H

#include <QValidator>

class SvcInstanceNameValidator : public QValidator {
    Q_OBJECT
public:
     State validate(QString &input, int &pos) const;
};

#endif // SVCINSTANCENAMEVALIDATOR_H
