#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"
#include "mapsettings.h"

#include <QSettings>
#include <QDir>
// TODO: replace all exit(1) calls with something else so the app can continue without maps.

MainWindow::MainWindow(MapSettings* _settings, QWidget* parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow),
   settings(_settings)
{
   ui->setupUi(this);
   
   connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
   
   m_pForm = new Form(settings, this);
   setCentralWidget(m_pForm);
}

MainWindow::~MainWindow()
{
   delete ui;
}
