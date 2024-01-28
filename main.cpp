#include "mainwindow.h"

#include <QApplication>

#include <QApplication>
#include <QtCore/QTextCodec>
#include <QFile>
#include <QSplashScreen>
#include <QTranslator>
#include <QLibraryInfo>


#pragma execution_character_set("utf-8")
int main(int argc, char *argv[])
{
    if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    //***************** Установка шрифта ****************************//

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load("VIM_" + QLocale::system().name());
    a.installTranslator(&myappTranslator);

    //***************** END Установка шрифта ****************************//


    MainWindow w;
    w.show();
    return a.exec();
}
