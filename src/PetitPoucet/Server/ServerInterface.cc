#include "ServerInterface.hh"

namespace petitpoucet::serverinterface
{
    void ConfigurationSetup::ReadConfigFile(std::string configFileName, std::string *casterName, std::string *serialPortName)
    {
        std::ifstream configFile(configFileName);
        std::string line;
        if (configFile.is_open())
        {
            while (getline(configFile, line))
            {
                // avoid comment lines
                char firstChar = line[0];
                if (&firstChar == "#"){continue;};
                
                int stringLength = line.size();
                int index = line.find(":");
                std::string flag = line.substr(0, index);
                if(flag == "caster"){*casterName = line.substr(index+2, stringLength);};
                if(flag == "serial"){*serialPortName = line.substr(index+2, stringLength);};
            } 
            configFile.close();
        }
        else
        {
            std::cout << "The config file 'config.txt' could not be read";
        }
    }

    void PPServer::ReadGNGGASolution(long double &longitude, long double &latitude, long double &altitude, int &signalToNoiseRatio, double &horizontalDilutionOfPrecision, std::string &fixQuality, int &timeStamp)
    {
        std::string line;
        std::stringstream stringStreamedBuffer;
        stringStreamedBuffer << this->RTKLIBStreamServer.buff;
        int i = 0;
        char delimiter = ',';
        std::string item;
        std::vector<int> signalToNoiseRatios;
        int meanSignalToNoiseRatio;
        while(std::getline(stringStreamedBuffer, line))
        {
            std::vector<std::string> vectorizedGNSSMessage;
            if (line.rfind("$GNGGA", 0) == 0) 
            {
                std::stringstream stringStreamedLine(line);
                while (std::getline(stringStreamedLine, item, delimiter))
                {
                    vectorizedGNSSMessage.push_back(item);
                }
                int count = 1;
                while(vectorizedGNSSMessage[4].size() == 0 && count < 4)
                {
                    std::cout << "no coordinates detected from reciever. Waiting 2 seconds ..." << count << "/3" << std::endl;;
                    sleepms(2000);
                }
                if(vectorizedGNSSMessage[4].size() == 0 )
                {
                    std::cout << "Still no coordinates detected from reciever. Aborting" ;
                    EXIT_FAILURE;
                }
                longitude = std::stold(vectorizedGNSSMessage[4]);
                latitude = std::stold(vectorizedGNSSMessage[2]);
                altitude = std::stold(vectorizedGNSSMessage[9]);
                timeStamp = std::stold(vectorizedGNSSMessage[1]);
                if (vectorizedGNSSMessage[6] == "5")
                {
                    fixQuality = "| RTK fixed ! :)";
                }
                else if (vectorizedGNSSMessage[6] == "4")
                {
                    fixQuality = "| RTK float !";
                }
                else if (vectorizedGNSSMessage[6] == "2")
                {
                    fixQuality = "| Differential GPS fix";
                }
                else if (vectorizedGNSSMessage[6] == "1")
                {
                    fixQuality = "| GPS fix :/";
                }
                else if (vectorizedGNSSMessage[6] == "0")
                {
                    fixQuality = "| Invalid fix :(";
                }
                horizontalDilutionOfPrecision = std::stold(vectorizedGNSSMessage[8]);
            }
            
            if (line.rfind("$GPGSV", 0) == 0||line.rfind("$GLGSV", 0) == 0||line.rfind("$GAGSV", 0) == 0|| line.rfind("$GBGSV", 0) == 0)
            {
                std::stringstream stringStreamedLine(line);
                while (std::getline(stringStreamedLine, item, delimiter))
                {
                    vectorizedGNSSMessage.push_back(item);
                }
                if(!vectorizedGNSSMessage[7].length() == 0)
                {
                    signalToNoiseRatios.push_back(std::stoi(vectorizedGNSSMessage[7]));
                }
            }
            if(signalToNoiseRatios.size()>0)
            {
                for(int i=0; i<signalToNoiseRatios.size(); i++)
                {
                    meanSignalToNoiseRatio += signalToNoiseRatios[i];
                }
                meanSignalToNoiseRatio /= signalToNoiseRatios.size();
                signalToNoiseRatio = meanSignalToNoiseRatio;
            }
            
        }
    }

    PPServer PPServer::SetupCorrectionServer(std::string *casterName, std::string *serialPortName, PPServerOptions options)
    {
        PPServer CorrectionServer;

        static char s1[3][MAXSTRPATH]={{0}};
        static char s2[3][MAXSTRPATH]={{0}};
        static char cmd_strs[3][MAXRCVCMD]={"",""};
        static char cmd_periodic_strs[3][MAXRCVCMD]={"",""};
        char *paths[3] = {0};
        int types[3] = {STR_NTRIPCLI, STR_SERIAL};
        char *logs[3] = {0};
        strconv_t *converter[3] = {NULL};
        char* commands[3] = {0};
        char* commandsPeriodic[3] = {0};
        double stapos[3] = {0};

        for (int i=0;i<3;i++) 
        {
            paths[i]=s1[i];
            logs[i]=s2[i];
            commands[i]=cmd_strs[i];
            commandsPeriodic[i]=cmd_periodic_strs[i];
        }
        paths[0] = new char[casterName->size() + 1];
        paths[1] = new char[serialPortName->size() + 1];

        strcpy(paths[0], casterName->c_str());
        strcpy(paths[1], serialPortName->c_str());

        strsvrinit(&CorrectionServer.RTKLIBStreamServer, 2);
        strsetdir("");
        strsetproxy("");

        if (! strsvrstart(&CorrectionServer.RTKLIBStreamServer, 
                          options.ToIntegers(), 
                          types,
                          (const char **)paths,
                          (const char **)logs,
                          converter,
                          (const char **)commands,
                          (const char **)commandsPeriodic,
                          stapos))
        {
            std::cerr << "Correction server start failed" << std::endl;
        }
        
        return CorrectionServer;
    }

    PPServer PPServer::SetupReaderServer(std::string *serialPortName, PPServerOptions options)
    {
        PPServer ReaderServer;

        static char s1[3][MAXSTRPATH]={{0}};
        static char s2[3][MAXSTRPATH]={{0}};
        static char cmd_strs[3][MAXRCVCMD]={"","",""};
        static char cmd_periodic_strs[3][MAXRCVCMD]={"","",""};
        char *paths[3] = {0};
        int types[3] = {STR_SERIAL, STR_NONE};
        char *logs[3] = {0};
        strconv_t *converter[3] = {NULL};
        char* commands[3] = {0};
        char* commandsPeriodic[3] = {0};
        double stapos[3] = {0};

        for (int i=0;i<3;i++) 
        {
            paths[i]=s1[i];
            logs[i]=s2[i];
            commands[i]=cmd_strs[i];
            commandsPeriodic[i]=cmd_periodic_strs[i];
        }
        paths[0] = new char[serialPortName->size() + 1];

        strcpy(paths[0], serialPortName->c_str());

        strsvrinit(&ReaderServer.RTKLIBStreamServer, 2);
        strsetdir("");
        strsetproxy("");

        if (! strsvrstart(&ReaderServer.RTKLIBStreamServer, 
                          options.ToIntegers(), 
                          types,
                          (const char **)paths,
                          (const char **)logs,
                          converter,
                          (const char **)commands,
                          (const char **)commandsPeriodic,
                          stapos))
        {
            std::cerr << "Reader server start failed" << std::endl;
        }
        
        return ReaderServer;
    }

    void PPServer::GetServerStatus(int *stat, int *log_stat, int *byte, int *bps, std::string *stringMessage)
    {
        char charMessage[1024] ="";
        strsvrstat(&this->RTKLIBStreamServer, stat,log_stat, byte, bps, charMessage);
        *stringMessage = std::string(charMessage);
    }

    void PPServer::GetCurrentSolution(long double &longitude, long double &latitude, long double &altitude, int &signalToNoiseRatio, int &timeStamp, CoordinateSystem coordinateSystem, double &horizontalDilutionOfPrecision, std::string &fixQuality)
    {
        double error = 1;
        long double previousLong = 0;
        long double previousLat = 0;

        // Perform a single read operation
        this->ReadGNGGASolution(longitude, 
                                latitude, 
                                altitude, 
                                signalToNoiseRatio, 
                                horizontalDilutionOfPrecision, 
                                fixQuality, 
                                timeStamp);

        error = std::abs(previousLat - latitude + previousLong - longitude);
        previousLat = latitude;
        previousLong = longitude;

        if(coordinateSystem == 1) // if we use decimal WGS
        {
            petitpoucet::utils::conversions::ConvertNMEAToWGS84Decimal(longitude);
            petitpoucet::utils::conversions::ConvertNMEAToWGS84Decimal(latitude);
        }
        else if(coordinateSystem == 0) // if we use degrees WGS
        {
            petitpoucet::utils::conversions::ConvertNMEAToWGS84Degrees(longitude);
            petitpoucet::utils::conversions::ConvertNMEAToWGS84Degrees(latitude);
        }
    }
}
