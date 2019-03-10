#ifndef LQTDEFS_H
#define LQTDEFS_H

#ifdef Q_OS_WIN
    #define QSTRING_FROM_CSTR(str) QString::fromLocal8Bit(str)
    #define QSTRING_TO_CSTR(str)   str.toLocal8Bit()
#else
    #define QSTRING_FROM_CSTR(str) QString(str)
    #define QSTRING_TO_CSTR(str)   str.toUtf8()
#endif


#endif // LQTDEFS_H
