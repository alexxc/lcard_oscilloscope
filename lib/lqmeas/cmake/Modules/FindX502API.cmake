# - Find x502api library
# This module defines
#  X502API_INCLUDE_DIRS, where to find x502api headers
#  X502API_LIBRARIES, the libraries needed to x502api
#  X502API_FOUND, If false, do not try to use x502api.
#
# Components: l502api e502api (all enabled if not specified)
#
# On Windows you can specify LPCIE_ROOT_DIR for root dir of lpcie (if not default)

if (NOT X502API_FOUND)
    if(WIN32)
        cmake_policy(VERSION 3.2)
    endif(WIN32)

    set(X502API_SUPPORTED_COMPONENTS x502api l502api e502api)

    #по-умолчанию ищем все компоненты
    if(NOT X502API_FIND_COMPONENTS)
        set(X502API_FIND_COMPONENTS ${X502API_SUPPORTED_COMPONENTS})
    else(NOT X502API_FIND_COMPONENTS)
        set(X502API_FIND_COMPONENTS x502api ${X502API_FIND_COMPONENTS})
    endif(NOT X502API_FIND_COMPONENTS)

    if(WIN32)
        set(X502API_SEARCH_DIRS ENV PROGRAMFILES ENV PROGRAMFILES(x86) ${LPCIE_ROOT_DIR})
        set(X502API_SEARCH_INCLUDE_DIRS ${X502API_SEARCH_DIRS})
        set(X502API_SEARCH_LIBRARY_DIRS ${X502API_SEARCH_DIRS})
        set(X502API_SEARCH_INCLUDE_SUFFIX L-Card/lpcie/include include)
        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            if(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(X502API_SEARCH_LIBRARY_DEF_SUFFIX "msvc")
            else(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(X502API_SEARCH_LIBRARY_DEF_SUFFIX "msvc64")
            endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        else("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            if(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(X502API_SEARCH_LIBRARY_DEF_SUFFIX "mingw")
            else(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(X502API_SEARCH_LIBRARY_DEF_SUFFIX "mingw64")
            endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

        set(X502API_SEARCH_LIBRARY_SUFFIX "lib/${X502API_SEARCH_LIBRARY_DEF_SUFFIX}"
                                          "L-Card/lpcie/lib/${X502API_SEARCH_LIBRARY_DEF_SUFFIX}")
    else(WIN32)
        find_package(PkgConfig QUIET)
        pkg_check_modules(X502API_PKG QUIET libx502api1)

        set(X502API_SEARCH_INCLUDE_DIRS  ${X502API_PKG_INCLUDE_DIRS} include local/include)

        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(X502API_SEARCH_LIBRARY_DIRS lib local/lib lib/i386-linux-gnu)
        else(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(X502API_SEARCH_LIBRARY_DIRS lib64 lib/x86_64-linux-gnu local/lib64)
        endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    endif(WIN32)


    foreach(X502API_COMPONENT ${X502API_FIND_COMPONENTS})
        list(FIND X502API_SUPPORTED_COMPONENTS  ${X502API_COMPONENT} X502API_COMPONENT_IDX)
        if(${X502API_COMPONENT_IDX}_IDX EQUAL -1)
            message(WARNING "x502api: unsupported component ${X502API_COMPONENT}")
        else()

            find_path(X502API_${X502API_COMPONENT}_INCLUDE_DIR NAMES ${X502API_COMPONENT}.h
               PATHS
               ${X502API_SEARCH_INCLUDE_DIRS}
               PATH_SUFFIXES ${X502API_SEARCH_INCLUDE_SUFFIX}
            )


            if (WIN32)
                find_library(X502API_${X502API_COMPONENT}_LIBRARY NAMES ${X502API_COMPONENT}
                    PATHS
                    ${X502API_SEARCH_LIBRARY_DIRS}
                    PATH_SUFFIXES ${X502API_SEARCH_LIBRARY_SUFFIX}
                    NO_DEFAULT_PATH
                )
            else(WIN32)
                find_library(X502API_${X502API_COMPONENT}_LIBRARY NAMES ${X502API_COMPONENT}
                    PATHS
                    ${X502API_SEARCH_LIBRARY_DIRS}
                    PATH_SUFFIXES ${X502API_SEARCH_LIBRARY_SUFFIX}
                )
            endif(WIN32)

            if (X502API_${X502API_COMPONENT}_INCLUDE_DIR AND X502API_${X502API_COMPONENT}_LIBRARY)
                set(X502API_${X502API_COMPONENT}_FOUND TRUE)
                set(X502API_COMPONENT_LIBRARIES ${X502API_COMPONENT_LIBRARIES} ${X502API_${X502API_COMPONENT}_LIBRARY})
                set(X502API_COMPONENT_INCLUDE_DIRS ${X502API_COMPONENT_INCLUDE_DIRS} ${X502API_${X502API_COMPONENT}_INCLUDE_DIR})
            endif()
        endif()
    endforeach()

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(X502API REQUIRED_VARS
                                        X502API_COMPONENT_LIBRARIES
                                        X502API_COMPONENT_INCLUDE_DIRS
                                        HANDLE_COMPONENTS)

    if(X502API_FOUND)
        set(X502API_LIBRARIES ${X502API_LIBRARY} ${X502API_COMPONENT_LIBRARIES})
        set(X502API_INCLUDE_DIRS ${X502API_INCLUDE_DIR} ${X502API_COMPONENT_INCLUDE_DIRS})
    endif(X502API_FOUND)
endif (NOT X502API_FOUND)

