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
    static char s1[2][MAXSTRPATH]={{0}};
    static char s2[2][MAXSTRPATH]={{0}};
    static char cmd_strs[2][MAXRCVCMD]={"",""};
    static char cmd_periodic_strs[2][MAXRCVCMD]={"",""};
    int options[]={10000,10000,2000,32768,10,0,30,0}; //not quite sure what this is... but it is needed
    char *msg, *opt, *local, *proxy;
    int types[2] = {STR_NTRIPCLI, STR_SERIAL};
    int formats[2] = {-1, -1};
    double stapos[3]={0};
    char *paths[2] = {0};
    char *logs[2] = {0};
    char *cmds[2];
    char *cmds_periodic[2];

    //dummy message and option
    msg = new char[1];
    opt = new char[1];
    local = new char[1];
    proxy = new char[1];
    *cmds = {new char[0]};
    *cmds_periodic = {new char[0]};

    *msg = ' ';
    *opt = ' ';
    *local = ' ';
    *proxy = ' ';

    strconv_t *converter[1]={NULL};

    for (int i=0;i<2;i++) {
        paths[i]=s1[i];
        logs[i]=s2[i];
        cmds[i]=cmd_strs[i];
        cmds_periodic[i]=cmd_periodic_strs[i];
    }

    strsvrinit(&streamServer,2);
    strsetdir(local);
    strsetproxy(proxy);

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