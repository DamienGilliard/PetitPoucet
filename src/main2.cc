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

#define MAXSTR      3 

int main(int argc, char **argv)
{
    std::string configFileName = "../config.txt";
    std::string casterName;
    std::string serialPortName;

    // Reading the config file
    std::shared_ptr<petitpoucet::filemanipulation::ConfigurationSetup> setup = std::make_shared<petitpoucet::filemanipulation::ConfigurationSetup>();
    setup->ReadConfigFile(configFileName, &casterName, &serialPortName);
    petitpoucet::serverinterface::PPServerOptions options;

    // Create the server
    petitpoucet::serverinterface::PPServer correctionServer = petitpoucet::serverinterface::PPServer::SetupCorrectionServer(&casterName, &serialPortName,options);
    sleepms(300);
    for (int intrflg=0;!intrflg;) 
    {
        int stat[3] = {0}, log_stat[3] = {0}, byte[3] = {0}, bps[3] = {0};
        std::string stringMessage;
        correctionServer.GetServerStatus(stat, log_stat, byte, bps, &stringMessage);
        sleepms(1000);
    }
    return 0;
}