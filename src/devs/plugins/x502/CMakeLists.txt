set(LQMEASSTUDIO_ENABLE_DEVIFACECONFIGDIALOG ON)
set(LQMEASSTUDIO_ENABLE_LBOOTDIALOG          ON)


set(DEVPLUGIN_SOURCES     ${DEVPLUGIN_DIR}/X502ConfigDialog.cpp
                          ${DEVPLUGIN_DIR}/X502AsyncIoDialog.cpp
                          ${DEVPLUGIN_DIR}/X502DevPlugin.cpp
                          ${DEVPLUGIN_DIR}/BlackfinFirmwareSelection.cpp
                          ${DEVPLUGIN_DIR}/ComboBoxNoWheel.cpp
                          ${DEVPLUGIN_DIR}/SpinBoxNoWheel.cpp
                          )

set(DEVPLUGIN_QT_HEADERS  ${DEVPLUGIN_DIR}/X502ConfigDialog.h
                          ${DEVPLUGIN_DIR}/X502AsyncIoDialog.h
                          ${DEVPLUGIN_DIR}/X502DevPlugin.h
                          ${DEVPLUGIN_DIR}/BlackfinFirmwareSelection.h
                          ${DEVPLUGIN_DIR}/ComboBoxNoWheel.h
                          ${DEVPLUGIN_DIR}/SpinBoxNoWheel.h
                          )

set(DEVPLIGIN_UI          ${DEVPLUGIN_DIR}/X502ConfigDialog.ui
                          ${DEVPLUGIN_DIR}/X502AsyncIoDialog.ui)

set(LQMEAS_DEVS  ${LQMEAS_DEVS} E502 L502)
