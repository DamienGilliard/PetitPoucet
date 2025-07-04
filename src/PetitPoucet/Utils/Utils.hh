#pragma once

#include <math.h>
#include <iostream>
#include <fstream>
#include <optional>
#include <vector>
#include <sstream>

/**
 * @brief Namespace for utilities.
 */
namespace petitpoucet::utils::conversions
{
    /**
    @brief converts the raw NMEA coordinates (DDMM.MMMMMM) to decimal format (DD.DDDDDDDDD)

    @param coordinate the coordinate in DDMM.MMMMMM format
    */
    void ConvertNMEAToWGS84Decimal(long double &coordinate);

    /**
    @brief converts the raw NMEA coordinates (DDMM.MMMMMM) to degrees format (DDMMSS.SSSSS)

    @param coordinate the coordinate in DDMM.MMMMMM format
    */
    void ConvertNMEAToWGS84Degrees(long double &coordinate);

    /**
    @brief converts the raw NMEA  (DDMM.MMMMMM) to degrees, minutes and seconds

    @param longitude the longitude in DDMM.MMMMMM format
    @param latitude the latitude in DDMM.MMMMMM format
    @param altitude the altitude in DDMM.MMMMMM format
    @param newLongitudeDegrees the longitude in degrees
    @param newLongitudeMinutes the longitude in minutes
    @param newLongitudeSeconds the longitude in seconds
    @param newLatitudeDegrees the latitude in degrees
    @param newLatitudeMinutes the latitude in minutes
    @param newLatitudeSeconds the latitude in seconds
    @param newAltitude the altitude in meters
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
                                   double &newAltitude);
    
    /**
    @brief converts decimal WGS84 coordinates to UTM coordinates in meters

    @param longitude the longitude in decimal degree format
    @param latitude the latitude in decimal degree format
    @param utmX the UTM X coordinate in meters
    @param utmY the UTM Y coordinate in meters
    */
    void ConvertDecimalWGS84ToUTM(double longitude,     
                                  double latitude,
                                  double &utmX,
                                  double &utmY);
}

/**
 * @brief Namespace for file manipulation utilities.
 */
namespace petitpoucet::utils::filemanipulation
{
    class ConfigurationSetup
    {
        // Constructors
        public:
            ConfigurationSetup(){}
            ~ConfigurationSetup() = default;

        // Methods
        public:
            /**
             * @brief Reads a configuration files and attibutes values to a caster name and a serial port name.
             * @param std::string configFilName The file name where the configration is stored
             * @param std::string* casterName the pointer to the caster name variable where the name of the caster stored in the config file will be stored
             * @param std::string* serialPortName the pointer to the serial port name variable where the name of the serial port stored in the config file will be stored
             * @param std::vector<std::string>* labels the vector of pointers where the labels stored in the config file will be stored
             */
            void ReadConfigFile(std::string configFileName, std::string *casterName, std::string *serialPortName, std::vector<std::string> *labels);

    };
}