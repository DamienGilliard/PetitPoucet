#include "rtklib.h"
#include <iostream>
#include <fstream>
#include <memory>
#include "PetitPoucet.hh"
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
    std::vector<std::string> labels = {"Label1", "Label2", "Label3", "Label4", "Label5"};
    if(readConfigFile == 0)
    {
        std::shared_ptr<petitpoucet::utils::filemanipulation::ConfigurationSetup> setup = std::make_shared<petitpoucet::utils::filemanipulation::ConfigurationSetup>();
        setup->ReadConfigFile(configFileName, &casterName, &serialPortName);
    }
    else if (readConfigFile == 1)
    {
        petitpoucet::ui::configfromUserInput(casterName, serialPortName);
    }

    std::string messageForLabels = "Do you want to associate position with labels ?";
    int positionWithLabels = petitpoucet::ui::giveChoiceTwoOptions("associate position with labels", "do not associate position with labels", messageForLabels);

    std::string messageForRecordingTime = "For how long do you want to record the position?";
    int recordingTime = petitpoucet::ui::giveChoiceMultipleOptions({"30s", "60s", "120s", "5min"}, messageForRecordingTime);
    if(recordingTime == 0)
    {
        recordingTime = 30;
    }
    else if(recordingTime == 1)
    {
        recordingTime = 60;
    }
    else if(recordingTime == 2)
    {
        recordingTime = 120;
    }
    else if(recordingTime == 3)
    {
        recordingTime = 300;
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
            petitpoucet::ui::interfaceForPositionOverTimeWithLabelsAndTimer(30, options, casterName, serialPortName, coordinateSystem, std::chrono::seconds(recordingTime), labels);

            petitpoucet::ui::interfaceForPositionOverTime(30, options, casterName, serialPortName, coordinateSystem);
        }
        kill(pid, SIGTERM);
    }
    return 0;
}