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
   
   MapSettings* settings = new MapSettings("google-maps.ini");
   
   MainWindow w(settings);
   w.show();
   
   return a.exec();
}
