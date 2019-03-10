#ifndef LQMEAS_ERROR_H
#define LQMEAS_ERROR_H

#include <QString>
#include <QObject>

namespace LQMeas {
    class Error {
    public:
        static Error Success()            {return Error(0, QObject::tr("Ok"));}

        enum Type {
            ERRTYPE_LQMEAS          = 0,
            ERRTYPE_DEVNATIVE_LTR   = 1,
            ERRTYPE_DEVNATIVE_LCOMP = 2,
            ERRTYPE_DEVNATIVE_GPIB  = 3,
            ERRTYPE_DEVNATIVE_X502  = 4,
            ERRTYPE_USER            = 0x80
        } ;

        explicit Error(int err_code, QString msg, Type type = ERRTYPE_LQMEAS) :
            m_err_code(err_code), m_msg(msg), m_type(type) {}

        explicit Error() : m_err_code(0), m_type(ERRTYPE_LQMEAS) {}



        int errorCode() const {return m_err_code;}
        QString msg() const {return m_msg;}
        Type type() const {return m_type;}

        bool isSuccess() const {return isEqual(Success());}


        const bool operator==(const Error &rhs) const {return isEqual(rhs); }
        const bool operator!=(const Error &rhs) const {return !isEqual(rhs);}

        bool isEqual(const Error &rhs) const {return (m_err_code == rhs.m_err_code) && (m_type == rhs.m_type);}
    private:
        int m_err_code;
        QString m_msg;
        Type m_type;
    };
}


#endif // LQMEAS_ERROR_H
