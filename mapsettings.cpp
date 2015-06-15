/* 
 * File:   mapsettings.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 * 
 * Created on June 12, 2015, 8:21 PM
 */

#include "mapsettings.h"

#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

MapSettings::MapSettings(QString _filename, QObject* _parent)
:  QObject(_parent)
{
   m_userHomeDir = QDir::home().absolutePath();
   
   // Get config file from [application root directory]/config
   m_appRootDir = QCoreApplication::applicationDirPath();
   QDir appRoot(m_appRootDir);
   
   appRoot.cdUp();
   m_configDir = appRoot.absolutePath() + "/config";
   
   if (m_settingsFile == "") {
      m_settingsFile = m_configDir + "/google-maps.ini";
   }
   
   if (!QFile::exists(m_settingsFile)) {
      qWarning() << "Warning: Settings file" << m_settingsFile << "does not exist. Exiting.";
      exit(1);
   }
   qDebug() << "Loaded settings file:\n" << m_settingsFile;
   
   loadSettingsFile(m_settingsFile);
}

MapSettings::MapSettings(const MapSettings& orig)
{
}

MapSettings::~MapSettings()
{
}


/*
 * Load the settings in file _filename.
 */
void MapSettings::loadSettingsFile(QString _filename)
{
   // If the settings pointer already exists, delete it to start fresh on a new
   // file, since this can be called multiple times in an application.
   if (settings) {
      delete settings;
   }
   settings = new QSettings(_filename, QSettings::IniFormat);
   
   // Load the API_KEY
   m_apiKey = settings->value("api_key", "failed").toString();
   if ( m_apiKey == "failed" || m_apiKey.isEmpty() ) {
      qWarning() << "Warning: No Google Maps API Key provided. Exiting.";
      m_apiKeyValid = false;
   }
   else {
      // TODO: test API Key to ensure valid
      m_apiKeyValid = true;
   }
   
   // Load the starting coordinates
   m_lat = settings->value("lat", "0.0").toDouble();
   m_lon = settings->value("lon", "0.0").toDouble();
   
   // Load zoom level
   m_zoom = settings->value("zoom", "12").toInt();
   
   // Load map type
   m_mapType = settings->value("map_type", "ROADMAP").toString();
   
   // Load extra JS file
   m_extraJSPath = settings->value("extra_js_file").toString();
   loadExtraJS();
   
   // Load map HTML file from compiled in resource
   m_mapHtmlInPath = ":/html/config/google-maps.html";
   loadMapHtml(); // must be called AFTER loadExtraJS() for valid JS data to be loaded
}

/*
 * Create a new settings file with the name _filename.
 */
void MapSettings::saveSettingsFile(QString _filename)
{
   // TODO: make this work
   
   settings->sync();
}

/*
 * 
 */
void MapSettings::loadExtraJS()
{
   // Load the settings
   // TODO: ensure properly works with "/path" and "~/path" and "path"
   //       read example settings.ini file
   if (QFile::exists(m_extraJSPath)) {
      QFile file(m_extraJSPath);
      if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
         qWarning() << "Warning: Unable to open Extra JS file, skipping.";
         m_extraJSAvailable = false;
      }
      else {
         QTextStream in(&file);
         m_extraJSData = in.readAll();
         m_extraJSAvailable = true;
      }
   }
   else {
      qWarning() << "Warning: No Extra JS file provided, skipping.";
      m_extraJSAvailable = false;
   }
}

/*
 * 
 */
void MapSettings::loadMapHtml()
{
   // Load the settings
//   if (QFile::exists(m_mapHtmlInPath)) {
      QFile file(m_mapHtmlInPath);
      if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
         qWarning() << "Warning: Unable to open Map HTML file, disabling maps.";
         m_mapHtmlValid = false;
      }
      else {
         QTextStream in(&file);
         QString inText = in.readAll();
         
         m_mapHtmlPath = m_configDir + "/google-maps-gen.html";
         QFile htmlFile(m_mapHtmlPath);
         if (!htmlFile.open(QFile::WriteOnly | QFile::Text)) {
            qWarning() << "Warning: Could not open Generated Map HTML file for writing";
            m_mapHtmlValid = false;
         }
         else {
            // Replace all the tags with their actual values
            m_mapHtmlData = inText.replace("__API_KEY__", m_apiKey);
            m_mapHtmlData = inText.replace("__LAT__", QString::number(m_lat));
            m_mapHtmlData = inText.replace("__LON__", QString::number(m_lon));
            m_mapHtmlData = inText.replace("__ZOOM__", QString::number(m_zoom));
            m_mapHtmlData = inText.replace("__MAP_TYPE__", m_mapType);
            m_mapHtmlData = inText.replace("__EXTRA_JS__", m_extraJSData);
         
            QTextStream out(&htmlFile);
            out << m_mapHtmlData;
            
            m_mapHtmlValid = true;
         }
      }
//   }
//   else {
//      qWarning() << "Warning: No Map HTML file provided, disabling maps.";
//      m_mapHtmlValid = false;
//   }
}