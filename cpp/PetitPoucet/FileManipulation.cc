#include "FileManipulation.hh"

namespace petitpoucet::filemanipulation
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
                        if (&firstChar == "#"){continue;}

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
}

