#include "rtklib.h"
#include <iostream>
#include <fstream>
#include <memory>
#include "PetitPoucet.hh"

int main(int argc, char **argv)
{
    // rtklib's stream server
    static strsvr_t streamServer;
    static volatile int intrflg=0;
    int options[]={10000,10000,2000,32768,10,0,30,0}; //not quite sure what this is... but it is needed
    char *msg, *opt;
    int types[2] = {STR_NTRIPCLI, STR_SERIAL};
    double stapos[3]={0};
    char *paths[2];
    char *logs[2];
    char *cmds[2];
    char *cmds_periodic[2];

    //dummy message and option
    msg = new char[1];
    opt = new char[1];
    *cmds = {new char[0]};
    *cmds_periodic = {new char[0]};

    *msg = ' ';
    *opt = ' ';

    // rtklib's stream converter
    strconv_t *converter[1]={NULL};
    if (!(converter[1] = strconvnew(0, 0, msg, 0, 1, opt))){
        std::cout<<"Help, converstion of stream converter failed !!" << std::endl;
    }

    strsvrinit(&streamServer,2);

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
    if (!strsvrstart(&streamServer, options, types, (const char **)paths, (const char **)logs, converter, (const char **)cmds, (const char **)cmds_periodic, stapos))
    {
        std::cout << "server start failed" << std::endl;
    }

}