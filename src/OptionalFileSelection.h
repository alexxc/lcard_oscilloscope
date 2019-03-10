#ifndef OPTIALFILESELECTION_H
#define OPTIALFILESELECTION_H

#include <QObject>

class QCheckBox;
class QLineEdit;
class QWidget;

class OptionalFileSelection : public QObject {
    Q_OBJECT
public:
    OptionalFileSelection(QWidget *parent, QCheckBox *enBox, QLineEdit *edt, bool change_enabled);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);


    virtual QString fileDlgCaption() const = 0;
    virtual QString fileDlgFilters() const = 0;
    virtual QString fileLastdirSettingsEntry() const = 0;
private Q_SLOTS:
    void onEnabledChanged(int state);
private:
    void startFileSelection();

    QWidget   *m_parentWgt;
    QCheckBox *m_enBox;
    QLineEdit *m_edt;
};

#endif // OPTIALFILESELECTION_H
