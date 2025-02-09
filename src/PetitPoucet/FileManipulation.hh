#pragma once
#include <iostream>
#include <fstream>
#include <optional>

namespace petitpoucet::filemanipulation
{
    class ConfigurationSetup
    {
        // Constructors
        public:
            ConfigurationSetup(){}
            ~ConfigurationSetup() = default;

        // Methods
        public:
            void ReadConfigFile(std::string configFileName, std::string *casterName, std::string *serialPortName);

    };
}