#include "form.h"
#include "ui_form.h"
#include <QDebug>
#include <QWebFrame>
#include <QWebElement>
#include <QMessageBox>
#include <QFile>
#include <QDir>
// TODO: replace all exit(1) calls with something else so the app can continue without maps.


Form::Form(QString _apiKey, QWidget *parent) :
   QWidget(parent),
   ui(new Ui::Form),
   apiKey(_apiKey),
   m_geocodeDataManager(apiKey, this)
{
   ui->setupUi(this);
   connect(ui->goButton, SIGNAL(clicked()), this, SLOT(goClicked()));
   connect(ui->lePostalAddress, SIGNAL(returnPressed()), this, SLOT(goClicked()));
   
   connect(&m_geocodeDataManager, SIGNAL(coordinatesReady(double,double)), this, SLOT(showCoordinates(double,double)));
   connect(&m_geocodeDataManager, SIGNAL(errorOccured(QString)), this, SLOT(errorOccured(QString)));
   
   QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
   ui->lePostalAddress->setText("");
   
   // Edit the Google Maps API Key in the resource file to be valid.
   QString htmlModPath = getHTMLWithAPIKey(":/html/google_maps.html");
   ui->webView->setUrl(QUrl::fromLocalFile(htmlModPath));
}

Form::~Form()
{
   delete ui;
}


/*
 * Returns the path to a modified HTML file containing the Google Maps API Key 
 * instead of a placeholder.  If the modified file exists already in the config
 * directory, it is not regenerated.
 */
QString Form::getHTMLWithAPIKey(QString htmlFile)
{
   // Get config file from [application root directory]/config
   QString appRootDirStr = QCoreApplication::applicationDirPath();
   QDir appRootDir(appRootDirStr);
   appRootDir.cdUp();
   QString configDir = appRootDir.absolutePath() + "/config";
   
   // Check to see if the modified HTML file has been previously generated
   // and use it.
   QString modHTMLPath = configDir + "/google-maps-with-api.html";
   if (QFile::exists(modHTMLPath))
   {
      qDebug() << "Using previously generated HTML file with API Key at";
      qDebug() << "  " << modHTMLPath;
      return modHTMLPath;
   }
   
   // If the modified HTML file has not been generated, create it now.
   // Replace the filler string in the HTML and return the path
   QFile origHTML(htmlFile);
   if (!origHTML.open(QFile::ReadOnly | QFile::Text))
   {
      qDebug() << "Could not open original HTML file for reading";
      exit(1);
   }
   
   QFile modHTML(modHTMLPath);
   if (!modHTML.open(QFile::WriteOnly | QFile::Text))
   {
      qDebug() << "Could not open modified HTML file for writing";
      exit(1);
   }
   
   QTextStream in(&origHTML);
   QString origText = in.readAll();
   QString modText = origText.replace("YOUR_API_KEY_HERE", apiKey);
   
   QTextStream out(&modHTML);
   out << modText;
   
   return modHTMLPath;
}

void Form::showCoordinates(double east, double north, bool saveMarker)
{
   qDebug() << "Form, showCoordinates" << east << north;
   
   QString str =
           QString("var newLoc = new google.maps.LatLng(%1, %2); ").arg(north).arg(east) +
           QString("map.setCenter(newLoc);") +
           QString("map.setZoom(%1);").arg(ui->zoomSpinBox->value());
   
   qDebug() << str;
   
   ui->webView->page()->currentFrame()->documentElement().evaluateJavaScript(str);
   
   if (saveMarker)
      setMarker(east, north, ui->lePostalAddress->text());
}

void Form::setMarker(double east, double north, QString caption)
{
   for (int i=0; i<m_markers.size(); i++)
   {
      if (m_markers[i]->caption == caption) return;
   }
   
   QString str =
           QString("var marker = new google.maps.Marker({") +
           QString("position: new google.maps.LatLng(%1, %2),").arg(north).arg(east) +
           QString("map: map,") +
           QString("title: %1").arg("\""+caption+"\"") +
           QString("});") +
           QString("markers.push(marker);");
   qDebug() << str;
   ui->webView->page()->currentFrame()->documentElement().evaluateJavaScript(str);
   
   
   SMarker *_marker = new SMarker(east, north, caption);
   m_markers.append(_marker);
   
   //adding capton to ListWidget
   ui->lwMarkers->addItem(caption);
}

void Form::goClicked()
{
   QString address = ui->lePostalAddress->text();
   m_geocodeDataManager.getCoordinates(address.replace(" ", "+"));
}



void Form::errorOccured(const QString& error)
{
   QMessageBox::warning(this, tr("Geocode Error"), error);
}



void Form::on_lwMarkers_currentRowChanged(int currentRow)
{
   if (currentRow < 0) return;
   QString str =
           QString("var newLoc = new google.maps.LatLng(%1, %2); ").arg(m_markers[currentRow]->north).arg(m_markers[currentRow]->east) +
           QString("map.setCenter(newLoc);");
   
   qDebug() << str;
   
   ui->webView->page()->currentFrame()->documentElement().evaluateJavaScript(str);
}

void Form::on_pbRemoveMarker_clicked()
{
   if (ui->lwMarkers->currentRow() < 0) return;
   
   QString str = QString("markers[%1].setMap(null); markers.splice(%1, 1);").arg(ui->lwMarkers->currentRow());
   qDebug() << str;
   ui->webView->page()->currentFrame()->documentElement().evaluateJavaScript(str);
   
   //deleteing caption from markers list
   delete m_markers.takeAt(ui->lwMarkers->currentRow());
   //deleteing caption from ListWidget
   delete ui->lwMarkers->takeItem(ui->lwMarkers->currentRow());
   
}

void Form::on_zoomSpinBox_valueChanged(int arg1)
{
//    QString str =
//            QString("map.setZoom(%1);").arg(arg1);
//    qDebug() << str;
//    ui->webView->page()->currentFrame()->documentElement().evaluateJavaScript(str);
}
