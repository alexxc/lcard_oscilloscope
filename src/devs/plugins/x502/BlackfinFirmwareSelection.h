#ifndef BLACKFINFIRMWARESELECTION_H
#define BLACKFINFIRMWARESELECTION_H

#include "OptionalFileSelection.h"

class BlackfinFirmwareSelection : public OptionalFileSelection {
    Q_OBJECT
public:
    BlackfinFirmwareSelection(QWidget *parent, QCheckBox *enBox, QLineEdit *edt, bool change_enabled);

    virtual QString fileDlgCaption() const { return tr("Blackfin firmware file selection"); }
    virtual QString fileDlgFilters() const { return tr("Firmware files") + "(*.ldr);;" + tr("All files") +  "(*.*)";}
    virtual QString fileLastdirSettingsEntry() const {return "X502LastBfinFirmDir";}
};

#endif // BLACKFINFIRMWARESELECTION_H
