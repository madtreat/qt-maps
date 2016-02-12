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

#include "qtmapsconsts.h"


class MapSettings : public QObject {
   Q_OBJECT
   
public:
   MapSettings(QString _filename, QObject* _parent=0);
   MapSettings(const MapSettings& orig);
   virtual ~MapSettings();
   
   // If this is true, maps can be enabled in the application.
   // If false, there was probably a missing required setting.
   bool canEnableMaps()  const {
      return m_apiKeyValid && m_mapValid;
   }
   
   QString     configDir()    const { return m_configDir;      }
   QString     appRootDir()   const { return m_appRootDir;     }
   QString     userHomeDir()  const { return m_userHomeDir;    }
   QString     settingsFile() const { return m_settingsFile;   }
   
   QString     apiKey()       const { return m_apiKey;         }
   bool        isValidAPIKey()const { return m_apiKeyValid;    }
   
   double      lat()          const { return m_lat;            }
   double      lon()          const { return m_lon;            }
   int         zoom()         const { return m_zoom;           }
   
   QString     mapType()      const { return m_mapType;        }
   QString     mapHtmlPath()  const { return m_mapHtmlPath;    }
   
   QString     mapJSData()    const { return m_mapJSData;      }
   bool        mapDisableUI() const { return m_mapDisableUI;   }

   MapOrientation mapOrientation() const { return m_mapOrientation; }
   void        setMapOrientation(MapOrientation mo) {
      m_mapOrientation = mo;
      emit mapOrientationChanged(mo);
   }
   
   
public slots:
   void loadSettingsFile(QString _filename);
   void saveSettingsFile(QString _filename);
   
   void setLat(double lat)       { m_lat = lat;       emit latChanged(m_lat);  }
   void setLon(double lon)       { m_lon = lon;       emit lonChanged(m_lon);  }
   void setZoom(int level)       { m_zoom = level;    emit zoomChanged(m_zoom);}
   void setMapType(QString type) { m_mapType = type;  emit mapTypeChanged(m_mapType);}

signals:
   void latChanged(double lat);
   void lonChanged(double lon);
   void zoomChanged(int zoom);
   void mapTypeChanged(QString type);
   void mapOrientationChanged(MapOrientation mo);
   
private:
   QSettings*  settings;
   QString     m_configDir;
   QString     m_appRootDir;
   QString     m_userHomeDir;
   QString     m_settingsFile;
   
   // Settings values
   QString     m_mapProvider; // Map provider
   QString     m_apiKey;      // Google Maps API Key
   bool        m_apiKeyValid; // Google Maps API Key was provided and valid
   
   double      m_lat;         // map-center latitude
   double      m_lon;         // map-center longitude
   int         m_zoom;        // zoom level
   
   QString     m_mapType;     // map type
   
   QString     m_mapHtmlInPath;//map input HTML file
   QString     m_mapHtmlPath; // map generated HTML file
   QString     m_mapHtmlData; // map generated HTML file contents
   
   QString     m_mapJSPath; // JavaScript file for map data and overlays
   QString     m_mapJSData; // JS file contents
   
   bool        m_mapDisableUI; // Disable Default Map UI
   
   bool        m_mapValid;// true if the HTML and JS files were provided

   // "map" group
   MapOrientation m_mapOrientation;
   
   
   // Private functions
   bool loadMapJS();
   bool loadMapHtml();
};

#endif	/* MAPSETTINGS_H */

