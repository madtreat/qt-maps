#include "form.h"
#include "ui_form.h"
#include "mapsettings.h"

#include <QDebug>
#include <QWebFrame>
#include <QWebElement>
#include <QMessageBox>
#include <QFile>
#include <QDir>
// TODO: replace all exit(1) calls with something else so the app can continue without maps.


Form::Form(MapSettings* _settings, QWidget *parent) :
   QWidget(parent),
   ui(new Ui::Form),
   settings(_settings),
   m_geocodeDataManager(settings->apiKey(), this)
{
   ui->setupUi(this);
   ui->zoomSpinBox->setValue(settings->zoom());
   connect(ui->goButton, SIGNAL(clicked()), this, SLOT(goClicked()));
   connect(ui->lePostalAddress, SIGNAL(returnPressed()), this, SLOT(goClicked()));
   
   connect(&m_geocodeDataManager, SIGNAL(coordinatesReady(double,double)), this, SLOT(showCoordinates(double,double)));
   connect(&m_geocodeDataManager, SIGNAL(errorOccured(QString)), this, SLOT(errorOccured(QString)));
   
   QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
   ui->lePostalAddress->setText("");
   
   ui->webView->setUrl(QUrl::fromLocalFile(settings->mapHtmlPath()));
}

Form::~Form()
{
   delete ui;
}


void Form::showCoordinates(double lat, double lon, bool saveMarker)
{
   qDebug() << "Form, showCoordinates" << lat << "," << lon;
   
   QString str =
           QString("var newLoc = new google.maps.LatLng(%1, %2); ").arg(lat).arg(lon) +
           QString("map.setCenter(newLoc);") +
           QString("map.setZoom(%1);").arg(ui->zoomSpinBox->value());
   
   qDebug() << str;
   
   ui->webView->page()->currentFrame()->documentElement().evaluateJavaScript(str);
   
   if (saveMarker)
      setMarker(lat, lon, ui->lePostalAddress->text());
}

void Form::setMarker(double lat, double lon, QString caption)
{
   for (int i=0; i<m_markers.size(); i++)
   {
      if (m_markers[i]->caption == caption) return;
   }
   
   QString str =
           QString("var marker = new google.maps.Marker({") +
           QString("position: new google.maps.LatLng(%1, %2),").arg(lat).arg(lon) +
           QString("map: map,") +
           QString("title: %1").arg("\""+caption+"\"") +
           QString("});") +
           QString("markers.push(marker);");
   qDebug() << str;
   ui->webView->page()->currentFrame()->documentElement().evaluateJavaScript(str);
   
   
   SMarker *_marker = new SMarker(lat, lon, caption);
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
           QString("var newLoc = new google.maps.LatLng(%1, %2); ").arg(m_markers[currentRow]->lon).arg(m_markers[currentRow]->lat) +
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
    QString str =
            QString("map.setZoom(%1);").arg(arg1);
//    qDebug() << str;
    ui->webView->page()->currentFrame()->documentElement().evaluateJavaScript(str);
}
