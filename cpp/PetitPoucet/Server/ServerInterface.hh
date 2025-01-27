#pragma once
#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <vector>
#include <iomanip>

#include <rtklib.h>

#include "../Utils/utils.hh"

namespace petitpoucet::serverinterface
{
    /*
    @brief This class stores the method for reading the confifile
    */
    class ConfigurationSetup
    {
        // Constructors
        public:
            ConfigurationSetup(){}
            ~ConfigurationSetup() = default;

        // Methods
        public:
            /*
            @brief Reads a configuration files and attibutes values to a caster name and a serial port name.
            @param std::string configFilName The file name where the configration is stored
            @param std::string* casterName the pointer to the caster name variable where the name of the caster stored in the config file will be stored
            @param std::string* serialPortName the pointer to the serial port name variable where the name of the serial port stored in the config file will be stored
            */
            void ReadConfigFile(std::string configFileName, std::string *casterName, std::string *serialPortName);

    };

    /*
    @brief Enum for the different coordinate systems
    */
   enum CoordinateSystem
   {
        WGSDegrees = 0,
        WGSDecimals = 1
   };

    /*
    @brief This class stores the options for the PPServer when setup by PPServerInterface::SetupServer
    */
    class PPServerOptions
    {
        //Constructor
        public:
        PPServerOptions(){}
        ~PPServerOptions(){};

        // Properties
        public:
        int inactiveTimeout = 10000; // 10 000 ms
        int intervalToReconnect = 10000; // 10 000 ms
        int averagingTimeOfDataRate = 500; // 500 ms
        int bufferSize = 32768; // 32768 bytes
        int serverCycle = 10; // 10ms
        int NMEARequestCycle = 0; // 0ms = no NMEA requests
        int fileSwapMargin = 30; // 30s
        int relayBackToOutputstream = 0; // 0 =  no relay back

        //Methods
        public:
        int* ToIntegers()
        {
            int* options = new int[8]{this->inactiveTimeout,
                                    this->intervalToReconnect,
                                    this->averagingTimeOfDataRate,
                                    this->bufferSize,
                                    this->serverCycle,
                                    this->NMEARequestCycle,
                                    this->fileSwapMargin,
                                    this->relayBackToOutputstream};
            return options; // Be careful, caller must delete this pointer
        };
    };

    class PPServer
    {
        // Constructor
        private:
        PPServer(){}
        public:
        ~PPServer(){};

        // Properties
        private:
        /*
        @brief The rtklib streamserver running in the background
        */
        inline static strsvr_t RTKLIBStreamServer;

        // Methods
        private:
        /*
        @brief reads the serial output os the reciever, extracts the relevant NMEA sentence and structures it
        */
        void ReadGNGGASolution(long double &longitude, long double &latitude, long double &altitude, int &signalToNoiseRatio, double &horizontalDilutionOfPrecision, std::string &fixQuality, int &timeStamp);
        /*
        @brief reads the serial output os the reciever, extracts the relevant NMEA sentence and structures it
        */
        void ReadGNRMCSolution(long double &longitude, long double &latitude, long double &altitude, int &signalToNoiseRatio, double &horizontalDilutionOfPrecision, std::string &fixQuality, int &timeStamp);
        public:
        /*
        @brief Sets a server up for correcting and reading corrected data
        */
        static PPServer SetupCorrectedReaderServer(std::string *casterName, std::string *serialPortName, PPServerOptions options);

        /*
        @brief Sets a server up to send correction data to an RTK reciever, and returns it as a Server object.
        @param std::string* casterName the pointer to the name of the caster we use for the RTK
        @param std::string* serialPortName the pointer to the name of the serial port where the RTK reciever is connected
        @param PPServerOptions options The options for the server
        @return PPServer a running server that sends the correction data to the reciever
        */
        static PPServer SetupCorrectionServer(std::string *casterName, std::string *serialPortName, PPServerOptions options);

        /*
        @brief Sets a server up to listen to an RTK reciever, and returns it as a Server object.
        @param std::string* serialPortName the pointer to the name of the serial port where the RTK reciever is connected
        @param PPServerOptions options The options for the server
        @return PPServer a running server that reads the  data from the reciever
        */
        static PPServer SetupReaderServer(std::string *serialPortName, PPServerOptions options);

        /*
        @brief Gets the status of the server and stores it in pointers given as parameters*/
        void GetServerStatus(int *stat, int *log_stat, int *byte, int *bps, std::string *stringMessage);

        void GetCurrentSolution(long double &longitude, long double &latitude, long double &altitude, int &signalToNoiseRatio, int &timeStamp, CoordinateSystem coordinateSystem);

        void GetCurrentSolutionQuality(int *signalToNoiseRatio, double *horizontalDilutionOfPrecision, std::string *fixQuality);
    };

    class PPServerInterface
    {
        // Constructor
        public:
        PPServerInterface(){}
        ~PPServerInterface(){};
    };
}