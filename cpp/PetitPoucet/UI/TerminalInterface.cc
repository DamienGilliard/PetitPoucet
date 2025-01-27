#include "TerminalInterface.hh"

namespace petitpoucet::ui
{
    void configfromUserInput(std::string &casterName, std::string &serialPortName)
    {
        auto screen = ftxui::ScreenInteractive::FitComponent();
        
        ftxui::Component inputCasterName = ftxui::Input(&casterName, "Caster name");
        ftxui::Component inputserialPortName = ftxui::Input(&serialPortName, "Serial port name");
        auto okButton = ftxui::Button("OK", [&] { screen.ExitLoopClosure()(); }, ftxui::ButtonOption::Animated(ftxui::Color::Blue));

        std::string message = "Please define the config parameters";
        padMessage(message, WINDOW_WIDTH);
        auto secondComponent = ftxui::Renderer(ftxui::Container::Vertical({
            inputCasterName,
            inputserialPortName,
            okButton,
            }), [&] {
                return ftxui::vbox({
                    ftxui::vbox({
                        ftxui::text(message),
                    }) | ftxui::border,
                    inputCasterName->Render(),
                    inputserialPortName->Render(),
                    ftxui::separator(),
                    okButton->Render(),
                    
                });
            });
            
        screen.Loop(secondComponent);

        // Create some space between the different components
        std::cout << " " << std::endl;
        std::cout << " " << std::endl;
        std::cout << " " << std::endl;

    }

    int giveChoiceTwoOptions(std::string option1, std::string option2, std::string message)
    {
        int answer;
        auto screen = ftxui::ScreenInteractive::FitComponent();

        padMessage(option1, WINDOW_WIDTH/2);
        padMessage(option2, WINDOW_WIDTH/2);
        // The tree of components. This defines how to navigate using the keyboard.
        auto buttons = ftxui::Container::Horizontal({
            ftxui::Button(
                option1, [&] { answer = 1; screen.ExitLoopClosure()(); }, ftxui::ButtonOption::Animated(ftxui::Color::RGB(150, 100, 30))),
            ftxui::Button(
                option2, [&] { answer = 0; screen.ExitLoopClosure()(); }, ftxui::ButtonOption::Animated(ftxui::Color::Green)),
        });
        
        // Modify the way to render them on screen:
        padMessage(message, WINDOW_WIDTH);
        auto thirdComponent = ftxui::Renderer(buttons, [&] {
            return ftxui::vbox({
                ftxui::vbox({
                    ftxui::text(message),
                }) | ftxui::border,
                buttons->Render(),
            });
        });
        screen.Loop(thirdComponent);

        // Create some space between the different components
        std::cout << " " << std::endl;
        std::cout << " " << std::endl;
        std::cout << " " << std::endl;

        return answer;
    }

    void interfaceForInstantaneousPosition(int minimumSNR)
    {
        std::cout << "Getting instantaneous position" << std::endl;
    }
    
    void padMessage(std::string &message, int padding)
    {
        int paddingOnSides = (padding - message.size()) / 2;
        message = std::string(paddingOnSides, ' ') + message + std::string(paddingOnSides, ' ');
        if (message.size() < WINDOW_WIDTH) {
            message += " ";
        }
    }
} // namespace petitpoucet::ui