#include "rtklib.h"
#include <iostream>
#include <fstream>
#include <memory>
#include "PetitPoucet.hh"
#include <chrono>


int main(int argc, char **argv)
{
    std::string configFileName = "../config.txt";
    std::string casterName;
    std::string serialPortName, serialPortNamecopy;
    std::string readConfigFileMessage = "Do you want to use a config file or define the parameters yourself?";
    int readConfigFile = petitpoucet::ui::giveChoiceTwoOptions("Read from config file", " Define myself at runtime", readConfigFileMessage);

    if(readConfigFile == 1)
    {
        std::shared_ptr<petitpoucet::filemanipulation::ConfigurationSetup> setup = std::make_shared<petitpoucet::filemanipulation::ConfigurationSetup>();
        setup->ReadConfigFile(configFileName, &casterName, &serialPortName);
    }
    else if (readConfigFile == 0)
    {
        petitpoucet::ui::configfromUserInput(casterName, serialPortName);
    }

    std::string messageForInstantaneous = "Do you want to get instantaneous position or position over time?";
    int justInstantaneous = petitpoucet::ui::giveChoiceTwoOptions("Instantaneous", "Over time", messageForInstantaneous);

    int stat[3] = {0}, log_stat[3] = {0}, byte[3] = {0}, bps[3] = {0};
    std::string stringMessage;
    petitpoucet::serverinterface::PPServerOptions options;
    petitpoucet::serverinterface::CoordinateSystem coordinateSystem = petitpoucet::serverinterface::CoordinateSystem::WGSDecimals;

    if(justInstantaneous)
    {
        petitpoucet::ui::interfaceForInstantaneousPosition(30, options, casterName, serialPortName, coordinateSystem);
    }
    else
    {
        petitpoucet::ui::interfaceForPositionOverTime(30, options, casterName, serialPortName, coordinateSystem);
    }
    return 0;
}