#include "rtklib.h"
#include <iostream>
#include <fstream>
#include <memory>
#include "../PetitPoucet.hh"
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void runCorrectionServer()
    {
        execl("./correction_server", "./correction_server", NULL);
        std::cerr << "execl() failed!\n";
    }

int main(int argc, char **argv)
{
    std::string configFileName = "../config.txt";
    std::string casterName;
    std::string serialPortName, serialPortNamecopy;
    std::string readConfigFileMessage = "Do you want to use a config file or define the parameters yourself?";
    int readConfigFile = petitpoucet::ui::giveChoiceTwoOptions("Read from config file", " Define myself at runtime", readConfigFileMessage);

    if(readConfigFile == 0)
    {
        std::shared_ptr<petitpoucet::utils::filemanipulation::ConfigurationSetup> setup = std::make_shared<petitpoucet::utils::filemanipulation::ConfigurationSetup>();
        setup->ReadConfigFile(configFileName, &casterName, &serialPortName);
    }
    else if (readConfigFile == 1)
    {
        petitpoucet::ui::configfromUserInput(casterName, serialPortName);
    }

    std::string messageForInstantaneous = "Do you want to get instantaneous position or position over time?";
    int overTime = petitpoucet::ui::giveChoiceTwoOptions("Instantaneous", "Over time", messageForInstantaneous);

    petitpoucet::serverinterface::PPServerOptions options;
    petitpoucet::serverinterface::CoordinateSystem coordinateSystem = petitpoucet::serverinterface::CoordinateSystem::WGSDecimals;

    pid_t pid = fork();

    if(pid < 0) 
    {
        std::cerr << "Fork failed!\n";
    }
    if(pid == 0)
    {
        runCorrectionServer();
    }
    else
    {
        if(!overTime)
        {
            petitpoucet::ui::interfaceForInstantaneousPosition(30, options, casterName, serialPortName, coordinateSystem);
        }
        else
        {
            petitpoucet::ui::interfaceForPositionOverTime(30, options, casterName, serialPortName, coordinateSystem);
        }
        kill(pid, SIGTERM);
    }
    return 0;
}