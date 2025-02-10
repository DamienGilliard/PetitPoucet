#include "Utils.hh"

namespace petitpoucet::utils::conversions
{
    void ConvertNMEAToWGS84Decimal(long double &coordinate)
    {
        int degreeCoordinate = coordinate/100;
        coordinate = degreeCoordinate + (((coordinate/100 -  degreeCoordinate)/60) * 100);
    }

    void ConvertNMEAToWGS84Degrees(long double &coordinate)
    {
        int degreeCoordinate = coordinate/100;
        coordinate = degreeCoordinate;
    }

    void ConvertDecimalWGS84ToUTM(double longitude,     
                                  double latitude,
                                  double &utmX,
                                  double &utmY)
    {
        double a = 6378137.0; // WGS84 semi-major axis
        double f = 1.0 / 298.257223563; // WGS84 flattening
        double k0 = 0.9996; // scale factor
        double n = f / (2.0 - f); // third flattening
        double A = a / (1.0 + n) * (1.0 + n * n / 4.0 + n * n * n * n / 64.0); // radius of curvature in the prime vertical
        double alpha1 = (n / 2.0 - 2.0 * n * n / 3.0 + 5.0 * n * n * n / 16.0) / 2.0; // coefficients for the meridian arc
        double alpha2 = (13.0 * n * n / 48.0 - 3.0 * n * n * n / 5.0) / 3.0;
        double alpha3 = 61.0 * n * n * n / 240.0;
        double beta1 = n / 2.0 - 2.0 * n * n / 3.0 + 37.0 * n * n * n / 96.0;
        double beta2 = n * n / 48.0 + n * n * n / 15.0;
        double beta3 = 17.0 * n * n * n / 480.0;
        double delta1 = 2.0 * n - 2.0 * n * n / 3.0 - 2.0 * n * n * n / 5.0;
        double delta2 = 7.0 * n * n / 3.0 - 8.0 * n * n * n / 5.0;
        double delta3 = 56.0 * n * n * n / 15.0;

        
        double t = sinh(atanh(sin(latitude)) - 2.0 * sqrt(n) / (1.0 + n) * atanh(2.0 * sqrt(n) / (1.0 + n) * sin(latitude)));
        double xi = atan(t / cos(longitude - 0.0));
        double eta = atanh(sin(longitude - 0.0) / sqrt(1.0 + t * t));
        double sigma = sinh(A * atanh(sin(latitude)) - alpha1 * sin(2.0 * latitude) + alpha2 * sin(4.0 * latitude) - alpha3 * sin(6.0 * latitude));
        double tau = tan(latitude);
        double xi0 = xi - k0 * tau * cos(latitude) * cos(latitude) * (eta * eta / 2.0 + 5.0 * (1.0 - tau * tau + sigma * sigma) * eta * eta * eta * eta / 24.0);
        double eta0 = eta - k0 * cos(latitude) * cos(latitude) * (1.0 + sigma * sigma) * (xi - xi0) * (xi - xi0) * (xi - xi0) / 6.0;
        double x0 = 500000.0;
        double y0 = 0.0;
        double e = 0.0066943799901413165; // WGS84 first eccentricity squared
        double e2 = e / (1.0 - e); // WGS84 second eccentricity squared
        double e4 = e2 * e2; // WGS84 fourth eccentricity squared
        double e6 = e4 * e2; // WGS84 sixth eccentricity squared
        double e8 = e6 * e2; // WGS84 eighth eccentricity squared

        double A0 = 1.0 - e / 4.0 - 3.0 * e2 / 64.0 - 5.0 * e4 / 256.0 - 175.0 * e6 / 16384.0 - 441.0 * e8 / 65536.0;
        double A2 = 3.0 * (e + e2 / 4.0 + 15.0 * e4 / 128.0 + 525.0 * e6 / 16384.0 + 2205.0 * e8 / 65536.0);
        double A4 = 15.0 * (e2 / 128.0 + 3.0 * e4 / 512.0 + 35.0 * e6 / 2048.0 + 315.0 * e8 / 16384.0);
        double A6 = 35.0 * (e4 / 3072.0 + 5.0 * e6 / 12288.0 + 63.0 * e8 / 32768.0);
        double A8 = 315.0 * (e6 / 131072.0 + 7.0 * e8 / 65536.0);

        utmX = x0 + A0 * eta0 + A2 * sin(2.0 * eta0) + A4 * sin(4.0 * eta0) + A6 * sin(6.0 * eta0) + A8 * sin(8.0 * eta0);
        utmY = y0 + A0 * xi0 + A2 * sin(2.0 * xi0) + A4 * sin(4.0 * xi0) + A6 * sin(6.0 * xi0) + A8 * sin(8.0 * xi0);

    return;
    }
}

namespace petitpoucet::utils::filemanipulation
{
   void ConfigurationSetup::ReadConfigFile(std::string configFileName, std::string *casterName, std::string *serialPortName)
            {
                std::ifstream configFile(configFileName);
                std::string line;
                if (configFile.is_open())
                {
                    while (getline(configFile, line))
                    {
                        // avoid comment lines
                        char firstChar = line[0];
                        if (&firstChar == "#"){continue;}

                        int stringLength = line.size();
                        int index = line.find(":");
                        std::string flag = line.substr(0, index);
                        if(flag == "caster"){*casterName = line.substr(index+2, stringLength);};
                        if(flag == "serial"){*serialPortName = line.substr(index+2, stringLength);};
                    }
                    configFile.close();
                }
                else
                {
                    std::cout << "The config file 'config.txt' could not be read";
                }
            } 
}
