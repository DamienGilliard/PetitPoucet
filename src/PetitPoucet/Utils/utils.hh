#pragma once

#include <math.h>

namespace petitpoucet::utils
{
    /*
    @brief converts the raw NMEA coordinates (DDMM.MMMMMM) to decimal format (DD.DDDDDDDDD)
    */
    void ConvertNMEAToWGS84Decimal(long double &coordinate);

    /*
    @brief converts the raw NMEA coordinates (DDMM.MMMMMM) to degrees format (DD)
    */
    void ConvertNMEAToWGS84Degrees(long double &coordinate);

    /*
    @brief converts the raw NMEA  (DDMM.MMMMMM) to degrees, minutes and seconds
    */
    void ConvertNMEAToWGS84Decimal(double longitude, 
                                   double latitude, 
                                   double altitude, 
                                   double &newLongitudeDegrees,
                                   double &newLongitudeMinutes,
                                   double &newLongitudeSeconds, 
                                   double &newLatitudeDegrees,
                                   double &newLatitudeMinutes,
                                   double &newLatitudeSeconds, 
                                   double &newAltitudeDegrees,
                                   double &newAltitudeMinutes,
                                   double &newAltitudeSeconds);
    
    /*
    @brief converts decimal WGS84 coordinates to UTM coordinates in meters

    @param longitude the longitude in decimal degree format
    @param latitude the latitude in decimal degree format
    @param utmX the UTM X coordinate in meters
    */
    void ConvertDecimalWGS84ToUTM(double longitude,     
                                  double latitude,
                                  double &utmX,
                                  double &utmY);
}