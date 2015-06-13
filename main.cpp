#include <QtGlobal>
#if QT_VERSION >= 0x050000
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif
#include <QDir>
#include <QString>

#include "mainwindow.h"
#include "mapsettings.h"


int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   
   QString appRootDirStr = QCoreApplication::applicationDirPath();
   QDir appRootDir(appRootDirStr);
   appRootDir.cdUp();
   QString configDir = appRootDir.absolutePath() + "/config";
   QString settingsFilename = configDir + "/google-maps.ini";
   
   MapSettings* settings = new MapSettings(settingsFilename);
   
   MainWindow w(settings);
   w.show();
   
   return a.exec();
}
