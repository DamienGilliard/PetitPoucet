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
                option1, [&] { answer = 0; screen.ExitLoopClosure()(); }, ftxui::ButtonOption::Animated(ftxui::Color::RGB(150, 100, 30))),
            ftxui::Button(
                option2, [&] { answer = 1; screen.ExitLoopClosure()(); }, ftxui::ButtonOption::Animated(ftxui::Color::Green)),
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

    int giveChoiceMultipleOptions(std::vector<std::string> options, std::string &message)
    {
        int answer = 0;
        std::atomic<bool> running(true);
        std::string liveMessage = message;
        auto screen = ftxui::ScreenInteractive::Fullscreen();
        for(auto &option : options)
        {
            padMessage(option, WINDOW_WIDTH / options.size());
        }

        std::vector<ftxui::Component> buttonsVec;
        for (size_t i = 0; i < options.size(); ++i) {
            buttonsVec.push_back(ftxui::Button(
                options[i], [&] { answer = i; screen.ExitLoopClosure()(); }, ftxui::ButtonOption::Animated(ftxui::Color::RGB(255 * double(i)/double(options.size()-1), 100, 255 * (1-(double(i)/double(options.size()-1)))))));
        }
        auto buttons = ftxui::Container::Horizontal(buttonsVec);

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

    void interfaceForInstantaneousPosition(int minimumSNR, petitpoucet::serverinterface::PPServerOptions options, std::string casterName, std::string serialPortName, petitpoucet::serverinterface::CoordinateSystem coordinateSystem)  
    {
        auto screen = ftxui::ScreenInteractive::Fullscreen();
        std::atomic<bool> running(true);
        std::string staticMessage = "Waiting for signal to noise ratio to be above " + std::to_string(minimumSNR);
        std::string liveMessage, SNRMessage, liveLongitude, liveLatitude, liveAltitude, liveTime, fixQuality = "initial message";
        int liveHour, liveMin, liveSec;
        double horizontalDilutionOfPrecision = 0;
        auto messageMutex = std::make_shared<std::mutex>();

        std::thread positionServerThread([&] 
        {
            petitpoucet::serverinterface::PPServer readerServer = petitpoucet::serverinterface::PPServer::SetupReaderServer(&serialPortName, options);
            while (running) {
                long double longitude = 0;
                long double latitude = 0;
                long double altitude = 0;
                int signalToNoiseRatio = 0;
                int timeStamp = 0;
                readerServer.GetCurrentSolution(longitude,
                                                latitude,
                                                altitude,
                                                signalToNoiseRatio,
                                                timeStamp,
                                                coordinateSystem,
                                                horizontalDilutionOfPrecision,
                                                fixQuality);
                int hour = timeStamp / 10000;
                int min = (timeStamp / 100) % 100;
                int sec = timeStamp % 100;

                if(timeStamp)
                {
                    std::lock_guard<std::mutex> lock(*messageMutex);
                    SNRMessage = "Signal to noise ratio: " + std::to_string(signalToNoiseRatio);

                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(8) << longitude;
                    liveLongitude = oss.str();
                    oss.str(""); // Clear the stream
                    oss << std::fixed << std::setprecision(8) << latitude;
                    liveLatitude = oss.str();
                    oss.str(""); // Clear the stream
                    oss << std::fixed << std::setprecision(3) << altitude;
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
                    ftxui::text(fixQuality),
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
        positionServerThread.join();

    }
    
    void interfaceForPositionOverTime(int minimumSNR, petitpoucet::serverinterface::PPServerOptions options, std::string casterName, std::string serialPortName, petitpoucet::serverinterface::CoordinateSystem coordinateSystem)
    {
        auto screen = ftxui::ScreenInteractive::Fullscreen();
        std::atomic<bool> running(true);
        std::string staticMessage = "Waiting for signal to noise ratio to be above " + std::to_string(minimumSNR);
        std::string liveMessage, SNRMessage, liveLongitude, liveLatitude, liveAltitude, liveTime, liveFixQuality = "initial message";
        std::vector<long double> longitudes, latitudes, altitudes;
        std::vector<int> signalToNoiseRatios;
        bool recording = false;

        long double meanLongitude, meanLatitude, meanAltitude, liveHorizontalDilutionOfPrecision = 0;
        int meanSignalToNoiseRatio = 0;
        long double stdDevLongitude, stdDevLatitude, stdDevAltitude = 0;
        int stdDevSignalToNoiseRatio = 0;  
        int liveHour, liveMin, liveSec;
        auto messageMutex = std::make_shared<std::mutex>();

        std::thread positionServerThread([&] {
            petitpoucet::serverinterface::PPServer readerServer = petitpoucet::serverinterface::PPServer::SetupReaderServer(&serialPortName, options);

            while (running) {
                long double longitude, latitude, altitude = 0;
                double horizontalDilutionOfPrecision = 0;
                int signalToNoiseRatio = 0;
                int timeStamp = 0;
                std::string fixQuality = "";
                readerServer.GetCurrentSolution(longitude,
                                                latitude,
                                                altitude,
                                                signalToNoiseRatio,
                                                timeStamp,
                                                coordinateSystem,
                                                horizontalDilutionOfPrecision,
                                                fixQuality);

                int hour = timeStamp / 10000;
                int min = (timeStamp / 100) % 100;
                int sec = timeStamp % 100;

                if(horizontalDilutionOfPrecision)
                {
                    liveHorizontalDilutionOfPrecision = horizontalDilutionOfPrecision;
                    liveFixQuality = fixQuality;
                }

                if(timeStamp)
                {
                    std::lock_guard<std::mutex> lock(*messageMutex);
                    SNRMessage = "Signal to noise ratio: " + std::to_string(signalToNoiseRatio);

                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(8) << longitude;
                    liveLongitude = oss.str();
                    oss.str(""); // Clear the stream
                    oss << std::fixed << std::setprecision(8) << latitude;
                    liveLatitude = oss.str();
                    oss.str(""); // Clear the stream
                    oss << std::fixed << std::setprecision(3) << altitude;
                    liveAltitude = oss.str();

                    liveHour = hour;
                    liveMin = min;
                    liveSec = sec;

                    liveTime = std::to_string(hour) + "h" + std::to_string(min) + "m" + std::to_string(sec) + "s";

                    if(recording)
                    {
                        longitudes.push_back(longitude);
                        latitudes.push_back(latitude);
                        altitudes.push_back(altitude);
                        signalToNoiseRatios.push_back(signalToNoiseRatio);
                        meanLongitude = std::accumulate(longitudes.begin(), longitudes.end(), 0.0) / longitudes.size();
                        meanLatitude = std::accumulate(latitudes.begin(), latitudes.end(), 0.0) / latitudes.size();
                        meanAltitude = std::accumulate(altitudes.begin(), altitudes.end(), 0.0) / altitudes.size();
                        meanSignalToNoiseRatio = std::accumulate(signalToNoiseRatios.begin(), signalToNoiseRatios.end(), 0) / signalToNoiseRatios.size();

                        stdDevLongitude = std::sqrt(std::inner_product(longitudes.begin(), longitudes.end(), longitudes.begin(), 0.0) / longitudes.size() - meanLongitude * meanLongitude);
                        stdDevLatitude = std::sqrt(std::inner_product(latitudes.begin(), latitudes.end(), latitudes.begin(), 0.0) / latitudes.size() - meanLatitude * meanLatitude);
                        stdDevAltitude = std::sqrt(std::inner_product(altitudes.begin(), altitudes.end(), altitudes.begin(), 0.0) / altitudes.size() - meanAltitude * meanAltitude);
                        stdDevSignalToNoiseRatio = std::sqrt(std::inner_product(signalToNoiseRatios.begin(), signalToNoiseRatios.end(), signalToNoiseRatios.begin(), 0) / signalToNoiseRatios.size() - meanSignalToNoiseRatio * meanSignalToNoiseRatio);
                    }
                }
                screen.PostEvent(ftxui::Event::Custom);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        });

        auto buttons = ftxui::Container::Horizontal({
        ftxui::Button(">>>>>>> Toggle <<<<<<<", [&] 
        {
            recording = !recording;

        }, ftxui::ButtonOption::Animated(ftxui::Color::Blue)),
        
        ftxui::Button("XXXXXXX Quit XXXXXXXX", [&]
        { 
            screen.ExitLoopClosure()(); 
        
        }, ftxui::ButtonOption::Animated(ftxui::Color::Red)),

        ftxui::Button("VVVVVVV Save VVVVVVV", [&] 
        {
            recording = false;
            std::string filename = "position" + liveTime + ".petitpoucet";
            std::ofstream outfile(filename);
            outfile << "#Mean longitude;Mean latitude;Mean altitude;SNR;stddev longitude;stddev latitude;stddev altitude;stddev SNR" << std::endl;
            outfile << std::fixed << std::setprecision(7)
                    << meanLongitude << ";"
                    << meanLatitude << ";"
                    << meanAltitude << ";"
                    << meanSignalToNoiseRatio << ";"
                    << stdDevLongitude << ";"
                    << stdDevLatitude << ";"
                    << stdDevAltitude << ";"
                    << stdDevSignalToNoiseRatio << std::endl;
            outfile.close();

        }, ftxui::ButtonOption::Animated(ftxui::Color::Green)),

        ftxui::Button("0000000 Reset 0000000", [&] 
        {
            recording = false;
            longitudes.clear();
            latitudes.clear();
            altitudes.clear();
            signalToNoiseRatios.clear();
            meanLongitude = 0;
            meanLatitude = 0;
            meanAltitude = 0;
            meanSignalToNoiseRatio = 0;
            stdDevLongitude = 0;
            stdDevLatitude = 0;
            stdDevAltitude = 0;
            stdDevSignalToNoiseRatio = 0;
        }, ftxui::ButtonOption::Animated(ftxui::Color::Yellow)),
        });

        auto component = ftxui::Renderer(buttons, [&] 
        {
            std::lock_guard<std::mutex> lock(*messageMutex);
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(7);
            return ftxui::vbox({
                ftxui::vbox({
                    ftxui::text(liveTime),
                    ftxui::text(liveFixQuality),
                    ftxui::text(SNRMessage),
                    ftxui::hbox({
                        ftxui::vbox({
                            ftxui::text("Longitude:"),
                            ftxui::text(liveLongitude),
                            [&] {
                                oss.str("");
                                oss << "Mean Longitude: " << meanLongitude;
                                return ftxui::text(oss.str());
                            }(),
                            [&] {
                                oss.str("");
                                oss << "Std Dev Longitude: " << stdDevLongitude;
                                return ftxui::text(oss.str());
                            }(),
                        }) | ftxui::border,
                        ftxui::vbox({
                            ftxui::text("Latitude:"),
                            ftxui::text(liveLatitude),
                            [&] {
                                oss.str("");
                                oss << "Mean Latitude: " << meanLatitude;
                                return ftxui::text(oss.str());
                            }(),
                            [&] {
                                oss.str("");
                                oss << "Std Dev Latitude: " << stdDevLatitude;
                                return ftxui::text(oss.str());
                            }(),
                        }) | ftxui::border,
                        ftxui::vbox({
                            ftxui::text("Altitude:"),
                            ftxui::text(liveAltitude),
                            [&] {
                                oss.str("");
                                oss << "Mean Altitude: " << meanAltitude;
                                return ftxui::text(oss.str());
                            }(),
                            [&] {
                                oss.str("");
                                oss << "Std Dev Altitude: " << stdDevAltitude;
                                return ftxui::text(oss.str());
                            }(),
                        }) | ftxui::border,
                    }),
                    [&] {
                        oss.str("");
                        oss << "Mean SNR: " << meanSignalToNoiseRatio;
                        return ftxui::vbox({
                            ftxui::text(oss.str()),
                            [&] {
                                oss.str("");
                                oss << "Std Dev SNR: " << stdDevSignalToNoiseRatio;
                                return ftxui::text(oss.str());
                            }(),
                        }) | ftxui::border;
                    }(),
                }) | ftxui::border,
                buttons->Render(),
            });
        });
        screen.Loop(component);
        running = false;
        positionServerThread.join();

    }

    void interfaceForPositionOverTimeWithLabelsAndTimer(int minimumSNR, petitpoucet::serverinterface::PPServerOptions options, std::string casterName, std::string serialPortName, petitpoucet::serverinterface::CoordinateSystem coordinateSystem, std::chrono::seconds recordingTime, std::vector<std::string> labels)
    {
        auto screen = ftxui::ScreenInteractive::Fullscreen();
        std::atomic<bool> running(true);
        std::string staticMessage = "Waiting for signal to noise ratio to be above " + std::to_string(minimumSNR);
        std::string species, liveMessage, SNRMessage, liveLongitude, liveLatitude, liveAltitude, liveTime, liveFixQuality = "initial message";
        std::vector<long double> longitudes, latitudes, altitudes;
        std::vector<int> signalToNoiseRatios;
        bool recording = true;
        std::chrono::seconds secondsLeft = recordingTime.count();
        
        // Just a small timer to show the user how much time is left for recording
        std::thread timerThread([&] 
        {
            while (running) && secondsLeft.count() > 0
            {
                secondsLeft = secondsLeft- std::chrono::seconds(1);
                if (secondsLeft.count() <= 0) 
                {
                    running = false;
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            screen.PostEvent(ftxui::Event::Custom);
        });

        long double meanLongitude, meanLatitude, meanAltitude, liveHorizontalDilutionOfPrecision = 0;
        int meanSignalToNoiseRatio = 0;
        long double stdDevLongitude, stdDevLatitude, stdDevAltitude = 0;
        int stdDevSignalToNoiseRatio = 0;  
        int liveHour, liveMin, liveSec;
        auto messageMutex = std::make_shared<std::mutex>();

        std::thread positionServerThread([&] {
            petitpoucet::serverinterface::PPServer readerServer = petitpoucet::serverinterface::PPServer::SetupReaderServer(&serialPortName, options);

            while (running) {
                long double longitude, latitude, altitude = 0;
                double horizontalDilutionOfPrecision = 0;
                int signalToNoiseRatio = 0;
                int timeStamp = 0;
                std::string fixQuality = "";
                readerServer.GetCurrentSolution(longitude,
                                                latitude,
                                                altitude,
                                                signalToNoiseRatio,
                                                timeStamp,
                                                coordinateSystem,
                                                horizontalDilutionOfPrecision,
                                                fixQuality);

                int hour = timeStamp / 10000;
                int min = (timeStamp / 100) % 100;
                int sec = timeStamp % 100;

                if(horizontalDilutionOfPrecision)
                {
                    liveHorizontalDilutionOfPrecision = horizontalDilutionOfPrecision;
                    liveFixQuality = fixQuality;
                }

                if(timeStamp)
                {
                    std::lock_guard<std::mutex> lock(*messageMutex);
                    SNRMessage = "Signal to noise ratio: " + std::to_string(signalToNoiseRatio);

                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(8) << longitude;
                    liveLongitude = oss.str();
                    oss.str(""); // Clear the stream
                    oss << std::fixed << std::setprecision(8) << latitude;
                    liveLatitude = oss.str();
                    oss.str(""); // Clear the stream
                    oss << std::fixed << std::setprecision(3) << altitude;
                    liveAltitude = oss.str();

                    liveHour = hour;
                    liveMin = min;
                    liveSec = sec;

                    liveTime = std::to_string(hour) + "h" + std::to_string(min) + "m" + std::to_string(sec) + "s";

                    if(recording)
                    {
                        longitudes.push_back(longitude);
                        latitudes.push_back(latitude);
                        altitudes.push_back(altitude);
                        signalToNoiseRatios.push_back(signalToNoiseRatio);
                        meanLongitude = std::accumulate(longitudes.begin(), longitudes.end(), 0.0) / longitudes.size();
                        meanLatitude = std::accumulate(latitudes.begin(), latitudes.end(), 0.0) / latitudes.size();
                        meanAltitude = std::accumulate(altitudes.begin(), altitudes.end(), 0.0) / altitudes.size();
                        meanSignalToNoiseRatio = std::accumulate(signalToNoiseRatios.begin(), signalToNoiseRatios.end(), 0) / signalToNoiseRatios.size();

                        stdDevLongitude = std::sqrt(std::inner_product(longitudes.begin(), longitudes.end(), longitudes.begin(), 0.0) / longitudes.size() - meanLongitude * meanLongitude);
                        stdDevLatitude = std::sqrt(std::inner_product(latitudes.begin(), latitudes.end(), latitudes.begin(), 0.0) / latitudes.size() - meanLatitude * meanLatitude);
                        stdDevAltitude = std::sqrt(std::inner_product(altitudes.begin(), altitudes.end(), altitudes.begin(), 0.0) / altitudes.size() - meanAltitude * meanAltitude);
                        stdDevSignalToNoiseRatio = std::sqrt(std::inner_product(signalToNoiseRatios.begin(), signalToNoiseRatios.end(), signalToNoiseRatios.begin(), 0) / signalToNoiseRatios.size() - meanSignalToNoiseRatio * meanSignalToNoiseRatio);
                    }
                }
                screen.PostEvent(ftxui::Event::Custom);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }

            if (!running)
            {
                std::lock_guard<std::mutex> lock(*messageMutex);
                recording = false; // Stop recording when the timer ends
                std::string filename = "position" + liveTime + ".csv";
                std::ofstream outfile(filename);
                outfile << "#Mean longitude;Mean latitude;Mean altitude;SNR;stddev longitude;stddev latitude;stddev altitude;stddev SNR" << std::endl;
                outfile << std::fixed << std::setprecision(7)
                        << meanLongitude << ";"
                        << meanLatitude << ";"
                        << meanAltitude << ";"
                        << meanSignalToNoiseRatio << ";"
                        << stdDevLongitude << ";"
                        << stdDevLatitude << ";"
                        << stdDevAltitude << ";"
                        << stdDevSignalToNoiseRatio << std::endl;
                outfile.close();
            }
        });

        std::vector<ftxui::Component> buttonsVec;
        for (size_t i = 0; i < labels.size(); ++i) 
        {
            buttonsVec.push_back(ftxui::Button(
                labels[i], [&] { species = labels[i]; screen.ExitLoopClosure()(); }, ftxui::ButtonOption::Animated(ftxui::Color::RGB(255 * double(i)/double(labels.size()-1), 100, 255 * (1-(double(i)/double(labels.size()-1)))))));
        }
        auto buttons = ftxui::Container::Horizontal(buttonsVec);

        auto component = ftxui::Renderer(buttons, [&] 
        {
            std::lock_guard<std::mutex> lock(*messageMutex);
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(7);
            return ftxui::vbox({
                ftxui::vbox({
                    ftxui::text(liveTime),
                    ftxui::text("Time of recording left: " + std::to_string(secondsLeft.count()) + " seconds"),
                    ftxui::text("species label:" + species),
                    ftxui::text(liveFixQuality),
                    ftxui::text(SNRMessage),
                    ftxui::hbox({
                        ftxui::vbox({
                            ftxui::text("Longitude:"),
                            ftxui::text(liveLongitude),
                            [&] {
                                oss.str("");
                                oss << "Mean Longitude: " << meanLongitude;
                                return ftxui::text(oss.str());
                            }(),
                            [&] {
                                oss.str("");
                                oss << "Std Dev Longitude: " << stdDevLongitude;
                                return ftxui::text(oss.str());
                            }(),
                        }) | ftxui::border,
                        ftxui::vbox({
                            ftxui::text("Latitude:"),
                            ftxui::text(liveLatitude),
                            [&] {
                                oss.str("");
                                oss << "Mean Latitude: " << meanLatitude;
                                return ftxui::text(oss.str());
                            }(),
                            [&] {
                                oss.str("");
                                oss << "Std Dev Latitude: " << stdDevLatitude;
                                return ftxui::text(oss.str());
                            }(),
                        }) | ftxui::border,
                        ftxui::vbox({
                            ftxui::text("Altitude:"),
                            ftxui::text(liveAltitude),
                            [&] {
                                oss.str("");
                                oss << "Mean Altitude: " << meanAltitude;
                                return ftxui::text(oss.str());
                            }(),
                            [&] {
                                oss.str("");
                                oss << "Std Dev Altitude: " << stdDevAltitude;
                                return ftxui::text(oss.str());
                            }(),
                        }) | ftxui::border,
                    }),
                    [&] {
                        oss.str("");
                        oss << "Mean SNR: " << meanSignalToNoiseRatio;
                        return ftxui::vbox({
                            ftxui::text(oss.str()),
                            [&] {
                                oss.str("");
                                oss << "Std Dev SNR: " << stdDevSignalToNoiseRatio;
                                return ftxui::text(oss.str());
                            }(),
                        }) | ftxui::border;
                    }(),
                }) | ftxui::border,
                buttons->Render(),
            });
        });
        screen.Loop(component);
        running = false;
        positionServerThread.join();
        timerThread.join();
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