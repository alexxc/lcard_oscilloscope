set(DEVIFACECONFIG_DIALOG_SOURCES 
        ${DEVIFACECONFIG_DIALOG_DIR}/DevIfaceConfigDialog.cpp
        ${DEVIFACECONFIG_DIALOG_DIR}/ConfigPasswordDialog.cpp
        ${DEVIFACECONFIG_DIALOG_DIR}/SvcInstanceNameValidator.cpp
        )
     
set(DEVIFACECONFIG_DIALOG_QT_HEADERS
        ${DEVIFACECONFIG_DIALOG_DIR}/DevIfaceConfigDialog.h
        ${DEVIFACECONFIG_DIALOG_DIR}/ConfigPasswordDialog.h
        ${DEVIFACECONFIG_DIALOG_DIR}/SvcInstanceNameValidator.h
        )
        
set(DEVIFACECONFIG_DIALOG_QT_UI_FILES
        ${DEVIFACECONFIG_DIALOG_DIR}/DevIfaceConfigDialog.ui
        ${DEVIFACECONFIG_DIALOG_DIR}/ConfigPasswordDialog.ui)
        

set(DEVIFACECONFIG_DIALOG_TS_BASENAME qtdevifaceconfigdialog)
#обработка файлов переводов
set(DEVIFACECONFIG_LANGUAGE_TS_FILES)
foreach(LANGUAGE ${LANGUAGES})
    set(DEVIFACECONFIG_TS_FILE "${DEVIFACECONFIG_DIALOG_DIR}/${DEVIFACECONFIG_DIALOG_TS_BASENAME}_${LANGUAGE}.ts")
    set_source_files_properties(${DEVIFACECONFIG_TS_FILE} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/translations)
    set(DEVIFACECONFIG_LANGUAGE_TS_FILES ${DEVIFACECONFIG_LANGUAGE_TS_FILES} ${DEVIFACECONFIG_TS_FILE})
endforeach(LANGUAGE ${LANGUAGES})

if(DEVIFACECONFIG_DIALOG_BUILD_WITH_QT5)
    qt5_wrap_cpp(DEVIFACECONFIG_DIALOG_MOC_SOURCES  ${DEVIFACECONFIG_DIALOG_QT_HEADERS})
    qt5_wrap_ui(DEVIFACECONFIG_DIALOG_QT_UI_HEADERS ${DEVIFACECONFIG_DIALOG_QT_UI_FILES})

    if(DEVIFACECONFIG_DIALOG_CREATE_TRANSLATION)
        qt5_create_translation(DEVIFACECONFIG_DIALOG_QM_FILES ${DEVIFACECONFIG_DIALOG_QT_UI_HEADERS} ${DEVIFACECONFIG_LANGUAGE_TS_FILES}
                            ${DEVIFACECONFIG_DIALOG_QT_HEADERS} ${DEVIFACECONFIG_DIALOG_SOURCES} ${DEVIFACECONFIG_DIALOG_MOC_SOURCES})
    else()
        qt5_add_translation(DEVIFACECONFIG_DIALOG_QM_FILES ${DEVIFACECONFIG_LANGUAGE_TS_FILES})
    endif()
else(DEVIFACECONFIG_DIALOG_BUILD_WITH_QT5)
    qt4_wrap_cpp(DEVIFACECONFIG_DIALOG_MOC_SOURCES  ${DEVIFACECONFIG_DIALOG_QT_HEADERS})
    qt4_wrap_ui(DEVIFACECONFIG_DIALOG_QT_UI_HEADERS ${DEVIFACECONFIG_DIALOG_QT_UI_FILES})

    if(DEVIFACECONFIG_DIALOG_CREATE_TRANSLATION)
        qt4_create_translation(DEVIFACECONFIG_DIALOG_QM_FILES ${DEVIFACECONFIG_DIALOG_QT_UI_HEADERS} ${DEVIFACECONFIG_LANGUAGE_TS_FILES}
                            ${DEVIFACECONFIG_DIALOG_QT_HEADERS} ${DEVIFACECONFIG_DIALOG_SOURCES} ${DEVIFACECONFIG_DIALOG_MOC_SOURCES})
    else()
        qt4_add_translation(DEVIFACECONFIG_DIALOG_QM_FILES ${DEVIFACECONFIG_LANGUAGE_TS_FILES})
    endif()
endif(DEVIFACECONFIG_DIALOG_BUILD_WITH_QT5)
        

        
set(DEVIFACECONFIG_DIALOG_FILES  ${DEVIFACECONFIG_DIALOG_SOURCES} ${DEVIFACECONFIG_DIALOG_QT_HEADERS} ${DEVIFACECONFIG_DIALOG_QT_UI_FILES}   
                        ${DEVIFACECONFIG_DIALOG_MOC_SOURCES} ${DEVIFACECONFIG_DIALOG_QT_UI_HEADERS}     
                        ${DEVIFACECONFIG_DIALOG_QM_FILES})
