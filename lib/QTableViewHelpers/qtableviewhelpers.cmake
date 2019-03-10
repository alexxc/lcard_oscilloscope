cmake_minimum_required(VERSION 2.8.11)

if(NOT QTABLEVIEWHELPERS_DIR)
    set(QTABLEVIEWHELPERS_DIR "${CMAKE_SOURCE_DIR}/lib/QTableViewHelpers")
endif(NOT QTABLEVIEWHELPERS_DIR)

set(QTABLEVIEWHELPERS_SOURCES
    ${QTABLEVIEWHELPERS_DIR}/TableViewSettings.cpp
    ${QTABLEVIEWHELPERS_DIR}/delegates/BoolBoxEditorDelegate.cpp
    ${QTABLEVIEWHELPERS_DIR}/delegates/CheckBoxItemDelegate.cpp
    ${QTABLEVIEWHELPERS_DIR}/delegates/ComboBoxEditorDelegate.cpp
    ${QTABLEVIEWHELPERS_DIR}/delegates/DoubleSpinBoxDelegate.cpp
    ${QTABLEVIEWHELPERS_DIR}/actions/TableCopyAction.cpp
    )

set(QTABLEVIEWHELPERS_HEADERS
    ${QTABLEVIEWHELPERS_DIR}/TableParameter.h
    ${QTABLEVIEWHELPERS_DIR}/TableParamsModelHelper.h
    ${QTABLEVIEWHELPERS_DIR}/TableParamsModelIface.h
    ${QTABLEVIEWHELPERS_DIR}/TableViewSettings.h
    ${QTABLEVIEWHELPERS_DIR}/delegates/BoolBoxEditorDelegate.h
    ${QTABLEVIEWHELPERS_DIR}/delegates/CheckBoxItemDelegate.h
    ${QTABLEVIEWHELPERS_DIR}/delegates/ComboBoxEditorDelegate.h
    ${QTABLEVIEWHELPERS_DIR}/delegates/DoubleSpinBoxDelegate.h
    ${QTABLEVIEWHELPERS_DIR}/delegates/DelegateIndexSelector.h
    ${QTABLEVIEWHELPERS_DIR}/actions/TableCopyAction.h
    )


set(QTABLEVIEWHELPERS_FILES  ${QTABLEVIEWHELPERS_SOURCES} ${QTABLEVIEWHELPERS_HEADERS})
