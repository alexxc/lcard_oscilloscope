# - Find LTRAPI library
# This module defines
#  LTRAPI_INCLUDE_DIRS, where to find LTRAPI headers
#  LTRAPI_LIBRARIES, the libraries needed to LTRAPI
#  LTRAPI_FOUND, If false, do not try to use LTRAPI.
#
# Components: l502api e502api (all enabled if not specified)
#
# On Windows you can specify LPCIE_ROOT_DIR for root dir of lpcie (if not default)

if (NOT LTRAPI_FOUND)
    if(WIN32)
        cmake_policy(VERSION 3.2)
    endif(WIN32)

    #по-умолчанию ищем все компоненты
    if(NOT LTRAPI_FIND_COMPONENTS)
        set(LTRAPI_FIND_COMPONENTS ltrapi)
    endif(NOT LTRAPI_FIND_COMPONENTS)

    if(WIN32)
        set(LTRAPI_SEARCH_DIRS ENV PROGRAMFILES ENV PROGRAMFILES(x86) ${LTRAPI_ROOT_DIR})
        set(LTRAPI_SEARCH_INCLUDE_DIRS ${LTRAPI_SEARCH_DIRS})
        set(LTRAPI_SEARCH_LIBRARY_DIRS ${LTRAPI_SEARCH_DIRS})
        set(LTRAPI_SEARCH_INCLUDE_SUFFIX L-Card)
        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            if(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(LTRAPI_SEARCH_LIBRARY_DEF_SUFFIX "msvc")
            else(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(LTRAPI_SEARCH_LIBRARY_DEF_SUFFIX "msvc64")
            endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        else("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            if(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(LTRAPI_SEARCH_LIBRARY_DEF_SUFFIX "mingw")
            else(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(LTRAPI_SEARCH_LIBRARY_DEF_SUFFIX "mingw64")
            endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

        set(LTRAPI_SEARCH_LIBRARY_SUFFIX "lib/${LTRAPI_SEARCH_LIBRARY_DEF_SUFFIX}"
                                         "ltr/lib/${LTRAPI_SEARCH_LIBRARY_DEF_SUFFIX}"
                                         "L-Card/ltr/lib/${LTRAPI_SEARCH_LIBRARY_DEF_SUFFIX}")
    else(WIN32)
        find_package(PkgConfig QUIET)
        pkg_check_modules(LTRAPI_PKG QUIET libltrapi1)

        set(LTRAPI_SEARCH_INCLUDE_DIRS  ${LTRAPI_PKG_INCLUDE_DIRS} include local/include)

        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(LTRAPI_SEARCH_LIBRARY_DIRS lib local/lib lib/i386-linux-gnu)
        else(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(LTRAPI_SEARCH_LIBRARY_DIRS lib64 lib/x86_64-linux-gnu local/lib64)
        endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    endif(WIN32)


    foreach(LTRAPI_COMPONENT ${LTRAPI_FIND_COMPONENTS})
        find_path(LTRAPI_${LTRAPI_COMPONENT}_INCLUDE_DIR NAMES ltr/include/${LTRAPI_COMPONENT}.h
           PATHS
           ${LTRAPI_SEARCH_INCLUDE_DIRS}
           PATH_SUFFIXES ${LTRAPI_SEARCH_INCLUDE_SUFFIX}
        )
        if (WIN32)
            find_library(LTRAPI_${LTRAPI_COMPONENT}_LIBRARY NAMES ${LTRAPI_COMPONENT}
               PATHS
               ${LTRAPI_SEARCH_LIBRARY_DIRS}
               PATH_SUFFIXES ${LTRAPI_SEARCH_LIBRARY_SUFFIX}
               NO_DEFAULT_PATH
            )
        else(WIN32)
            find_library(LTRAPI_${LTRAPI_COMPONENT}_LIBRARY NAMES ${LTRAPI_COMPONENT}
               PATHS
               ${LTRAPI_SEARCH_LIBRARY_DIRS}
               PATH_SUFFIXES ${LTRAPI_SEARCH_LIBRARY_SUFFIX}
            )
        endif(WIN32)

        if (LTRAPI_${LTRAPI_COMPONENT}_INCLUDE_DIR AND LTRAPI_${LTRAPI_COMPONENT}_LIBRARY)
            set(LTRAPI_${LTRAPI_COMPONENT}_FOUND TRUE)
            set(LTRAPI_COMPONENT_LIBRARIES ${LTRAPI_COMPONENT_LIBRARIES} ${LTRAPI_${LTRAPI_COMPONENT}_LIBRARY})
            set(LTRAPI_COMPONENT_INCLUDE_DIRS ${LTRAPI_COMPONENT_INCLUDE_DIRS} ${LTRAPI_${LTRAPI_COMPONENT}_INCLUDE_DIR})
        endif()
    endforeach()

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(LTRAPI REQUIRED_VARS
                                        LTRAPI_COMPONENT_LIBRARIES
                                        LTRAPI_COMPONENT_INCLUDE_DIRS
                                        HANDLE_COMPONENTS)

    if(LTRAPI_FOUND)
        set(LTRAPI_LIBRARIES ${LTRAPI_LIBRARY} ${LTRAPI_COMPONENT_LIBRARIES})
        set(LTRAPI_INCLUDE_DIRS ${LTRAPI_INCLUDE_DIR} ${LTRAPI_COMPONENT_INCLUDE_DIRS})
        set(LTRAPI_DEFINITIONS -DLTRAPI_DISABLE_COMPAT_DEFS)
    endif(LTRAPI_FOUND)
endif (NOT LTRAPI_FOUND)

