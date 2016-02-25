/* 
 * File:   smarker.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 15, 2015, 11:25 AM
 */

#ifndef SMARKER_H
#define SMARKER_H

#include <QString>

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

#endif  // SMARKER_H

