#include "rtklib.h"
#include <iostream>
#include <fstream>
#include <memory>
#include "PetitPoucet.hh"

int main(int argc, char **argv)
{
    // rtklib's stream server
    static strsvr_t streamServer;
    int options[]={10000,10000,2000,32768,10,0,30,0}; //not quite sure what this is... but it is needed
    int types[2] = {STR_NTRIPCLI, STR_SERIAL};
    char *paths[2];
    char *logs[2];

    // Reading the config file
    std::shared_ptr<petitpoucet::filemanipulation::ConfigurationSetup> setup = std::make_shared<petitpoucet::filemanipulation::ConfigurationSetup>();

    std::string configFileName = "../config.txt";
    std::string casterName;
    std::string serialPortName;

    setup->ReadConfigFile(configFileName, &casterName, &serialPortName);
    std::cout << "caster name: " << casterName << std::endl;
    std::cout << "serial port name: " << serialPortName << std::endl;

    // assign the names to the "paths" needed to start the server
    paths[0] = new char[casterName.size() + 1]; // +1 for null terminator
    paths[1] = new char[serialPortName.size() + 1];
    strcpy(paths[0], casterName.c_str());
    strcpy(paths[1], serialPortName.c_str());

    // start rtklib's stream server
    //if (!strsvrstart(&streamServer, options, types, paths, logs,))

}