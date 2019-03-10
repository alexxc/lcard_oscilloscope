# - Find x502api library
# This module defines
#  X502API_INCLUDE_DIRS, where to find x502api headers
#  X502API_LIBRARIES, the libraries needed to x502api
#  X502API_FOUND, If false, do not try to use x502api.
#
# Components: L502, E502 (all enabled if not specified)
#
# On Windows you can specify LPCIE_ROOT_DIR for root dir of lpcie (if not default)

if (NOT FFTW3_FOUND)
    if(WIN32)
        cmake_policy(VERSION 3.2)
    endif(WIN32)

    if(WIN32)
        set(FFTW3_SEARCH_DIRS ENV PROGRAMFILES ENV PROGRAMFILES(x86) ${FFTW3_ROOT_DIR})
        set(FFTW3_SEARCH_INCLUDE_DIRS ${FFTW3_SEARCH_DIRS})
        set(FFTW3_SEARCH_LIBRARY_DIRS ${FFTW3_SEARCH_DIRS})
        set(FFTW3_SEARCH_INCLUDE_SUFFIX include libfftw3/include fftw3/include)
        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            if(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(FFTW3_SEARCH_LIBRARY_DEF_SUFFIX "msvc")
            else(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(FFTW3_SEARCH_LIBRARY_DEF_SUFFIX "msvc64")
            endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        else("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            if(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(FFTW3_SEARCH_LIBRARY_DEF_SUFFIX "mingw")
            else(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(FFTW3_SEARCH_LIBRARY_DEF_SUFFIX "mingw64")
            endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(FFTW3_SEARCH_LIBRARY_SUFFIX   "lib/${FFTW3_SEARCH_LIBRARY_DEF_SUFFIX}"
                                          "libfftw3/lib/${FFTW3_SEARCH_LIBRARY_DEF_SUFFIX}"
                                          "fftw3/lib/${FFTW3_SEARCH_LIBRARY_DEF_SUFFIX}")
    else(WIN32)
        find_package(PkgConfig QUIET)
        pkg_check_modules(FFTW3_PKG QUIET libfftw3)

        set(FFTW3_SEARCH_INCLUDE_DIRS  ${FFTW3_PKG_INCLUDE_DIRS} include local/include)

        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(FFTW3_SEARCH_LIBRARY_DIRS lib local/lib lib/i386-linux-gnu)
        else(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(FFTW3_SEARCH_LIBRARY_DIRS lib64 lib/x86_64-linux-gnu local/lib64)
        endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    endif(WIN32)

    find_path(FFTW3_INCLUDE_DIR NAMES fftw3.h
       PATHS       
       ${FFTW3_SEARCH_INCLUDE_DIRS}
       PATH_SUFFIXES ${FFTW3_SEARCH_INCLUDE_SUFFIX}
    )

    find_library(FFTW3_LIBRARY NAMES fftw3
       PATHS
       ${FFTW3_SEARCH_LIBRARY_DIRS}
       PATH_SUFFIXES ${FFTW3_SEARCH_LIBRARY_SUFFIX}
    )

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(FFTW3
            REQUIRED_VARS FFTW3_INCLUDE_DIR FFTW3_LIBRARY
            )

    if(FFTW3_FOUND)
        set(FFTW3_LIBRARIES ${FFTW3_LIBRARY})
        set(FFTW3_INCLUDE_DIRS ${FFTW3_INCLUDE_DIR})
    endif(FFTW3_FOUND)
endif (NOT FFTW3_FOUND)

