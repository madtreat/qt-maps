#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "geocode_data_manager.h"

namespace Ui {
    class Form;
}


//structure for save markers data
struct SMarker
{
    SMarker()
    {
        lat = 0;
        lon = 0;
        caption = "";
    };
    SMarker(double _lat, double _lon, QString _caption)
    {
        lat = _lat; lon = _lon; caption = _caption;
    };

    double lat;
    double lon;
    QString caption;
};


class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QString _apiKey, QWidget *parent = 0);
    ~Form();



private slots:
    void goClicked();

    void showCoordinates(double lat, double lon, bool saveMarker = true);
    //set marker to map and save marker in markers list
    void setMarker(double lat, double lon, QString caption);
    void errorOccured(const QString&);



    void on_lwMarkers_currentRowChanged(int currentRow);

    void on_pbRemoveMarker_clicked();

    void on_zoomSpinBox_valueChanged(int arg1);

private:
    void getCoordinates(const QString& address);
    QString getHTMLWithAPIKey(QString htmlFile);

private:
    Ui::Form *ui;
    QString apiKey;
    GeocodeDataManager m_geocodeDataManager;
    //markers list
    QList <SMarker*> m_markers;
};

#endif // FORM_H
