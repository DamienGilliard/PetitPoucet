#pragma once

namespace petitpoucet::utils
{
    /*
    @brief converts the raw NMEA coordinates (DDMM.MMMMMM) to decimal format (DD.DDDDDDDDD)
    */
    double ConvertNMEAToWGS84Decimal(double coordinate);

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
}