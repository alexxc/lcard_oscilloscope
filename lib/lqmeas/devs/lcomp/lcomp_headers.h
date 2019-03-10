#ifndef LCOMP_HEADERS_H
#define LCOMP_HEADERS_H



#ifdef Q_OS_WIN    
    #include <windows.h>
    #define LCOMP_LAST_ERROR  GetLastError()
#else
    #define LCOMP_LINUX 1
    #include "include/stubs.h"
    #include "errno.h"
    #define LCOMP_LAST_ERROR  errno
#endif
#include "include/ioctl.h"
#include "include/ifc_ldev.h"





#endif // LCOMP_HEADERS_H
