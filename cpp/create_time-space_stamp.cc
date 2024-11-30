#include "rtklib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include <typeinfo>
#include <iomanip>

#include "PetitPoucet.hh"

int main(int argc, char ** argv)
{
    std::string configFileName = "../config.txt";
    std::string casterName;
    std::string serialPortName;

    // Reading the config file
    std::shared_ptr<petitpoucet::filemanipulation::ConfigurationSetup> setup = std::make_shared<petitpoucet::filemanipulation::ConfigurationSetup>();
    setup->ReadConfigFile(configFileName, &casterName, &serialPortName);
    petitpoucet::serverinterface::PPServerOptions options;

    // Create the server
    petitpoucet::serverinterface::PPServer readerServer = petitpoucet::serverinterface::PPServer::SetupReaderServer(&serialPortName,options);

    // /* get stream server status */
    long double longitude = 0;
    long double latitude = 0;
    long double altitude = 0;
    int signalToNoiseRatio = 0;
    double horizontalDilutionOfPrecision = 0;
    std::string fixQuality = "";
    int timeStamp = 0;
    petitpoucet::serverinterface::CoordinateSystem coordSys = petitpoucet::serverinterface::WGSDecimals;

    std::cout << "Waiting 1 second..." << std::endl;
    sleepms(1000);
    
    while(timeStamp == 0)
    {
        readerServer.GetCurrentSolution(longitude,
                                        latitude,
                                        altitude, 
                                        signalToNoiseRatio,
                                        timeStamp,
                                        coordSys);
    }
    
    int hour = timeStamp/10000;
    int min = (timeStamp/100)%100;
    int sec = timeStamp%100;
    
    if(timeStamp)
    {
        std::cout << "Longitude:"<< std::fixed << std::setprecision(7) << longitude; 
        std::cout << " | Latitude:"<< std::fixed << std::setprecision(7) << latitude;
        std::cout << " | Altitude:"<< std::fixed << std::setprecision(2) << altitude;
        std::cout << " | SNR:"<< signalToNoiseRatio;
        std::cout << " | "<< hour << "h "<< min << "m "<< sec << "s" <<std::endl;;
    }
    // Save to file
    std::string filename = "position" + std::to_string(hour) + "h" + std::to_string(min) + "m" + std::to_string(sec) + "s.petitpoucet"; 
    std::ofstream outfile(filename);
    outfile << std::fixed << std::setprecision(7) << longitude << ";"
            << std::fixed << std::setprecision(7) << latitude << ";" 
            << std::fixed << std::setprecision(2) << altitude << ";" 
            << signalToNoiseRatio << std::endl;
    
    outfile.close();
}