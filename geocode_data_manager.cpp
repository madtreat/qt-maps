#include "geocode_data_manager.h"

#include <QJson/Parser>
#include <QDebug>


GeocodeDataManager::GeocodeDataManager(QString _apiKey, QObject *parent) :
    QObject(parent),
    apiKey(_apiKey)
{
    m_pNetworkAccessManager = new QNetworkAccessManager(this);
    connect(m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void GeocodeDataManager::getCoordinates(const QString& address)
{
    QString url = QString("https://maps.googleapis.com/maps/api/geocode/json?q=%1&key=%2&output=json&oe=utf8&sensor=false").arg(address).arg(apiKey);
    m_pNetworkAccessManager->get(QNetworkRequest(QUrl(url)));
    qDebug() << "Getting coordinates:\n" << url;
}


void GeocodeDataManager::replyFinished(QNetworkReply* reply)
{
    QString json = reply->readAll();
    qDebug() << "Reply = " << json;
    qDebug() << "URL = " << reply->url();
    QString strUrl = reply->url().toString();

//    QJson::Parser parser;

//    bool ok;

//    // json is a QString containing the data to convert
//    QVariant result = parser.parse (json.toLatin1(), &ok);
//    if(!ok)
//    {
//        emit errorOccured(QString("Cannot convert to QJson object: %1").arg(json));
//        return;
//    }

//    int code = result.toMap()["Status"].toMap()["code"].toInt();
//    if(code != 200)
//    {
//        emit errorOccured(QString("Code of request is: %1").arg(code));
//        return;
//    }

//    QVariantList placeMarks = result.toMap()["Placemark"].toList();
//    if(placeMarks.count() == 0)
//    {
//        emit errorOccured(QString("Cannot find any locations"));
//        return;
//    }

//    double east  = placeMarks[0].toMap()["Point"].toMap()["coordinates"].toList()[0].toDouble();
//    double north = placeMarks[0].toMap()["Point"].toMap()["coordinates"].toList()[1].toDouble();

//    emit coordinatesReady(east, north);

}
