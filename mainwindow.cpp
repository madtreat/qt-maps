#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"

#include <QSettings>
#include <QDir>
// TODO: replace all exit(1) calls with something else so the app can continue without maps.

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   
   connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
   
   apiKey = getAPIKey();
   m_pForm = new Form(apiKey, this);
   setCentralWidget(m_pForm);
}

MainWindow::~MainWindow()
{
   delete ui;
}


/*
 * Returns the Google Maps API Key contained in the INI file
 *    [project root]/config/google-maps.ini
 */
QString MainWindow::getAPIKey()
{
   // Get config file from [application root directory]/config
   QString appRootDirStr = QCoreApplication::applicationDirPath();
   QDir appRootDir(appRootDirStr);
   appRootDir.cdUp();
   QString configDir = appRootDir.absolutePath() + "/config";
   
   QString apiKeyFilename = configDir + "/google-maps.ini";
   QString apiKeyFile(apiKeyFilename);
   
   // Ensure the file exists or exit
   if (!QFile::exists(apiKeyFile))
   {
      qWarning() << "Warning: Settings file" << apiKeyFile << "does not exist. Exiting.";
      exit(1);
   }
   qDebug() << "Loaded settings file:\n  " << apiKeyFilename;
   
   // Create the settings object
   QSettings* gmapSettings = new QSettings(apiKeyFile, QSettings::IniFormat);
   gmapSettings->sync();
   
   // Load the API_KEY value
   QString key = gmapSettings->value("API_KEY", "failed").toString();
   if (key == "failed")
   {
      qWarning() << "Warning: No Google Maps API Key provided. Exiting.";
      exit(1);
   }
   
   return key;
}
