#include "rtklib.h"
#include <iostream>
#include <fstream>
#include <memory>
#include "PetitPoucet.hh"

#define MAXSTR      4 

int main(int argc, char **argv)
{
    // rtklib's stream server
    static strsvr_t streamServerOut; // stream server for reading reciever output
    static volatile int intrflg=0;
    static char s1[MAXSTR][MAXSTRPATH]={{0}};
    static char s2[MAXSTR][MAXSTRPATH]={{0}};
    static char cmd_strs[MAXSTR][MAXRCVCMD]={"","",""};
    static char cmd_periodic_strs[MAXSTR][MAXRCVCMD]={"","",""};
    const char ss[]={'E','-','W','C','C'};
    int options[]={10000,10000,2000,32768,10,0,30,0}; //options[0]= inactive timeout (ms); options[1]= interval to reconnect (ms); options[2]= averaging time of data rate (ms); options[3]= receive/send buffer size (bytes);; options[4]= server cycle (ms); options[5]= nmea request cycle (ms) (0:no); options[6]= file swap margin (s); options[7]= relay back of output stream (0:no)
    char *msg, *opt, *local="", *proxy="";
    int typesIn[MAXSTR] = {STR_NTRIPCLI, STR_SERIAL};
    int typesOut[MAXSTR] = {STR_SERIAL};
    int formatsIn[MAXSTR] = {-1, -1}, formatsOut = {-1}, stat[MAXSTR]={0}, log_stat[MAXSTR]={0}, byte[MAXSTR]={0}, bps[MAXSTR]={0};
    double staposIn[3]={0};
    double staposOut[3]={0};
    char *pathsIn[MAXSTR] = {0};
    char *pathsOut[MAXSTR] = {0};
    char *logsIn[MAXSTR] = {0};
    char *logsOut[MAXSTR] = {0};
    char *cmdsIn[MAXSTR] = {0};
    char *cmds_periodicIn[MAXSTR] = {0};
    char *cmdsOut[MAXSTR] = {0};
    char *cmds_periodicOut[MAXSTR] = {0};
    char strmsg[MAXSTRMSG]="", buff[256], *p;
    int i;
    

    strconv_t *converterOut[2]={NULL};

    for (int i=0;i<MAXSTR;i++) {
        pathsIn[i]=s1[i];
        pathsOut[i]=s1[i];
        logsIn[i]=s2[i];
        logsOut[i]=s2[i];
        cmdsIn[i]=cmd_strs[i];
        cmdsOut[i]=cmd_strs[i];
        cmds_periodicIn[i]=cmd_periodic_strs[i];
        cmds_periodicOut[i]=cmd_periodic_strs[i];
    }

    strsvrinit(&streamServerOut,2);
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

    //assign the names to the "paths" needed to start the server
    pathsOut[0] = new char[serialPortName.size() + 1];

    strcpy(pathsOut[0], serialPortName.c_str());

    // start rtklib's stream server
    if (!strsvrstart(&streamServerOut, options, typesOut, (const char **)pathsOut, (const char **)logsOut, converterOut, (const char **)cmdsOut, (const char **)cmds_periodicOut, staposOut))
    {
        std::cout << "server output start failed" << std::endl;
    }

    for (intrflg=0;!intrflg;) {
        
        /* get stream server status */
        strsvrstat(&streamServerOut,stat,log_stat,byte,bps,strmsg);
        
        /* show stream server status */
        // for (i=0,p=buff;i<MAXSTR;i++) p+=sprintf(p,"%c",ss[stat[i]+1]);
        
        fprintf(stderr,"%s [%s] %10d B %7d bps %s\n",
                time_str(utc2gpst(timeget()),0),buff,byte[0],bps[0],strmsg);
        std::cout << "String message: " << strmsg[0] << std::endl;
        
        sleepms(1000);
    }

}