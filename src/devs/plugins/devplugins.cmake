MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

set(ROOT_DEVICES_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/src/devs)
set(ROOT_DEVPLUGIN_DIR ${ROOT_DEVICES_DIR}/plugins)


set(QT_HEADERS ${QT_HEADERS} ${ROOT_DEVPLUGIN_DIR}/LQDevPlugin.h)
set(SOURCES    ${SOURCES}    ${ROOT_DEVPLUGIN_DIR}/LQDevPlugin.cpp)

SUBDIRLIST(SUBDIRS ${ROOT_DEVPLUGIN_DIR})
FOREACH(subdir ${SUBDIRS})
    string(TOUPPER ${subdir} PLUGIN_UPPER)
    if(LQMEASSTUDIO_USE_DEV_${PLUGIN_UPPER})
        set(DEVPLUGIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/devs/plugins/${subdir})

        include(${DEVPLUGIN_DIR}/CMakeLists.txt)

        set(SOURCES ${SOURCES} ${DEVPLUGIN_SOURCES})
        set(HEADERS ${HEADERS} ${DEVPLUGIN_HEADERS})
        set(QT_HEADERS ${QT_HEADERS} ${DEVPLUGIN_QT_HEADERS})
        set(QT_UI_FILES ${QT_UI_FILES} ${DEVPLIGIN_UI})
    endif()
ENDFOREACH()
