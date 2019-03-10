#include <QApplication>
#include "MainWindow.h"
#include <QTextCodec>
#include <QSettings>


int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif

#ifdef Q_OS_WIN
    /* на Windows используем ini-файлы, а не реестр (как по умолчанию) */
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif


    /* Приложение ltrmanager должно быть запущено всегда одно.
     * уникальность определяем по сгенерированному GUID */
    QApplication a(argc, argv);

    QApplication::setOrganizationName("L Card");
    QApplication::setApplicationName(PRJ_APP_TITLE);
    QApplication::setOrganizationDomain("lcard.ru");
    QApplication::setApplicationVersion(PRJ_VERION_STR);



    LQMeasStudio::MainWindow w;

    w.show();

    return QApplication::exec();
}
