#ifndef LTRMODULESELECTION_H
#define LTRMODULESELECTION_H

#include <QObject>
#include <QStringList>
#include <QSharedPointer>

#include "lqmeas/devs/ltr/modules/LtrModule.h"
#include "lqmeas/devs/DeviceValidator.h"



class QLineEdit;
class QComboBox;
class QProgressDialog;

namespace LQMeas {
    class LtrModuleSelection : public QObject {
        Q_OBJECT
    public:
        explicit LtrModuleSelection(QStringList devs, QComboBox *crate, QComboBox *slot,
                                    QLineEdit *serial, QWidget *parent = 0,
                                    QString set_group = "lastModule");

        explicit LtrModuleSelection(QSharedPointer<DeviceValidator> validator, QComboBox *crate, QComboBox *slot,
                                    QLineEdit *serial, QWidget *parent = 0,
                                    QString set_group = "lastModule");

        ~LtrModuleSelection();


        QSharedPointer<LtrModule> currentModule() const {return m_dev;}
        QList<QSharedPointer <LtrModule> > moduleList() const {return m_devList;}

    signals:
        void beforeModuleChange();
        void moduleChanged(QSharedPointer<LQMeas::LtrModule>  dev);
    public slots:
        void reset(bool refresh = true);
        void refreshDevList(QString prefCrate=QString(), int prefSlot=0);
        void closeCurModule();

        void saveSettings();
    private slots:

        void onCrateChanged(int prefSlot=0, bool try_reopen=false);
        void onModuleChanged(bool try_reopen=false);
        void onModuleOpProgr(LQMeas::Device::OpStatus status,  QString descr, int done, int size, LQMeas::Error err);

    private:
        void init(QWidget *parent);

        QComboBox *m_crateBox;
        QComboBox *m_slotBox;
        QLineEdit *m_serialEdt;
        QStringList m_sup_devs;
        QString m_set_group;
        QList<QSharedPointer <LtrModule> > m_devList;
        QSharedPointer<LtrModule> m_dev;
        QStringList m_crateSerialList;
        QWidget *m_parent;
        QProgressDialog* m_progrDialog;
        int m_dev_reopen_cnt;

        QSharedPointer<DeviceValidator> m_validator;
    };
}

#endif // LTRMODULESELECTION_H
