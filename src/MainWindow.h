#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QSharedPointer>
#include <QList>
#include <QHash>
#include <QTranslator>
#include <QSharedPointer>
#include <QThread>

#include "SettingsDialog.h"
//#include "config.h"
#include "lqmeas/Error.h"
#include "lqmeas/devs/Device.h"

namespace Ui {
    class MainWindow;
}

class QTreeWidgetItem;
class IpAddrConfig;


namespace LQMeas {
    class LogPanel;
}

namespace LQMeasStudio {
    class ChannelSignalParamPanel;

    class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
		explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;

	protected:
        void closeEvent ( QCloseEvent *event );
        void changeEvent ( QEvent *event );

	private slots:
		void on_actionDraw_Plot_toggled(bool arg1);

        void saveSettings();
        void clearStreamResources();

        void showDeviceError(const QSharedPointer<LQMeas::Device>& dev, const LQMeas::Error& err, const QString& caption, const QString& msg);        
        void onBrowseError(const LQMeas::Error& err);
        void indicateOutGenUnderflow(const QSharedPointer<LQMeas::Device>& dev);

        void on_actionStart_triggered();
        void on_actionStop_triggered();
        void on_actionRefreshDevList_triggered();
        void on_actionSettingsDialog_triggered();
        void on_actionQuit_triggered();
        void on_actionSaveImage_triggered();

        void on_actIpAddrConfig_triggered();
        void on_actionSaveDataBlock_triggered();

        void retranslateUi();
        void createLanguageMenu();
        void onLanguageChanged(QAction*act);
        void setLanguage(const QString& name);
        void switchTranslator(QTranslator &translator, QLocale &locale, const QString& name, const QString& dir);
        void on_actionBrowseNetworkDevices_triggered();

	private:
        bool m_running;

        unsigned runningDevs();
        void streamStop();

        LQMeas::LogPanel *m_logPanel;

        QTranslator m_translator;
        QTranslator m_translatorLQMeas;
    #ifdef LBOOT_DIALOG_TS_BASENAME
        QTranslator m_translatorLboot;
    #endif
    #ifdef DEVIFACECONFIG_DIALOG_TS_BASENAME
        QTranslator m_translatorDevIfaceCfg;
    #endif
        QTranslator m_translatorQt;
        QString     m_curLangName;
    };
}

#endif // MAINWINDOW_H
