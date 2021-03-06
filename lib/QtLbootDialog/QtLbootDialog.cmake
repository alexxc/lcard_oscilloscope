

set(LBOOT_DIALOG_SOURCES 
        ${LBOOT_DIALOG_DIR}/LbootDialog.cpp
        ${LBOOT_DIALOG_DIR}/LbootOutRecord.cpp
        )
     
set(LBOOT_DIALOG_QT_HEADERS
        ${LBOOT_DIALOG_DIR}/LbootDialog.h
        ${LBOOT_DIALOG_DIR}/LbootOutRecord.h)
        
set(LBOOT_DIALOG_QT_UI_FILES
        ${LBOOT_DIALOG_DIR}/LbootDialog.ui)
        

set(LBOOT_DIALOG_TS_BASENAME qtlbootdialog)

#обработка файлов переводов
set(LBOOT_LANGUAGE_TS_FILES)
foreach(LANGUAGE ${LANGUAGES})
    set(LBOOT_TS_FILE "${LBOOT_DIALOG_DIR}/${LBOOT_DIALOG_TS_BASENAME}_${LANGUAGE}.ts")
    set_source_files_properties(${LBOOT_TS_FILE} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/translations)
    set(LBOOT_LANGUAGE_TS_FILES ${LBOOT_LANGUAGE_TS_FILES} ${LBOOT_TS_FILE})
endforeach(LANGUAGE ${LANGUAGES})

if(LBOOT_DIALOG_BUILD_WITH_QT5)
    qt5_wrap_cpp(LBOOT_DIALOG_MOC_SOURCES  ${LBOOT_DIALOG_QT_HEADERS})
    qt5_wrap_ui(LBOOT_DIALOG_QT_UI_HEADERS ${LBOOT_DIALOG_QT_UI_FILES})

    if(LBOOT_DIALOG_CREATE_TRANSLATION)
        qt5_create_translation(LBOOT_DIALOG_QM_FILES ${LBOOT_DIALOG_QT_UI_HEADERS} ${LBOOT_LANGUAGE_TS_FILES}
                            ${LBOOT_DIALOG_QT_HEADERS} ${LBOOT_DIALOG_SOURCES} ${LBOOT_DIALOG_MOC_SOURCES})
    else(LBOOT_DIALOG_CREATE_TRANSLATION)
        qt5_add_translation(LBOOT_DIALOG_QM_FILES ${LBOOT_LANGUAGE_TS_FILES})
    endif(LBOOT_DIALOG_CREATE_TRANSLATION)
else(LBOOT_DIALOG_BUILD_WITH_QT5)
    qt4_wrap_cpp(LBOOT_DIALOG_MOC_SOURCES  ${LBOOT_DIALOG_QT_HEADERS})
    qt4_wrap_ui(LBOOT_DIALOG_QT_UI_HEADERS ${LBOOT_DIALOG_QT_UI_FILES})

    if(LBOOT_DIALOG_CREATE_TRANSLATION)
        qt4_create_translation(LBOOT_DIALOG_QM_FILES ${LBOOT_DIALOG_QT_UI_HEADERS} ${LBOOT_LANGUAGE_TS_FILES}
                            ${LBOOT_DIALOG_QT_HEADERS} ${LBOOT_DIALOG_SOURCES} ${LBOOT_DIALOG_MOC_SOURCES})
    else(LBOOT_DIALOG_CREATE_TRANSLATION)
        qt4_add_translation(LBOOT_DIALOG_QM_FILES ${LBOOT_LANGUAGE_TS_FILES})
    endif(LBOOT_DIALOG_CREATE_TRANSLATION)
endif(LBOOT_DIALOG_BUILD_WITH_QT5)
        



        
set(LBOOT_DIALOG_FILES  ${LBOOT_DIALOG_SOURCES} ${LBOOT_DIALOG_QT_HEADERS} ${LBOOT_DIALOG_QT_UI_FILES}   
                        ${LBOOT_DIALOG_MOC_SOURCES} ${LBOOT_DIALOG_QT_UI_HEADERS}     
                        ${LBOOT_DIALOG_QM_FILES})
