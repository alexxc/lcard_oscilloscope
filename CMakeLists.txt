set(PROJECT lqmeasstudio)
set(PRJ_APP_TITLE "L-Card Measurement Studio")

set(PRJ_DESCR_EN "Measurement software")
set(PRJ_DESCR_RU "Измерительное ПО")

option(LQMEASSTUDIO_BUILD_WITH_QT5 "Build with Qt5 libraries rather Qt4" ON)
option(LQMEASSTUDIO_USE_DEV_X502 "include x502 support module" ON)
option(LQMEASSTUDIO_USE_DEV_LCOMP "include lcomp devices support" OFF)
option(LQMEASSTUDIO_USE_DEV_LTR   "include LTR devices support" OFF)
option(LQMEASSTUDIO_INCLUDE_LMATH "build lmath rather use system version" ON)





if(LQMEASSTUDIO_BUILD_WITH_QT5)
    cmake_minimum_required(VERSION 2.8.11)
else(LQMEASSTUDIO_BUILD_WITH_QT5)
    cmake_minimum_required(VERSION 2.6)
endif(LQMEASSTUDIO_BUILD_WITH_QT5)


project(${PROJECT})
set(PRJ_VER_MAJOR 1)
set(PRJ_VER_MINOR 2)
set(PRJ_VER_PATCH 2)
set(PRJ_VERSION   ${PRJ_VER_MAJOR}.${PRJ_VER_MINOR}.${PRJ_VER_PATCH})

add_definitions(-DPRJ_NAME="${PROJECT}")
add_definitions(-DPRJ_VERION_STR="${PRJ_VERSION}")
add_definitions(-DPRJ_APP_TITLE="${PRJ_APP_TITLE}")

if(WIN32)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS,5.01")
endif(WIN32)

SET(LANGUAGES ru)

if(NOT PRJ_TRANSLATIONS_DIR)
    if(WIN32)
        set(PRJ_TRANSLATIONS_DIR "translations")
    else(WIN32)
        set(PRJ_TRANSLATIONS_DIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/translations")
    endif(WIN32)
endif(NOT PRJ_TRANSLATIONS_DIR)

add_definitions(-DPRJ_TRANSLATIONS_DIR="${PRJ_TRANSLATIONS_DIR}")




include(src/devs/plugins/devplugins.cmake)


if(LQMEASSTUDIO_INCLUDE_LMATH)
    set(LMATH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/lmath)
    set(LMATH_LIBRARIES_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib/lmath)
    add_subdirectory(lib/lmath)
endif(LQMEASSTUDIO_INCLUDE_LMATH)

link_directories(${FFTW_LIBRARIES_DIRS})


if(LQMEASSTUDIO_DEBUG_X502API_SRC)
	add_subdirectory(${LQMEASSTUDIO_DEBUG_X502API_SRC} ${CMAKE_CURRENT_BINARY_DIR}/x502api)
    include_directories(${LQMEASSTUDIO_DEBUG_X502API_SRC}/src
                        ${LQMEASSTUDIO_DEBUG_X502API_SRC}/devs/l502
						${LQMEASSTUDIO_DEBUG_X502API_SRC}/devs/e502)
endif(LQMEASSTUDIO_DEBUG_X502API_SRC)



set(SOURCES ${SOURCES}
    src/main.cpp
    src/MainWindow.cpp
    src/LQMeasStudio.cpp

    src/FreqConfigWidget.cpp
    src/WaitDialog.cpp
    src/SettingsDialog.cpp

    src/QHexSpinBox.cpp

    src/ProcessPlugin.cpp

    src/devs/DeviceTree.cpp
    src/devs/IpAddrConfig/IpAddrConfig.cpp
    src/devs/IpAddrConfig/IpAddrConfigDialog.cpp
    src/devs/IpAddrConfig/IpAddrAddDialog.cpp
    src/devs/NetworkDevicesBrowser/NetworkBrowserModel.cpp
    src/devs/NetworkDevicesBrowser/NetworkBrowserDialog.cpp

    src/out/OutSigGenaration.cpp
    src/out/OutSignalConfigCtl.cpp
    src/out/DacSignalDialog.cpp
    src/out/DacSignalParams/DacSignalParamUiSin.cpp
    src/out/DacSignalParams/DacSignalParamUiConst.cpp
    src/out/DacSignalParams/DacSignalParamUiPulse.cpp
    src/out/DacSignalParams/DacSignalParamUiRamp.cpp

    src/in/InReceiveLauncher.cpp
    src/in/AdcReceiver.cpp
    src/in/ChannelInfo.cpp
    src/in/InDataChannelsConfig.cpp

    src/proc/AdcBlockReceivePlugin.cpp
    src/proc/AdcBlockReceiver.cpp
    src/proc/ACDCEstimatePlugin.cpp
#    src/proc/SpectrumPlugin.cpp

    src/plot/qcustomplot.cpp
    src/plot/LQMeasStudioPlot.cpp
    src/plot/PlotConfigDialog.cpp
    src/plot/BlockTimeGraphPlugin.cpp
#    src/plot/SpectrumGraphPlugin.cpp

    src/OptionalFileSelection.cpp
    src/ChannelSignalParamPanel.cpp

    lib/QTableViewHelpers/actions/TableCopyAction.cpp
    lib/QLedIndicator/QLedIndicator.cpp
)


set(HEADERS ${HEADERS}
    src/LQMeasStudio.h
    src/DataBlock.h
    )

set(QT_HEADERS ${QT_HEADERS}
    src/MainWindow.h
    src/SettingsDialog.h
    src/FreqConfigWidget.h
    src/WaitDialog.h

    src/QHexSpinBox.h

    src/ProcessPlugin.h
    src/DataBlockProcessPlugin.h


    src/devs/DeviceTree.h
    src/devs/IpAddrConfig/IpAddrConfig.h
    src/devs/IpAddrConfig/IpAddrConfigDialog.h
    src/devs/IpAddrConfig/IpAddrAddDialog.h
    src/devs/NetworkDevicesBrowser/NetworkBrowserModel.h
    src/devs/NetworkDevicesBrowser/NetworkBrowserDialog.h

    src/out/OutSigGenaration.h
    src/out/OutSignalConfigCtl.h
    src/out/DacSignalDialog.h
    src/out/DacSignalParams/DacSignalParamUi.h
    src/out/DacSignalParams/DacSignalParamUiSin.h
    src/out/DacSignalParams/DacSignalParamUiConst.h
    src/out/DacSignalParams/DacSignalParamUiPulse.h
    src/out/DacSignalParams/DacSignalParamUiRamp.h

    src/in/InReceiveLauncher.h
    src/in/AdcReceiver.h
    src/in/ChannelInfo.h
    src/in/InDataChannelsConfig.h

    src/proc/AdcBlockReceivePlugin.h
    src/proc/AdcBlockReceiver.h
    src/proc/ACDCEstimatePlugin.h
#    src/proc/SpectrumPlugin.h

    src/plot/qcustomplot.h
    src/plot/LQMeasStudioPlot.h
    src/plot/PlotConfigDialog.h
    src/plot/BlockTimeGraphPlugin.h
#    src/plot/SpectrumGraphPlugin.h

    src/OptionalFileSelection.h
    src/ChannelSignalParamPanel.h

    lib/QTableViewHelpers/actions/TableCopyAction.h
    lib/QLedIndicator/QLedIndicator.h
    )

set(QT_UI_FILES ${QT_UI_FILES}
    src/MainWindow.ui    
    src/SettingsDialog.ui
    src/WaitDialog.ui

    src/out/DacSignalDialog.ui
    src/devs/IpAddrConfig/IpAddrConfigDialog.ui
    src/devs/IpAddrConfig/IpAddrAddDialog.ui
    src/devs/NetworkDevicesBrowser/NetworkBrowserDialog.ui
    src/plot/PlotConfigDialog.ui
    )

set(RESOURCES
    icons/icons.qrc)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(src lib lib/QTableViewHelpers lib/QLedIndicator)


include_directories(${LMATH_INCLUDE_DIR})
link_directories(${LMATH_LIBRARIES_DIR})
set(LIBS ${LIBS} lmath)




#обработка файлов переводов
set(LANGUAGE_TS_FILES)
foreach(LANGUAGE ${LANGUAGES})
    set(TS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/translations/lqmeasstudio_${LANGUAGE}.ts")
    set_source_files_properties(${TS_FILE} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/translations)
    set(LANGUAGE_TS_FILES ${LANGUAGE_TS_FILES} ${TS_FILE})
endforeach(LANGUAGE ${LANGUAGES})





if(LQMEASSTUDIO_BUILD_WITH_QT5)
    find_package(Qt5Widgets)
    find_package(Qt5Network)
    find_package(Qt5PrintSupport)
    find_package(Qt5LinguistTools)

    qt5_wrap_cpp(MOC_SOURCES ${QT_HEADERS})
    qt5_wrap_ui(QT_UI_HEADERS ${QT_UI_FILES})
    qt5_add_resources(QRC_SOURCES ${RESOURCES})


    qt5_create_translation(QM_FILES ${QT_UI_HEADERS} ${LANGUAGE_TS_FILES} ${HEADERS} ${QT_HEADERS} ${SOURCES} ${MOC_SOURCES})

    set(LIBS ${LIBS} Qt5::Widgets Qt5::Network Qt5::PrintSupport)
else(LQMEASSTUDIO_BUILD_WITH_QT5)
    find_package(Qt4 REQUIRED QtGui QtCore QtNetwork)
    include(${QT_USE_FILE})
    qt4_wrap_cpp(MOC_SOURCES ${QT_HEADERS})
    qt4_wrap_ui(QT_UI_HEADERS ${QT_UI_FILES})
    qt4_add_resources(QRC_SOURCES ${RESOURCES})

    qt4_create_translation(QM_FILES ${QT_UI_HEADERS} ${LANGUAGE_TS_FILES} ${HEADERS} ${QT_HEADERS} ${SOURCES} ${MOC_SOURCES})

    set(LIBS ${LIBS} ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})
endif(LQMEASSTUDIO_BUILD_WITH_QT5)




if(LQMEASSTUDIO_BUILD_WITH_QT5)
    set(LQMEAS_BUILD_WITH_QT5 ON)
endif(LQMEASSTUDIO_BUILD_WITH_QT5)
set(LQMEAS_USE_NETWORK_BROWSER ON)
set(LQMEAS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/lqmeas)
set(LMATH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/lmath)

include(lib/lqmeas/lqmeas.cmake)
set(LQMEASSTUDIO_ADDITIONAL_FILES ${LQMEASSTUDIO_ADDITIONAL_FILES} ${LQMEAS_FILES})




if(LQMEASSTUDIO_ENABLE_DEVIFACECONFIGDIALOG)
    if(LQMEASSTUDIO_BUILD_WITH_QT5)
        set(DEVIFACECONFIG_DIALOG_BUILD_WITH_QT5 ON)
    endif(LQMEASSTUDIO_BUILD_WITH_QT5)

    set(DEVIFACECONFIG_DIALOG_DIR lib/QtDevIfaceConfigDialog)
    include_directories(${DEVIFACECONFIG_DIALOG_DIR})
    include(${DEVIFACECONFIG_DIALOG_DIR}/QtDevIfaceConfigDialog.cmake)   
    set(LQMEASSTUDIO_ADDITIONAL_FILES ${LQMEASSTUDIO_ADDITIONAL_FILES} ${DEVIFACECONFIG_DIALOG_FILES})
endif(LQMEASSTUDIO_ENABLE_DEVIFACECONFIGDIALOG)

if(LQMEASSTUDIO_ENABLE_LBOOTDIALOG)
    if(LQMEASSTUDIO_BUILD_WITH_QT5)
        set(LBOOT_DIALOG_BUILD_WITH_QT5 ON)
    endif(LQMEASSTUDIO_BUILD_WITH_QT5)
    set(LBOOT_DIALOG_DIR lib/QtLbootDialog)
    include_directories(${LBOOT_DIALOG_DIR})
    include(${LBOOT_DIALOG_DIR}/QtLbootDialog.cmake)    
    set(LQMEASSTUDIO_ADDITIONAL_FILES ${LQMEASSTUDIO_ADDITIONAL_FILES} ${LBOOT_DIALOG_FILES})
endif(LQMEASSTUDIO_ENABLE_LBOOTDIALOG)






configure_file(src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${QT_UI_HEADERS} ${QRC_SOURCES} ${MOC_SOURCES} ${LQMEASSTUDIO_ADDITIONAL_FILES} ${QM_FILES})


target_link_libraries(${PROJECT} ${LIBS} ${LQMEAS_LIBS})

if(UNIX)
    install(TARGETS ${PROJECT} DESTINATION bin)
    install(FILES ${QM_FILES}  ${LQMEAS_QM_FILES} ${LBOOT_DIALOG_QM_FILES} ${DEVIFACECONFIG_DIALOG_QM_FILES} DESTINATION ${PRJ_TRANSLATIONS_DIR})

    configure_file(${PROJECT_NAME}.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}.desktop)
    install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}.desktop DESTINATION share/applications )


    set(CMAKE_PACKAGES ${CMAKE_CURRENT_SOURCE_DIR}/packages/packages.cmake)
    if(NOT OSC_PROJECT)
        set(OSC_PROJECT      home:l-card)
    endif(NOT OSC_PROJECT)

    set(PACKAGE_NAME     ${PROJECT})
    set(PACKAGE_VERSION  ${PRJ_VERSION})
    include(${CMAKE_PACKAGES})
endif(UNIX)
