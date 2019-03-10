#include "MainWindow.h"
#include "ui_MainWindow.h"


#include <QCoreApplication>
#include <QMap>
#include <QBrush>
#include <QSettings>
#include <QTreeWidget>
#include <QAction>
#include <QToolButton>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QLibraryInfo>
#include <QElapsedTimer>

#include "SettingsDialog.h"

#include "lqmeas/log/Log.h"
#include "lqmeas/log/LogPanel.h"

#include "ChannelSignalParamPanel.h"

#include "lqmeas/vptr.h"
#include "devs/IpAddrConfig/IpAddrConfigDialog.h"
#include "devs/NetworkDevicesBrowser/NetworkBrowserModel.h"
#include "devs/NetworkDevicesBrowser/NetworkBrowserDialog.h"
#include "proc/AdcBlockReceivePlugin.h"
#include "proc/ACDCEstimatePlugin.h"
#include "plot/BlockTimeGraphPlugin.h"
#include "LQMeasStudio.h"




namespace LQMeasStudio {
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow), m_running(false) {
        ui->setupUi(this);

        setWindowTitle(QCoreApplication::applicationName() + " - " + QCoreApplication::applicationVersion());

        createLanguageMenu();

        m_logPanel = new LQMeas::LogPanel(this);
        addDockWidget(Qt::BottomDockWidgetArea, m_logPanel);
        m_logPanel->hide();

        /* загрузка настроек состояния интерфейса */
        ui->refreshDevButton->setDefaultAction(ui->actionRefreshDevList);

        LQMeasStudioEnv->init(new DeviceTree(ui->devicesTree, this), ui->centralWidgetLout);

        LQMeasStudioEnv->deviceTree()->addAction(LQDevPlugin::ActionTypeDevConfig, ui->actionDevCfg);
        LQMeasStudioEnv->deviceTree()->addAction(LQDevPlugin::ActionTypeDevIfaceConfig, ui->actionDevIfaceSettings);
        LQMeasStudioEnv->deviceTree()->addAction(LQDevPlugin::ActionTypeDevUpdateFirmware, ui->actionDevUpdateFirmware);
        LQMeasStudioEnv->deviceTree()->addAction(LQDevPlugin::ActionTypeAsyncIO, ui->actionAsyncIO);

        LQMeasStudioEnv->addPlugin(new AdcBlockReceivePlugin(this));
        LQMeasStudioEnv->addPlugin(new ACDCEstimatePlugin(this));
        LQMeasStudioEnv->addPlugin(new BlockTimeGraphPlugin(this));

        this->addDockWidget(Qt::BottomDockWidgetArea, LQMeasStudioEnv->chSignalParamsPanel());

		/* Восстановление общего положения окон */
        QSettings set;
        set.beginGroup("MainWindow");
        restoreState(set.value("state").toByteArray());
        resize(set.value("size", QSize(800, 600)).toSize());
        move(set.value("pos", QPoint(0, 0)).toPoint());
        set.endGroup();

        /* настройка действий по скрытию/показу паналей */
        connect(ui->actionShowDevsPanel, SIGNAL(triggered(bool)), ui->devicePanel,
                SLOT(setVisible(bool)));
        connect(ui->devicePanel, SIGNAL(visibilityChanged(bool)), ui->actionShowDevsPanel,
                SLOT(setChecked(bool)));

        connect(ui->actionShowSigParamsPanel, SIGNAL(triggered(bool)), LQMeasStudioEnv->chSignalParamsPanel(),
                SLOT(setVisible(bool)));
        connect(LQMeasStudioEnv->chSignalParamsPanel(), SIGNAL(visibilityChanged(bool)), ui->actionShowSigParamsPanel,
                SLOT(setChecked(bool)));

        connect(LQMeasStudioEnv->outGenerator(), SIGNAL(deviceError(QSharedPointer<LQMeas::Device>,LQMeas::Error,QString,QString)),
                SLOT(showDeviceError(QSharedPointer<LQMeas::Device>,LQMeas::Error,QString,QString)));
        connect(LQMeasStudioEnv->outGenerator(), SIGNAL(deviceGenUnderflow(QSharedPointer<LQMeas::Device>)),
                SLOT(indicateOutGenUnderflow(QSharedPointer<LQMeas::Device>)));

        connect(LQMeasStudioEnv->inLauncher(), SIGNAL(deviceError(QSharedPointer<LQMeas::Device>,LQMeas::Error,QString,QString)),
                SLOT(showDeviceError(QSharedPointer<LQMeas::Device>,LQMeas::Error,QString,QString)));

        connect(LQMeasStudioEnv->netBrowser(), SIGNAL(error(LQMeas::Error)), SLOT(onBrowseError(LQMeas::Error)));

        connect(ui->actionShowLogPanel, SIGNAL(triggered(bool)), m_logPanel, SLOT(setVisible(bool)));
        connect(m_logPanel, SIGNAL(visibilityChanged(bool)), ui->actionShowLogPanel,
                SLOT(setChecked(bool)));

        LQMeasStudioEnv->loadSettings(set);

        setLanguage(set.value("lang").toString());
        retranslateUi();

		ui->actionDraw_Plot->setChecked(set.value("DrawPlot", true).toBool());

        LQMeasLog->info(tr("Application started"));

        /* Если есть сохраненные автоподключаемые устройства по сети, то ждем
           их обнаружения */
        /** @todo подумать о том, как сделать это вместе с открытием остальных устройств */
        if (LQMeasStudioEnv->netBrowser()->browseIsEnabled()) {
            QElapsedTimer timer;
            timer.start();

            while (!LQMeasStudioEnv->netBrowser()->allEnabledDevsPresent()
                   && !timer.hasExpired(2000)) {
                QApplication::processEvents(QEventLoop::WaitForMoreEvents, 100);
            }

            LQMeasStudioEnv->netBrowser()->removeNotPresentDevs();
        }

        LQMeasStudioEnv->deviceTree()->refreshDeviceList();
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

    void MainWindow::saveSettings() {
        QSettings set;

        set.beginGroup("MainWindow");
        /* если коно в настоящий момент не свернуто - то сохраняем его текущие
           параметры */
        if (this->isVisible() && !isMinimized()) {
            set.setValue("state", saveState());
            set.setValue("size", size());
            set.setValue("pos", pos());
        }
        set.endGroup();
        set.setValue("lang", m_curLangName);
		set.setValue("DrawPlot", ui->actionDraw_Plot->isChecked());
        LQMeasStudioEnv->saveSettings(set);
        LQMeasStudioEnv->deviceTree()->saveDevicesState(set);
    }

    void MainWindow::showDeviceError(const QSharedPointer<LQMeas::Device>& dev, const LQMeas::Error& err, const QString& caption, const QString& msg) {
        QMessageBox::critical(this, caption, dev->devStr() + ": " + msg +
							  QString(" (%1): %2").arg(QString::number(err.errorCode()), err.msg()));
        if (runningDevs()==0) {
            streamStop();
        }
    }

    void MainWindow::onBrowseError(const LQMeas::Error& err) {
        QMessageBox::critical(this, tr("Device discovery error"), tr("Error occured during device discovery") +
							  QString(" (%1): %2").arg(QString::number(err.errorCode()), err.msg()));
    }

    void MainWindow::indicateOutGenUnderflow(const QSharedPointer<LQMeas::Device>& dev) {
        LQMeasLog->warning(tr("Output generation buffer underflow occured"), dev.data());
    }

    void MainWindow::closeEvent(QCloseEvent *event) {
		Q_UNUSED(event);
        LQMeasStudioEnv->netBrowser()->browseStop();
        saveSettings();
        if (m_running) {
            streamStop();
            clearStreamResources();
        }

        foreach (QSharedPointer<LQMeas::Device> dev, LQMeasStudioEnv->deviceTree()->devlist()) {
            dev->close();
        }
    }

    void MainWindow::changeEvent(QEvent *event) {
        if (event->type() == QEvent::LanguageChange)
            retranslateUi();
        QMainWindow::changeEvent(event);
    }

    void MainWindow::on_actionStart_triggered() {
        if (!m_running) {
            ui->actionStart->setEnabled(false);
            ui->actionRefreshDevList->setEnabled(false);

            clearStreamResources();

            QList<QSharedPointer<LQMeas::Device> > devlist = LQMeasStudioEnv->deviceTree()->selectedList();
            foreach (QSharedPointer<LQMeas::Device> dev, devlist) {
                dev->configure();
            }

            foreach (ProcessPlugin *plugin, LQMeasStudioEnv->processPlugings()) {
                plugin->procStartPrepare();
            }

            foreach (ProcessPlugin *plugin, LQMeasStudioEnv->processPlugings()) {
                plugin->procStart();
            }

            m_running = true;
            ui->actionStop->setEnabled(true);

            if (runningDevs() == 0) {
                QMessageBox::critical(this, tr("Start error"), tr("No device has been started"));
                streamStop();
            }
        }
    }

    void MainWindow::on_actionStop_triggered() {
        streamStop();
    }

    void MainWindow::streamStop() {
        if (m_running) {
            ui->actionStop->setEnabled(false);

            foreach (ProcessPlugin *plugin, LQMeasStudioEnv->processPlugings()) {
                plugin->procStopRequest();
            }

            foreach (ProcessPlugin *plugin, LQMeasStudioEnv->processPlugings()) {
                plugin->procStop();
            }

            m_running = false;

            ui->actionStart->setEnabled(true);
            ui->actionRefreshDevList->setEnabled(true);
        }
    }

    void MainWindow::clearStreamResources() {
        foreach (ProcessPlugin *plugin, LQMeasStudioEnv->processPlugings()) {
            plugin->procClear();
        }
    }

    void MainWindow::on_actionRefreshDevList_triggered() {
        LQMeasStudioEnv->deviceTree()->refreshDeviceList();
    }

    void MainWindow::on_actionSettingsDialog_triggered() {
        SettingsDialog dialog(this);
        dialog.exec();
    }

    void MainWindow::on_actionQuit_triggered() {
        close();
    }

    void MainWindow::on_actionSaveImage_triggered() {
        QString filename = QFileDialog::getSaveFileName(this, tr("Saving screen image"),
														qApp->applicationDirPath(), "*.png");
        if (!filename.isEmpty()) {
            QPixmap pixmap(size());
            render(&pixmap);
            //m_ctlDialog->render(&pixmap, m_ctlDialogOffset);
			if (!pixmap.save(filename, nullptr, 100)) {
                QMessageBox::critical(this, tr("Error"), tr("Cannot save screen image to file!"));
            } else {
                QMessageBox::information(this, tr("Saving screen image"), tr("Screen image saved successfully"));
            }
        }
    }

    unsigned MainWindow::runningDevs() {
        return LQMeasStudioEnv->inLauncher()->runningDevs() +
                LQMeasStudioEnv->outGenerator()->runningDevs();
    }

    void MainWindow::on_actIpAddrConfig_triggered() {
        IpAddrConfigDialog dialog(LQMeasStudioEnv->ipConfig(), this);
		if (dialog.exec() == QDialog::Accepted) {
            LQMeasStudioEnv->deviceTree()->refreshDeviceList();
        }
    }

    void MainWindow::on_actionBrowseNetworkDevices_triggered() {
        NetworkBrowserDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            LQMeasStudioEnv->deviceTree()->refreshDeviceList();
        }
    }

    void MainWindow::on_actionSaveDataBlock_triggered() {
		QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
														qApp->applicationDirPath(),
														QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if (!dir.isEmpty()) {
			LQMeasStudioEnv->savingDir = dir;
		}
    }

    void MainWindow::createLanguageMenu() {
        auto *langGroup = new QActionGroup(ui->menuLanguage);
        connect(langGroup, SIGNAL(triggered(QAction*)), this,
                SLOT(onLanguageChanged(QAction*)));


        ui->actionLangSystem->setData("");
        langGroup->addAction(ui->actionLangSystem);

        ui->actionLangRussian->setData("ru");
        langGroup->addAction(ui->actionLangRussian);

        ui->actionLangEnglish->setData("en");
        langGroup->addAction(ui->actionLangEnglish);
    }

    void MainWindow::onLanguageChanged(QAction *act) {
        QString locName = act->data().toString();
        if (locName!=m_curLangName)
            setLanguage(locName);
    }

    void MainWindow::setLanguage(const QString& name) {
        m_curLangName = name;
        QLocale loc;
        if (name.isEmpty()) {
            loc = QLocale::system();
        } else {
            loc = QLocale(name);
        }

        foreach (QAction *act, ui->menuLanguage->actions()) {
            if (act->data().toString()==name) {
                act->setChecked(true);
            }
        }

		switchTranslator(m_translator, loc, LQMEAS_TS_BASENAME, PRJ_TRANSLATIONS_DIR);
        switchTranslator(m_translatorLQMeas, loc, LQMEAS_TS_BASENAME, PRJ_TRANSLATIONS_DIR);
    #ifdef LBOOT_DIALOG_TS_BASENAME
        switchTranslator(m_translatorLboot, loc, LBOOT_DIALOG_TS_BASENAME, PRJ_TRANSLATIONS_DIR);
    #endif
    #ifdef DEVIFACECONFIG_DIALOG_TS_BASENAME
        switchTranslator(m_translatorDevIfaceCfg, loc, DEVIFACECONFIG_DIALOG_TS_BASENAME, PRJ_TRANSLATIONS_DIR);
    #endif
        switchTranslator(m_translatorQt, loc, "qt",
                     #ifdef _WIN32
                         PRJ_TRANSLATIONS_DIR
                     #else
                         QLibraryInfo::location(QLibraryInfo::TranslationsPath)
                     #endif
                         );
    }

    void MainWindow::switchTranslator(QTranslator &translator, QLocale &locale,
                                      const QString& name, const QString& dir) {
        qApp->removeTranslator(&translator);

    #if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
        translator.load(locale, name, "_", dir);
    #else
        translator.load(name + "_" + locale.name(), dir);
    #endif
        qApp->installTranslator(&translator);
    }

    void MainWindow::retranslateUi() {
        ui->retranslateUi(this);
        LQMeasStudioEnv->chSignalParamsPanel()->retranslate();
        LQMeasLog->retranslate();

        foreach (ProcessPlugin *plugin, LQMeasStudioEnv->processPlugings()) {
            plugin->retranslate();
        }
    }
}

void LQMeasStudio::MainWindow::on_actionDraw_Plot_toggled(bool arg1)
{
	foreach (ProcessPlugin *plugin, LQMeasStudioEnv->processPlugings()) {
		DataBlockProcessPlugin *blockPlugin = dynamic_cast<DataBlockProcessPlugin *>(plugin);
		if (blockPlugin && blockPlugin->typePluginName() == "BlockTimeGraphPlugin") {
			blockPlugin->procSetEnabled(arg1);
		}
	}
}
