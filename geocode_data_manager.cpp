/*
 * See https://developers.google.com/maps/documentation/geocoding/#GeocodingResponses
 * for response format.
 */

#include "geocode_data_manager.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "locationdata.h"

GeocodeDataManager::GeocodeDataManager(QString _apiKey, QObject *parent) :
   QObject(parent),
   apiKey(_apiKey)
{
   m_pNetworkAccessManager = new QNetworkAccessManager(this);
   connect(m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processReply(QNetworkReply*)));
}


void GeocodeDataManager::getCoordinates(const QString& address)
{
   QString url = QString("https://maps.googleapis.com/maps/api/geocode/json?address=%1&key=%2").arg(address).arg(apiKey);
   m_pNetworkAccessManager->get(QNetworkRequest(QUrl(url)));
}


void GeocodeDataManager::processReply(QNetworkReply* reply)
{
   QByteArray json = reply->readAll();
   QString strUrl = reply->url().toString();
//   qDebug() << "Reply = " << jsonData;
//   qDebug() << "URL = " << reply->url();
   
   QJsonDocument replyDoc = QJsonDocument::fromJson(json);
   if (replyDoc.isNull()) {
      qDebug() << "Warning: Google replied with invalid data";
      return;
   }
   // It SHOULD be an object, but there is no harm in confirming
   if (!replyDoc.isObject()) {
      qDebug() << "Warning: Google replied with a non-object, which is incorrect";
      return;
   }
   
   // Check the status
   QJsonObject rootObj = replyDoc.object();
   QString statusStr = rootObj.value("status").toString();
   bool status = parseStatus(statusStr);
   if (!status) {
      return;
   }
   
   // Now check the results
   QJsonArray results = rootObj.value("results").toArray();
   LocationDataList resultLocs = parseResults(results);
   if (resultLocs.size() == 0) {
      qDebug() << "No results returned";
      return;
   }
   
   // For each resulting location, place a marker on the map
   foreach (LocationData* loc, resultLocs) {
      QString address = loc->formattedAddress;
      qDebug() << "Address:" << address;
      double lat = loc->geometry->coordinates.latitude();
      double lon = loc->geometry->coordinates.longitude();
      qDebug() << "Coords:" << lat << "," << lon;
      emit coordinatesReady(lat, lon);
   }
}


bool GeocodeDataManager::parseStatus(QString status)
{
   // Check the status
   qDebug() << "Reply status =" << status;
   
   if (status == "ZERO_RESULTS") {
      qDebug() << "No locations were found matching your query";
      // TODO: prob need to do something else here
      return false;
   } else if ( (status == "OVER_QUERY_LIMIT") ||
               (status == "REQUEST_DENIED") ||
               (status == "INVALID_REQUEST") ||
               (status == "UNKNOWN_ERROR")) {
      qDebug() << "There was an error with the query or the response";
      return false;
   }
   // else status === "OK"
   return true;
}


/*
 * Parse a QGeoCoordinate object from a location.
 */
QGeoCoordinate GeocodeDataManager::parseGeoCoordinate(QJsonObject location)
{
   double lat = location.value("lat").toDouble();
   double lon = location.value("lng").toDouble();
   return QGeoCoordinate(lat, lon);
}


/*
 * Parse a GeometryData object.
 */
GeometryData* GeocodeDataManager::parseGeometry(QJsonObject geo)
{
   GeometryData* geometry = new GeometryData();
   geometry->locationType = geo.value("location_type").toString();
   
   QJsonObject location = geo.value("location").toObject();
   geometry->coordinates = parseGeoCoordinate(location);
   
   QJsonObject viewport = geo.value("viewport").toObject();
   QJsonObject viewportNE = viewport.value("northeast").toObject();
   QJsonObject viewportSW = viewport.value("southwest").toObject();
   geometry->viewportNE = parseGeoCoordinate(viewportNE);
   geometry->viewportSW = parseGeoCoordinate(viewportSW);
   
   return geometry;
}


/*
 * Parse a single result and return a LocationData struct.
 */
LocationData* GeocodeDataManager::parseResult(QJsonObject result)
{
   LocationData* loc = new LocationData();
   
   // Ignore address components (but we would parse them here if we ever want
   // to use them)
   QJsonArray addressComponents = result.value("address_components").toArray();
   loc->addressComponents = addressComponents;
   
   // Parse formatted address
   QString formattedAddress = result.value("formatted_address").toString();
   loc->formattedAddress = formattedAddress;
   
   // Parse place id
   QString placeID = result.value("place_id").toString();
   loc->placeID = placeID;
   
   // Parse types
   QJsonArray typesArray = result.value("types").toArray();
   QStringList types;
   while (!typesArray.isEmpty()) {
      types.push_back(typesArray.takeAt(0).toString());
   }
   loc->types = types;
   
   // Parse geometry
   QJsonObject geoObj = result.value("geometry").toObject();
   GeometryData* geo = parseGeometry(geoObj);
   loc->geometry = geo;
   
   return loc;
}


/*
 * Parse all results (typically only 1 unless query was ambiguous) and return
 * a list of all the location data.
 */
LocationDataList GeocodeDataManager::parseResults(QJsonArray results)
{
//   qDebug() << "Results =" << results;
   LocationDataList locs;
   
   if (results.isEmpty()) {
      return locs;
   }
   
   while (!results.isEmpty()) {
      QJsonObject result = results.takeAt(0).toObject();
      LocationData* loc = parseResult(result);
      locs.push_back(loc);
   }
   
   return locs;
}