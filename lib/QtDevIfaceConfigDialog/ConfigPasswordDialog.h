#ifndef CONFIGPASSWORDDIALOG_H
#define CONFIGPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigPasswordDialog;
}

class ConfigPasswordDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConfigPasswordDialog(QWidget *parent = 0);
    ~ConfigPasswordDialog();

    QString passwd() const;
private:
    Ui::ConfigPasswordDialog *ui;
};

#endif // CONFIGPASSWORDDIALOG_H
