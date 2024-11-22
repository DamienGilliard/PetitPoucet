#include "rtklib.h"
#include <iostream>
#include <fstream>
#include <memory>
#include "PetitPoucet.hh"

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
    petitpoucet::serverinterface::PPServer correctionServer = petitpoucet::serverinterface::PPServer::SetupCorrectionServer(&casterName, &serialPortName, options);
 
    // get server status at regular interval
    for (int intrflg=0;!intrflg;) {
        int stat[3] = {0}, log_stat[3] = {0}, byte[3] = {0}, bps[3] = {0};
        std::string stringMessage;
        correctionServer.GetServerStatus(stat, log_stat, byte, bps, &stringMessage);
        std::cout << "server running. | bytes: " << *byte << "| bps: " << *bps << "| source (0) destination (1):" << stringMessage << std::endl;
        sleepms(5000);
    }

}