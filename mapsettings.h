/* 
 * File:   mapsettings.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 12, 2015, 8:21 PM
 */

#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QString>

class MapSettings : public QObject {
   Q_OBJECT
   
public:
   MapSettings(QString _filename=QString(), QObject* _parent=0);
   MapSettings(const MapSettings& orig);
   virtual ~MapSettings();
   
   // If this is true, maps can be enabled in the application.
   // If false, there was probably a missing required setting.
   bool canEnableMaps()  const {
      return m_apiKeyValid && m_mapHtmlValid;
   }
   
   QString     configDir()    const { return m_configDir;      }
   QString     appRootDir()   const { return m_appRootDir;     }
   QString     userHomeDir()  const { return m_userHomeDir;    }
   QString     settingsFile() const { return m_settingsFile;   }
   
   QString     apiKey()       const { return m_apiKey;         }
   bool        isValidAPIKey()const { return m_apiKeyValid;   }
   
   double      lat()          const { return m_lat;            }
   double      lon()          const { return m_lon;            }
   int         zoom()         const { return m_zoom;           }
   
   QString     mapType()      const { return m_mapType;        }
   QString     mapHtmlPath()  const { return m_mapHtmlPath;    }
   
   QString     extraJSData()  const { return m_extraJSData;    }
   
   
public slots:
   void loadSettingsFile(QString _filename);
   void saveSettingsFile(QString _filename);

private:
   QSettings*  settings;
   QString     m_configDir;
   QString     m_appRootDir;
   QString     m_userHomeDir;
   QString     m_settingsFile;
   
   // Settings values
   QString     m_apiKey;      // Google Maps API Key
   bool        m_apiKeyValid; // Google Maps API Key was provided and valid
   
   double      m_lat;         // map-center latitude
   double      m_lon;         // map-center longitude
   int         m_zoom;        // zoom level
   
   QString     m_mapType;     // map type
   QString     m_mapHtmlInPath;//map input HTML file
   QString     m_mapHtmlPath; // map generated HTML file
   QString     m_mapHtmlData; // map generated HTML file contents
   bool        m_mapHtmlValid;// true if the file was provided
   
   QString     m_extraJSPath; // extra JavaScript file for map overlays, etc.
   QString     m_extraJSData; // extra JS file contents
   bool        m_extraJSAvailable; // extra JS file is non-empty and can be used
   
   
   // Private functions
   void loadExtraJS();
   void loadMapHtml();
};

#endif	/* MAPSETTINGS_H */

