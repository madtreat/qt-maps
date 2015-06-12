#ifndef GEOCODE_DATA_MANAGER_H
#define GEOCODE_DATA_MANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QJsonArray>
#include <QGeoCoordinate>

#include "locationdata.h"


class LocationData;

class GeocodeDataManager : public QObject
{
   Q_OBJECT
    
public:
   explicit GeocodeDataManager(QString _apiKey, QObject *parent = 0);
   ~GeocodeDataManager() {};
   
   void getCoordinates(const QString& address);

signals:
   void errorOccured(const QString&);
   void coordinatesReady(double east, double north);

private slots:
   void processReply(QNetworkReply* reply);

private:
   QString apiKey;
   QNetworkAccessManager* m_pNetworkAccessManager;

   bool parseStatus(QString status);
   QGeoCoordinate    parseGeoCoordinate(QJsonObject location);
   GeometryData*     parseGeometry(QJsonObject geo);
   LocationData*     parseResult(QJsonObject result);
   LocationDataList  parseResults(QJsonArray results);
};

#endif // GEOCODE_DATA_MANAGER_H
