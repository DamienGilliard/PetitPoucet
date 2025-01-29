#include <string>  // for operator+, to_string
#include <iostream>
#include <fstream>
#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp" 
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp" 
#include "ftxui/dom/elements.hpp" 
#include "ftxui/screen/color.hpp"

#include "../Server/ServerInterface.hh"

namespace petitpoucet::ui
{   
    // @brief Function to ask the user to define the config parameters.
    // @param casterName The reference to the name of the caster.
    // @param serialPortName The reference to the name of the serial port.
    void configfromUserInput(std::string &casterName, std::string &serialPortName);

    // @brief Function to ask the user to choose between two options.
    // @param option1 The first option.
    // @param option2 The second option.
    // @param message The message to display to the user to explain the choice presented.
    int giveChoiceTwoOptions(std::string option1, std::string option2, std::string &message);

    // @brief User interface for getting the instantaneous position.
    // @param allowableSNR The minimum signal to noise ratio to trigger the instantaneous position recording.
    void interfaceForInstantaneousPosition(int minimumSNR, petitpoucet::serverinterface::PPServerOptions options, std::string casterName, std::string serialPortName);

    // @brief User is prompted a trigger button. 
    // @param messageForTrigger The message to display to the user to explain the consequence of the trigger.
    void waitThenTriggerProcess(std::string messageForTrigger);

    // @brief The width of the window.
    int WINDOW_WIDTH = 80;

    // @brief Function to pad a message with a certain number of dashes on each side.
    // @param message The message to pad.
    // @param padding The number of dashes to add on each side.
    void padMessage(std::string &message, int padding);
} // namespace petitpoucet::ui
