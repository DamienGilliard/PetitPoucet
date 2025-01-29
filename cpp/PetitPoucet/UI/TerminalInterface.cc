#include "TerminalInterface.hh"

namespace petitpoucet::ui
{
    void configfromUserInput(std::string &casterName, std::string &serialPortName)
    {
        auto screen = ftxui::ScreenInteractive::Fullscreen();
        
        ftxui::Component inputCasterName = ftxui::Input(&casterName, "Caster name");
        ftxui::Component inputserialPortName = ftxui::Input(&serialPortName, "Serial port name");
        auto okButton = ftxui::Button("OK", [&] { screen.ExitLoopClosure()(); }, ftxui::ButtonOption::Animated(ftxui::Color::Blue));

        std::string message = "Please define the config parameters";
        padMessage(message, WINDOW_WIDTH);
        auto component = ftxui::Renderer(ftxui::Container::Vertical({
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
            
        screen.Loop(component);

    }

    int giveChoiceTwoOptions(std::string option1, std::string option2, std::string &message)
    {
        int answer;
        std::atomic<bool> running(true);
        std::string liveMessage = message;
        auto screen = ftxui::ScreenInteractive::Fullscreen();

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
        padMessage(liveMessage, WINDOW_WIDTH);
        auto component = ftxui::Renderer(buttons, [&] {
            return ftxui::vbox({
                ftxui::vbox({
                    ftxui::text(liveMessage),
                }) | ftxui::border,
                buttons->Render(),
            });
        });

        screen.Loop(component);

        return answer;
    }

    void interfaceForInstantaneousPosition(int minimumSNR, petitpoucet::serverinterface::PPServerOptions options, std::string casterName, std::string serialPortName)  
    {
        auto screen = ftxui::ScreenInteractive::Fullscreen();
        std::atomic<bool> running(true);
        std::string staticMessage = "Waiting for signal to noise ratio to be above " + std::to_string(minimumSNR);
        std::string liveMessage, SNRMessage, liveLongitude, liveLatitude, liveAltitude, liveTime = "initial message";
        int liveHour, liveMin, liveSec;
        auto messageMutex = std::make_shared<std::mutex>();

        std::thread correctionServerThread([&] {
            petitpoucet::serverinterface::PPServer correctionServer = petitpoucet::serverinterface::PPServer::SetupCorrectionServer(&casterName, &serialPortName, options);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            while (running) {
                int stat[3] = {0}, log_stat[3] = {0}, byte[3] = {0}, bps[3] = {0};
                std::string stringMessage;
                correctionServer.GetServerStatus(stat, log_stat, byte, bps, &stringMessage);
                {
                    std::lock_guard<std::mutex> lock(*messageMutex);
                    liveMessage = "Correction server bytes: " + std::to_string(*byte);
                }
                screen.PostEvent(ftxui::Event::Custom);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        });

        std::thread positionServerThread([&] {
            petitpoucet::serverinterface::PPServer readerServer = petitpoucet::serverinterface::PPServer::SetupReaderServer(&serialPortName, options);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            while (running) {
                long double longitude = 0;
                long double latitude = 0;
                long double altitude = 0;
                int signalToNoiseRatio = 0;
                double horizontalDilutionOfPrecision = 0;
                std::string fixQuality = "";
                int timeStamp = 0;
                petitpoucet::serverinterface::CoordinateSystem coordSys = petitpoucet::serverinterface::WGSDegrees;
                readerServer.GetCurrentSolution(longitude,
                                                latitude,
                                                altitude,
                                                signalToNoiseRatio,
                                                timeStamp,
                                                coordSys);
                int hour = timeStamp / 10000;
                int min = (timeStamp / 100) % 100;
                int sec = timeStamp % 100;
                longitude = longitude / 100;
                latitude = latitude / 100;

                if(timeStamp)
                {
                    std::lock_guard<std::mutex> lock(*messageMutex);
                    SNRMessage = "Signal to noise ratio: " + std::to_string(signalToNoiseRatio);

                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(7) << longitude;
                    liveLongitude = oss.str();
                    oss.str(""); // Clear the stream
                    oss << std::fixed << std::setprecision(7) << latitude;
                    liveLatitude = oss.str();
                    oss.str(""); // Clear the stream
                    oss << std::fixed << std::setprecision(7) << altitude;
                    liveAltitude = oss.str();

                    liveHour = hour;
                    liveMin = min;
                    liveSec = sec;

                    liveTime = std::to_string(hour) + "h" + std::to_string(min) + "m" + std::to_string(sec) + "s";
                }
                screen.PostEvent(ftxui::Event::Custom);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        });

        auto buttons = ftxui::Container::Horizontal({
        ftxui::Button(">>>>>>>>>> Trigger <<<<<<<<<<", [&] 
        {
            std::string filename = "position" + liveTime + ".petitpoucet"; 
            std::ofstream outfile(filename);
            outfile << liveLongitude << ";"
                    << liveLatitude << ";" 
                    << liveAltitude << ";" 
                    << SNRMessage << std::endl;
    
            outfile.close();
        }, ftxui::ButtonOption::Animated(ftxui::Color::Blue)),
        
        ftxui::Button("XXXXXXXXXX Quit XXXXXXXXXX", [&] { screen.ExitLoopClosure()(); }, ftxui::ButtonOption::Animated(ftxui::Color::Red)),
        });

        auto component = ftxui::Renderer(buttons, [&] 
        {
            std::lock_guard<std::mutex> lock(*messageMutex);
            return ftxui::vbox({
                ftxui::vbox({
                    ftxui::text(liveTime),
                    ftxui::text(liveMessage),
                    ftxui::text(SNRMessage),
                    ftxui::hbox({
                        ftxui::vbox({
                            ftxui::text("Longitude:"),
                            ftxui::text(liveLongitude),
                        }) | ftxui::border,
                        ftxui::vbox({
                            ftxui::text("Latitude:"),
                            ftxui::text(liveLatitude),
                        }) | ftxui::border,
                        ftxui::vbox({
                            ftxui::text("Altitude:"),
                            ftxui::text(liveAltitude),
                        }) | ftxui::border,
                    }),
                }) | ftxui::border,
                buttons->Render(),
            });
        });
        screen.Loop(component);
        running = false;
        correctionServerThread.join();
        positionServerThread.join();

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