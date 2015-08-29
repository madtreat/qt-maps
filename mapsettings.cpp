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
:  QObject(_parent),
   settings(NULL)
{
   m_userHomeDir = QDir::home().absolutePath();
   
   // Get config file from [application root directory]/config
   m_appRootDir = QCoreApplication::applicationDirPath();
   QDir appRoot(m_appRootDir);
   
   appRoot.cdUp();
   m_configDir = appRoot.absolutePath() + "/config";
   
   // If _filename starts with a "/", assume it is a full path
   if (_filename.startsWith("/")) {
      m_settingsFile = _filename;
      m_configDir = QFileInfo(m_settingsFile).absolutePath();
   }
   // Otherwise, add it to the default m_configDir
   else {
      m_settingsFile = m_configDir + "/" + _filename;
   }
   
   if (!QFile::exists(m_settingsFile)) {
      qWarning() << "Warning: Map settings file" << m_settingsFile << "does not exist. Exiting.";
      exit(1);
   }
   qDebug() << "Loading settings file:\n  " << m_settingsFile;
   
   loadSettingsFile(m_settingsFile);
   qDebug() << "   Done loading maps settings.";
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
   // file, since this can potentially be called multiple times in an application.
   if (settings) {
      qDebug() << "Settings object already exists, re-creating it" << settings;
      delete settings;
   }
   settings = new QSettings(_filename, QSettings::IniFormat);
   
   // Load the Map Provider
   m_mapProvider = settings->value("map_provider").toString();
   
   // Load the API_KEY
   m_apiKey = settings->value("api_key", "failed").toString();
   if ( m_mapProvider == "google" && ( m_apiKey == "failed" || m_apiKey.isEmpty() ) ) {
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
   
   // Load UI disable
   m_mapDisableUI = settings->value("disable_map_ui", false).toBool();
   
   // Load map JS file
   m_mapJSPath = settings->value("js_file").toString();
   bool validJS = loadMapJS();
   if (validJS) {
      // Load map HTML file from compiled in resource
      if (m_mapProvider == "google") {
         m_mapHtmlInPath = ":/html/config/google-maps.html";
      }
      else {
         m_mapHtmlInPath = ":/html/config/openlayers.html";
      }
      
       // must be called AFTER loadExtraJS() for valid JS data to be loaded
      bool validHtml = loadMapHtml();
      if (validHtml) {
         m_mapValid = true;
      }
      else {
         m_mapValid = false;
      }
   }
   else {
      m_mapValid = false;
   }

   // Load Map Orientation
   QString os = settings->value("map_orientation").toString();
   if (os == "north_up") {
      m_mapOrientation = NORTH_UP;
   }
   else if (os == "track_up") {
      m_mapOrientation = TRACK_UP;
   }
   else {
      qWarning() << "Warning: invalid map orientation, defaulting to NORTH_UP";
      m_mapOrientation = NORTH_UP;
   }
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
 * Returns valid JS: true or false
 */
bool MapSettings::loadMapJS()
{
   // Load the settings
   if (m_mapJSPath == "") {
      return false;
   }
   qDebug() << "   Map JS File:";
   
   // TODO: ensure properly works with "/path" and "~/path" and "path"
   //       read example settings.ini file
   if (!m_mapJSPath.startsWith("/") && !m_mapJSPath.startsWith("~")) {
      m_mapJSPath.prepend(m_configDir + "/");
   }
   qDebug() << "     " << m_mapJSPath;
   
   if (QFile::exists(m_mapJSPath)) {
      QFile file(m_mapJSPath);
      if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
         qWarning() << "Warning: Unable to open Map JS file, disabling maps.";
         return false;
      }
      else {
         QTextStream in(&file);
         m_mapJSData = in.readAll();
         return true;
      }
   }
   else {
      qWarning() << "Warning: No Map JS file provided, disabling maps.";
      return false;
   }
}

/*
 * Returns valid HTML: true or false
 */
bool MapSettings::loadMapHtml()
{
   // Load the settings
   if (m_mapJSData == "") {
      qWarning() << "Warning: Map JS file was empty or invalid, disabling maps.";
      return false;
   }
   
//   if (QFile::exists(m_mapHtmlInPath)) {
      QFile file(m_mapHtmlInPath);
      if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
         qWarning() << "Warning: Unable to open Map HTML file, disabling maps.";
         return false;
      }
      else {
         QTextStream in(&file);
         QString inText = in.readAll();
         
         m_mapHtmlPath = m_configDir + "/map-gen.html";
         QFile htmlFile(m_mapHtmlPath);
         if (!htmlFile.open(QFile::WriteOnly | QFile::Text)) {
            qWarning() << "Warning: Could not open Generated Map HTML file for writing";
            return false;
         }
         else {
            // Replace all the tags with their actual values
            // __API_KEY__ and __JS_CODE__ are directly in the HTML file,
            // The other tags are in the mapJSData string.
            m_mapHtmlData = inText.replace("__API_KEY__", m_apiKey);
            m_mapHtmlData = inText.replace("__JS_CODE__", m_mapJSData);
            m_mapHtmlData = inText.replace("__LAT__", QString::number(m_lat));
            m_mapHtmlData = inText.replace("__LON__", QString::number(m_lon));
            m_mapHtmlData = inText.replace("__ZOOM__", QString::number(m_zoom));
            m_mapHtmlData = inText.replace("__MAP_TYPE__", m_mapType);
            m_mapHtmlData = inText.replace("__DISABLE_MAP_UI__", m_mapDisableUI ? "true" : "false");
         
            QTextStream out(&htmlFile);
            out << m_mapHtmlData;
            
            return true;
         }
      }
//   }
//   else {
//      qWarning() << "Warning: No Map HTML file provided, disabling maps.";
//      return false;
//   }
}