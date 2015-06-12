/* 
 * File:   LocationData.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 12, 2015, 5:08 PM
 */

#ifndef LOCATIONDATA_H
#define LOCATIONDATA_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QJsonArray>
#include <QGeoCoordinate>

struct GeometryData {
   QGeoCoordinate coordinates;   // Latitude and Longitude
   QString        locationType;  // Google location type
   QGeoCoordinate viewportNE;    // Northeast viewport
   QGeoCoordinate viewportSW;    // Southwest viewport
};

struct LocationData {
   QJsonArray     addressComponents; // Do not care about this currently, so leave it unprocessed but available
   QString        formattedAddress;
   QStringList    types;
   QString        placeID;
   GeometryData*  geometry;
};

typedef QList<LocationData*> LocationDataList;

#endif	/* LOCATIONDATA_H */

