# Файл для включения в проект CMAKE.
# Перед включением библиотека Qt нужной версии должна быть уже обнаружена
#
# На выходе нужно просто добавить файлы из LQMEAS_FILES в список файлов проекта
# слинковать проект с библиотеками из LQMEAS_LIBS
#
# Перед включением могут быть установленны следующие конфигурационные переменные:
# LQMEAS_DIR                 - директория, в которой находится данный файл относительно корня
#                              проекта или абсолютный путь (по умолчанию lib/lqmeas)
# LQMEAS_DEVS                - список устройств, поддержка которых должна быть включена
# LQMEAS_BUILD_WITH_QT5      - если ON, то для сборки будет использоваться Qt5, иначе - Qt4
# LQMEAS_TRANSLATION_DISABLE - запрет создания файлов переводов (не требует зависимости от LinguistTools
# LQMEAS_NO_GUI              - сборка с зависимостью только от QtCore
#
# Включение дополнительных компонентов:
#  LQMEAS_USE_NETWORK_BROWSER - если ON, то включается код для поддержки поиска устройств
#                               в локальной сети
#  LQMEAS_GUI_LTR_MODULE_SELECTION - включение компонента для помощьника реализации
#                                    интерфейса выбора одного модуля LTR
#  LQMEAS_SERVICE_MULTIMETER_THREAD - включение реализации потока для функции стандартного
#                                     мультиметра
#




if(NOT LQMEAS_DIR)
    set(LQMEAS_DIR lib/lqmeas)
endif(NOT LQMEAS_DIR)

include_directories(${LQMEAS_DIR}/..)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LQMEAS_DIR}/cmake/Modules/")

set(LQMEAS_SOURCES ${LQMEAS_SOURCES}

    ${LQMEAS_DIR}/units/Units.cpp

    ${LQMEAS_DIR}/log/Log.cpp



    ${LQMEAS_DIR}/devs/Device.cpp
    ${LQMEAS_DIR}/devs/DeviceRef.cpp
    ${LQMEAS_DIR}/devs/DeviceConfig.cpp
    ${LQMEAS_DIR}/devs/DeviceTypeInfo.cpp
    ${LQMEAS_DIR}/devs/resolver/DevicesResolver.cpp


    ${LQMEAS_DIR}/ifaces/out/OutSignalFabric.cpp
    ${LQMEAS_DIR}/ifaces/out/DacSignals/DacSignalSin.cpp
    ${LQMEAS_DIR}/ifaces/out/DacSignals/DacSignalConst.cpp
    ${LQMEAS_DIR}/ifaces/out/DacSignals/DacSignalPulse.cpp
    ${LQMEAS_DIR}/ifaces/out/DacSignals/DacSignalRamp.cpp

    ${LQMEAS_DIR}/ifaces/out/DigSignals/DigSignalPulse.cpp

    ${LQMEAS_DIR}/ifaces/out/DevOutSync.cpp
    ${LQMEAS_DIR}/ifaces/out/DevOutSyncConfig.cpp
    ${LQMEAS_DIR}/ifaces/out/DevOutSyncStatusTracker.cpp
    ${LQMEAS_DIR}/ifaces/out/DevOutAsyncDac.cpp
    ${LQMEAS_DIR}/ifaces/out/DevOutAsyncDig.cpp
    ${LQMEAS_DIR}/ifaces/out/OutRamSignalGenerator.cpp
    ${LQMEAS_DIR}/ifaces/out/SyncModes/DevOutSyncModeImplStream.cpp
    ${LQMEAS_DIR}/ifaces/out/SyncModes/DevOutSyncModeImplCycle.cpp
    ${LQMEAS_DIR}/ifaces/out/SyncModes/DevOutSyncStreamThread.cpp

    ${LQMEAS_DIR}/ifaces/in/DevAdc.cpp
    ${LQMEAS_DIR}/ifaces/in/DevInAsyncDig.cpp
    ${LQMEAS_DIR}/ifaces/in/DevAdcConfig.cpp
    ${LQMEAS_DIR}/ifaces/in/DevAdcStdSeq32Config.cpp
   )
   
set(LQMEAS_HEADERS ${LQMEAS_HEADERS}
    ${LQMEAS_DIR}/Error.h
    ${LQMEAS_DIR}/lqtdefs.h

    ${LQMEAS_DIR}/EnumNamedValue.h

    ${LQMEAS_DIR}/units/Unit.h

    ${LQMEAS_DIR}/log/LogLevel.h
    ${LQMEAS_DIR}/log/LogMessage.h


    ${LQMEAS_DIR}/devs/DeviceInfo.h
    ${LQMEAS_DIR}/devs/DeviceRef.h
    ${LQMEAS_DIR}/devs/DeviceConfig.h
    ${LQMEAS_DIR}/devs/DeviceTypeInfo.h

    ${LQMEAS_DIR}/devs/DeviceValidator.h
    ${LQMEAS_DIR}/devs/DeviceNameValidator.h


    ${LQMEAS_DIR}/devs/resolver/DevicesResolver.h
    ${LQMEAS_DIR}/devs/resolver/DevicesResolverIpAddr.h
    ${LQMEAS_DIR}/devs/DeviceFrameReceiver.h
    ${LQMEAS_DIR}/devs/DeviceFrameSender.h


    ${LQMEAS_DIR}/ifaces/out/OutSignal.h

    ${LQMEAS_DIR}/ifaces/out/DacSignals/DacSignal.h
    ${LQMEAS_DIR}/ifaces/out/DacSignals/DacSignalSin.h
    ${LQMEAS_DIR}/ifaces/out/DacSignals/DacSignalConst.h
    ${LQMEAS_DIR}/ifaces/out/DacSignals/DacSignalPulse.h
    ${LQMEAS_DIR}/ifaces/out/DacSignals/DacSignalRamp.h

    ${LQMEAS_DIR}/ifaces/out/DigSignals/DigSignal.h
    ${LQMEAS_DIR}/ifaces/out/DigSignals/DigSignalPulse.h

    ${LQMEAS_DIR}/ifaces/out/DevOutInfo.h
    ${LQMEAS_DIR}/ifaces/out/DevOutSync.h
    ${LQMEAS_DIR}/ifaces/out/DevOutSyncConfig.h
    ${LQMEAS_DIR}/ifaces/out/DevOutAsyncDac.h
    ${LQMEAS_DIR}/ifaces/out/DevOutAsyncDig.h
    ${LQMEAS_DIR}/ifaces/out/OutRamSignalGenerator.h

    ${LQMEAS_DIR}/ifaces/out/SyncModes/DevOutSyncIfaceSend.h
    ${LQMEAS_DIR}/ifaces/out/SyncModes/DevOutSyncIfaceStream.h
    ${LQMEAS_DIR}/ifaces/out/SyncModes/DevOutSyncIfaceCycle.h

    ${LQMEAS_DIR}/ifaces/in/DevAdc.h
    ${LQMEAS_DIR}/ifaces/in/DevInAsyncDig.h
    ${LQMEAS_DIR}/ifaces/in/DevAdcInfo.h
    ${LQMEAS_DIR}/ifaces/in/DevAdcConfig.h
    ${LQMEAS_DIR}/ifaces/in/DevAdcStdSeq32Config.h
   )
   
set(LQMEAS_QT_HEADERS ${LQMEAS_QT_HEADERS}
    ${LQMEAS_DIR}/StdErrors.h

    ${LQMEAS_DIR}/units/Units.h

    ${LQMEAS_DIR}/log/Log.h    

    ${LQMEAS_DIR}/devs/Device.h


    ${LQMEAS_DIR}/ifaces/out/OutSignalFabric.h
    ${LQMEAS_DIR}/ifaces/out/DevOutSyncStatusTracker.h
    ${LQMEAS_DIR}/ifaces/out/SyncModes/DevOutSyncModeImpl.h
    ${LQMEAS_DIR}/ifaces/out/SyncModes/DevOutSyncModeImplStream.h
    ${LQMEAS_DIR}/ifaces/out/SyncModes/DevOutSyncModeImplCycle.h
    ${LQMEAS_DIR}/ifaces/out/SyncModes/DevOutSyncStreamThread.h
    )

if(NOT LQMEAS_NO_GUI)
    set(LQMEAS_SOURCES ${LQMEAS_SOURCES}
        ${LQMEAS_DIR}/log/LogModel.cpp
        ${LQMEAS_DIR}/log/LogPanel.cpp
        )
    set(LQMEAS_QT_HEADERS ${LQMEAS_QT_HEADERS}
        ${LQMEAS_DIR}/log/LogModel.h
        ${LQMEAS_DIR}/log/LogPanel.h)
endif(NOT LQMEAS_NO_GUI)




if(LQMEAS_USE_NETWORK_BROWSER)
    set(LQMEAS_HEADERS ${LQMEAS_HEADERS}
        ${LQMEAS_DIR}/devs/resolver/DeviceSvcRecord.h)
    set(LQMEAS_QT_HEADERS  ${LQMEAS_QT_HEADERS}
        ${LQMEAS_DIR}/devs/resolver/DeviceNetworkBrowser.h)
    set(LQMEAS_SOURCES ${LQMEAS_SOURCES}
        ${LQMEAS_DIR}/devs/resolver/DeviceNetworkBrowser.cpp)
endif(LQMEAS_USE_NETWORK_BROWSER)



set(LTR_MODULES LTR11 LTR24 LTR25 LTR35 LTR51 LTR210)

foreach(LTR_MODULE ${LTR_MODULES})
    list(FIND LQMEAS_DEVS ${LTR_MODULE} ${LTR_MODULE}_IDX)
    if(NOT ${LTR_MODULE}_IDX EQUAL -1)
        string(TOLOWER ${LTR_MODULE} LTR_MODULE_LOWER)

        set(LQMEAS_DEV_${LTR_MODULE} ON)
        set(LQMEAS_DEV_LTR           ON)

        set(LTR_MODULE_DIR ${LQMEAS_DIR}/devs/LTR/modules/${LTR_MODULE})

        set(LQMEAS_SOURCES ${LQMEAS_SOURCES}
            ${LTR_MODULE_DIR}/${LTR_MODULE}.cpp
            ${LTR_MODULE_DIR}/${LTR_MODULE}Config.cpp
            ${LTR_MODULE_DIR}/${LTR_MODULE}TypeInfo.cpp)
        set(LQMEAS_QT_HEADERS ${LQMEAS_QT_HEADERS}
            ${LTR_MODULE_DIR}/${LTR_MODULE}.h)
        set(LQMEAS_HEADERS ${LQMEAS_HEADERS}
            ${LTR_MODULE_DIR}/${LTR_MODULE}Config.h
            ${LTR_MODULE_DIR}/${LTR_MODULE}TypeInfo.h
            ${LTR_MODULE_DIR}/${LTR_MODULE}Info.h)

        set(LQMEAS_LTRAPI_COMPONENTS ${LQMEAS_LTRAPI_COMPONENTS} ${LTR_MODULE_LOWER}api)
    endif()
endforeach()

if(LQMEAS_DEV_LTR)

    set(LQMEAS_LTRAPI_COMPONENTS ${LQMEAS_LTRAPI_COMPONENTS} ltrapi)

    find_package(LTRAPI REQUIRED ${LQMEAS_LTRAPI_COMPONENTS})

    include_directories(${LTRAPI_INCLUDE_DIRS})
    add_definitions(${LTRAPI_DEFINITIONS})
    set(LQMEAS_LIBS ${LQMEAS_LIBS} ${LTRAPI_LIBRARIES})

    set(LQMEAS_QT_HEADERS ${LQMEAS_QT_HEADERS}
        ${LQMEAS_DIR}/devs/LTR/crates/LTRCrate.h
        ${LQMEAS_DIR}/devs/LTR/modules/LTRModule.h
    )

    set(LQMEAS_HEADERS ${LQMEAS_HEADERS}        
        ${LQMEAS_DIR}/devs/LTR/LTRResolver.h
        ${LQMEAS_DIR}/devs/LTR/LTRModuleRef.h
        ${LQMEAS_DIR}/devs/LTR/crates/LTRCrateTypeInfo.h
        ${LQMEAS_DIR}/devs/LTR/crates/LTRCrateInfo.h
        ${LQMEAS_DIR}/devs/LTR/modules/LTRMezzanine.h
    )

    set(LQMEAS_SOURCES ${LQMEAS_SOURCES}
        ${LQMEAS_DIR}/devs/LTR/LTRResolver.cpp
        ${LQMEAS_DIR}/devs/LTR/LTRModuleRef.cpp
        ${LQMEAS_DIR}/devs/LTR/crates/LTRCrateTypeInfo.cpp
        ${LQMEAS_DIR}/devs/LTR/modules/LTRModule.cpp
        ${LQMEAS_DIR}/devs/LTR/crates/LTRCrate.cpp
    )   
endif(LQMEAS_DEV_LTR)



set(LCOMP_DEVICES E2010 E14_440 E14_140)

foreach(LCOMP_DEVICE ${LCOMP_DEVICES})
    list(FIND LQMEAS_DEVS ${LCOMP_DEVICE} ${LCOMP_DEVICE}_IDX)
    if(NOT ${LCOMP_DEVICE}_IDX EQUAL -1)
        string(TOLOWER ${LCOMP_DEVICE} LCOMP_MODULE_LOWER)

        set(LQMEAS_DEV_${LCOMP_DEVICE}  ON)
        set(LQMEAS_DEV_LCOMP            ON)
        set(LQMEAS_DEV_DIR ${LQMEAS_DIR}/devs/lcomp/modules/${LCOMP_DEVICE})

        set(LQMEAS_SOURCES ${LQMEAS_SOURCES}
            ${LQMEAS_DEV_DIR}/${LCOMP_DEVICE}.cpp
            ${LQMEAS_DEV_DIR}/${LCOMP_DEVICE}Config.cpp
            ${LQMEAS_DEV_DIR}/${LCOMP_DEVICE}TypeInfo.cpp)
        set(LQMEAS_QT_HEADERS ${LQMEAS_QT_HEADERS}
           ${LQMEAS_DEV_DIR}/${LCOMP_DEVICE}.h)
        set(LQMEAS_HEADERS ${LQMEAS_HEADERS}
            ${LQMEAS_DEV_DIR}/${LCOMP_DEVICE}Config.h
            ${LQMEAS_DEV_DIR}/${LCOMP_DEVICE}Info.h
            ${LQMEAS_DEV_DIR}/${LCOMP_DEVICE}TypeInfo.h
            )
    endif()
endforeach()

if(LQMEAS_DEV_LCOMP)

    if(WIN32 AND NOT LCOMP_LIBRARY_ROOT_DIR)
        set(LCOMP_LIBRARY_ROOT_DIR "c:/Program Files (x86)/L-Card/LIBRARY")
    endif()
    include_directories(${LCOMP_LIBRARY_ROOT_DIR})

    set(LQMEAS_QT_HEADERS ${LQMEAS_QT_HEADERS}
        ${LQMEAS_DIR}/devs/lcomp/lcompDevice.h
        ${LQMEAS_DIR}/devs/lcomp/lcompStreamDevice.h
        ${LQMEAS_DIR}/devs/lcomp/lcompStreamReceiver.h
        ${LQMEAS_DIR}/devs/lcomp/lcompErrors.h
    )

    set(LQMEAS_HEADERS ${LQMEAS_HEADERS}
        ${LQMEAS_DIR}/devs/lcomp/lcomp_headers.h
        ${LQMEAS_DIR}/devs/lcomp/lcompResolver.h
        ${LQMEAS_DIR}/devs/lcomp/lcompRecvProcessorIface.h
        ${LQMEAS_DIR}/devs/lcomp/lcompSeqAdcConfig.h
    )

    set(LQMEAS_SOURCES ${LQMEAS_SOURCES}
        ${LQMEAS_DIR}/devs/lcomp/lcompResolver.cpp
        ${LQMEAS_DIR}/devs/lcomp/lcompDevice.cpp
        ${LQMEAS_DIR}/devs/lcomp/lcompStreamDevice.cpp
        ${LQMEAS_DIR}/devs/lcomp/lcompStreamReceiver.cpp
        ${LQMEAS_DIR}/devs/lcomp/lcompSeqAdcConfig.cpp
    )
endif(LQMEAS_DEV_LCOMP)



set(X502_DEVICES E502 L502)
foreach(X502_DEVICE ${X502_DEVICES})    
    list(FIND LQMEAS_DEVS ${X502_DEVICE} ${X502_DEVICE}_IDX)
    if(NOT ${X502_DEVICE}_IDX EQUAL -1)
        string(TOLOWER ${X502_DEVICE}  X502_DEVICE_LOWER)
        set(LQMEAS_DEV_${X502_DEVICE}  ON)
        set(LQMEAS_DEV_X502            ON)
        set(X502API_COMPONENTS         ${X502API_COMPONENTS} ${X502_DEVICE_LOWER}api)

        set(LQMEAS_DEV_DIR ${LQMEAS_DIR}/devs/x502/modules/${X502_DEVICE})

        set(LQMEAS_QT_HEADERS ${LQMEAS_QT_HEADERS}
            ${LQMEAS_DEV_DIR}/${X502_DEVICE}.h
        )

        set(LQMEAS_HEADERS ${LQMEAS_HEADERS}
            ${LQMEAS_DEV_DIR}/${X502_DEVICE}TypeInfo.h
            ${LQMEAS_DEV_DIR}/${X502_DEVICE}Info.h
        )

        set(LQMEAS_SOURCES ${LQMEAS_SOURCES}
            ${LQMEAS_DEV_DIR}/${X502_DEVICE}.cpp
            ${LQMEAS_DEV_DIR}/${X502_DEVICE}TypeInfo.cpp
        )
    endif()
endforeach()


if(LQMEAS_DEV_X502)
    find_package(X502API REQUIRED ${X502API_COMPONENTS})

    include_directories(${X502API_INCLUDE_DIRS})
    set(LQMEAS_LIBS ${LQMEAS_LIBS} ${X502API_LIBRARIES})

    set(LQMEAS_QT_HEADERS ${LQMEAS_QT_HEADERS}
        ${LQMEAS_DIR}/devs/x502/x502.h
    )

    set(LQMEAS_HEADERS ${LQMEAS_HEADERS}
        ${LQMEAS_DIR}/devs/x502/x502TypeInfo.h
        ${LQMEAS_DIR}/devs/x502/x502Info.h
        ${LQMEAS_DIR}/devs/x502/x502Config.h
        ${LQMEAS_DIR}/devs/x502/x502Resolver.h
        ${LQMEAS_DIR}/devs/x502/e502SvcRecord.h
    )

    set(LQMEAS_SOURCES ${LQMEAS_SOURCES}
        ${LQMEAS_DIR}/devs/x502/x502.cpp
        ${LQMEAS_DIR}/devs/x502/x502TypeInfo.cpp
        ${LQMEAS_DIR}/devs/x502/x502Config.cpp
        ${LQMEAS_DIR}/devs/x502/x502Resolver.cpp
        ${LQMEAS_DIR}/devs/x502/e502SvcRecord.cpp
    )

    if(LQMEAS_USE_NETWORK_BROWSER)
        set(LQMEAS_QT_HEADERS ${LQMEAS_QT_HEADERS}
            ${LQMEAS_DIR}/devs/x502/e502NetBrowser.h
        )
        set(LQMEAS_SOURCES ${LQMEAS_SOURCES}
            ${LQMEAS_DIR}/devs/x502/e502NetBrowser.cpp)
    endif(LQMEAS_USE_NETWORK_BROWSER)
endif(LQMEAS_DEV_X502)



list(FIND LQMEAS_DEVS Agilent3458 Agilent3458_IDX)
if(NOT Agilent3458_IDX EQUAL -1)
    set(LQMEAS_DEV_AGILENT_3458  ON)

    include_directories(${AG3458API_INCLUDE_DIR})
    link_directories(${AG3458API_LIBRARIES_DIR})

    set(LQMEAS_LIBS ${LQMEAS_LIBS} Agilent3458api)

    set(LQMEAS_QT_HEADERS ${LQMEAS_QT_HEADERS}

        ${LQMEAS_DIR}/devs/gpib/gpibResolver.h
        ${LQMEAS_DIR}/devs/gpib/Agilent3458.h
    )
    set(LQMEAS_HEADERS ${LQMEAS_HEADERS}
    )

    set(LQMEAS_SOURCES ${LQMEAS_SOURCES}

        ${LQMEAS_DIR}/devs/gpib/gpibResolver.cpp
        ${LQMEAS_DIR}/devs/gpib/Agilent3458.cpp
    )

endif()


if(LQMEAS_GUI_LTR_MODULE_SELECTION)
    set(LQMEAS_SOURCES ${LQMEAS_SOURCES}
        ${LQMEAS_DIR}/gui/LtrModuleSelection/LtrModuleSelection.cpp)
    set(LQMEAS_QT_HEADERS ${LQMEAS_QT_HEADERS}
        ${LQMEAS_DIR}/gui/LtrModuleSelection/LtrModuleSelection.h)
endif(LQMEAS_GUI_LTR_MODULE_SELECTION)

if(LQMEAS_SERVICE_MULTIMETER_THREAD)
    set(LQMEAS_USE_LMATH ON)
    set(LQMEAS_SOURCES ${LQMEAS_SOURCES}
        ${LQMEAS_DIR}/services/MultimeterThread/MultimeterThread.cpp
        ${LQMEAS_DIR}/services/MultimeterThread/MultimeterProcessor.cpp)
    set(LQMEAS_QT_HEADERS ${LQMEAS_QT_HEADERS}
        ${LQMEAS_DIR}/services/MultimeterThread/MultimeterThread.h
        ${LQMEAS_DIR}/services/MultimeterThread/MultimeterProcessor.h)
endif(LQMEAS_SERVICE_MULTIMETER_THREAD)

if(LQMEAS_USE_LMATH)
    include_directories(${LMATH_INCLUDE_DIR})
     link_directories(${LMATH_LIBRARIES_DIR})

    set(LQMEAS_LIBS ${LQMEAS_LIBS} lmath)
endif(LQMEAS_USE_LMATH)


configure_file(${LQMEAS_DIR}/lqmeas_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/lqmeas_config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})


set(LQMEAS_TS_BASENAME lqmeas)

#обработка файлов переводов
set(LQMEAS_LANGUAGE_TS_FILES)
foreach(LANGUAGE ${LANGUAGES})
    set(LQMEAS_TS_FILE "${LQMEAS_DIR}/${LQMEAS_TS_BASENAME}_${LANGUAGE}.ts")
    set_source_files_properties(${LQMEAS_TS_FILE} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/translations)
    set(LQMEAS_LANGUAGE_TS_FILES ${LQMEAS_LANGUAGE_TS_FILES} ${LQMEAS_TS_FILE})
endforeach(LANGUAGE ${LANGUAGES})


if(NOT CMAKE_AUTOMOC)
    if(LQMEAS_BUILD_WITH_QT5)
        qt5_wrap_cpp(LQMEAS_MOC_SOURCES  ${LQMEAS_QT_HEADERS})
    else(LQMEAS_BUILD_WITH_QT5)
        qt4_wrap_cpp(LQMEAS_MOC_SOURCES  ${LQMEAS_QT_HEADERS})
    endif(LQMEAS_BUILD_WITH_QT5)
endif(NOT CMAKE_AUTOMOC)

if(LQMEAS_QT_UI_HEADERS AND NOT LQMEAS_NO_GUI AND NOT CMAKE_AUTOUIC)
    if(LQMEAS_BUILD_WITH_QT5)
        qt5_wrap_ui(LQMEAS_QT_UI_HEADERS ${LQMEAS_QT_UI_FILES})
    else(LQMEAS_BUILD_WITH_QT5)
        qt4_wrap_ui(LQMEAS_QT_UI_HEADERS ${LQMEAS_QT_UI_FILES})
    endif(LQMEAS_BUILD_WITH_QT5)
endif(LQMEAS_QT_UI_HEADERS AND NOT LQMEAS_NO_GUI AND NOT CMAKE_AUTOUIC)


if(NOT LQMEAS_TRANSLATION_DISABLE)
    if(LQMEAS_BUILD_WITH_QT5)
        if(LQMEAS_CREATE_TRANSLATION)
            qt5_create_translation(LQMEAS_QM_FILES ${LQMEAS_QT_UI_HEADERS} ${LQMEAS_LANGUAGE_TS_FILES}
                                ${LQMEAS_QT_HEADERS} ${LQMEAS_HEADERS} ${LQMEAS_SOURCES} ${LQMEAS_MOC_SOURCES})
        else(LQMEAS_CREATE_TRANSLATION)
            qt5_add_translation(LQMEAS_QM_FILES ${LQMEAS_LANGUAGE_TS_FILES})
        endif(LQMEAS_CREATE_TRANSLATION)
    else(LQMEAS_BUILD_WITH_QT5)
        if(LQMEAS_CREATE_TRANSLATION)
            qt4_create_translation(LQMEAS_QM_FILES ${LQMEAS_QT_UI_HEADERS} ${LQMEAS_LANGUAGE_TS_FILES}
                                ${LQMEAS_QT_HEADERS} ${LQMEAS_HEADERS} ${LQMEAS_SOURCES} ${LQMEAS_MOC_SOURCES})
        else(LQMEAS_CREATE_TRANSLATION)
            qt4_add_translation(LQMEAS_QM_FILES ${LQMEAS_LANGUAGE_TS_FILES})
        endif(LQMEAS_CREATE_TRANSLATION)
    endif(LQMEAS_BUILD_WITH_QT5)
endif(NOT LQMEAS_TRANSLATION_DISABLE)


set(LQMEAS_FILES  ${LQMEAS_SOURCES} ${LQMEAS_QT_HEADERS} ${LQMEAS_HEADERS}
                   ${LQMEAS_QT_UI_FILES} ${LQMEAS_MOC_SOURCES} ${LQMEAS_QT_UI_HEADERS}
                   ${LQMEAS_QM_FILES})

