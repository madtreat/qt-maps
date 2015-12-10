/* 
 * File:   LocationData.cpp
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 22, 2015, 11:08 PM
 */

#include "locationdata.h"
#include <math.h>


double calculateDistance(float lat1, float lon1, float lat2, float lon2)
{
   float phi1 = lat1.toRadians();
   float phi2 = lat2.toRadians();
   float deltaLambda = (lon2-lon1).toRadians();
   int   R = 6371000; // gives d in meters

   double d = acos( sin(phi1)*sin(phi2) + cos(phi1)*cos(phi2) * cos(deltaLambda) ) * R;
}
