#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "geocode_data_manager.h"

namespace Ui {
    class Form;
}

class MapSettings;
class SMarker;

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(MapSettings* settings, QWidget *parent = 0);
    ~Form();



private slots:
    void goClicked();

    void showCoordinates(double lat, double lon, bool saveMarker = true);
    //set marker to map and save marker in markers list
    void setMarker(double lat, double lon, QString caption);
    void errorOccurred(const QString&);



    void on_lwMarkers_currentRowChanged(int currentRow);

    void on_pbRemoveMarker_clicked();

    void on_zoomSpinBox_valueChanged(int arg1);

private:
    void getCoordinates(const QString& address);

private:
    Ui::Form *ui;
    MapSettings* settings;
    GeocodeDataManager m_geocodeDataManager;
    //markers list
    QList <SMarker*> m_markers;
};

#endif // FORM_H
