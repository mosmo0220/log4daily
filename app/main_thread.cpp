#include <iostream>
#include <string>
#include <vector>

// For handling user inputs
#include "handlers.cpp"
// For handling log4daily files
#include "localStorage.cpp"
// For runThread enum
#include "structures.cpp"

using namespace ftxui;

/**
 * @class main_thread
 * @brief Provides functionalities to manage the main thread of the log4daily application.
 * 
 * The main_thread class provides functionalities to manage the main thread of the log4daily application.
 * It handles the parsing of console inputs, filtering of supported commands, and execution of the commands.
 * The class also provides methods to show help messages and respond to user inputs.
 */
class main_thread {
public:
    main_thread(std::string workingDirectory, std::string configPath) : localStorage(workingDirectory + configPath) {
            this->workingDirectory = workingDirectory;
            this->configName = configPath;
        }
    
    /**
     * @file main_thread.cpp
     * @brief Implementation of the main_thread class for handling log4daily commands.
     * 
     * This file contains the implementation of the main_thread class, which is responsible
     * for processing and executing commands related to log4daily files. The class provides
     * methods to parse console inputs, filter supported commands, and execute commands such
     * as creating, opening, and deleting log4daily files.
     * 
     * The main_thread class includes the following key methods:
     * - run: Parses and executes commands based on console inputs.
     * - ShowMessage: Displays a message based on the result of a command execution.
     * - getRespondMessage: Retrieves the response message after command execution.
     * - getOpenedFile: Retrieves the opened log4daily file data.
     * - show_help: Displays help information for available commands.
     * 
     * The class also maintains internal state such as the working directory, configuration
     * name, opened file data, and response message.
     * 
     * Usage:
     * @code
     * main_thread mt("/path/to/working/directory", "config.cfg");
     * runThread result = mt.run(argc, argv);
     * mt.ShowMessage(result);
     * @endcode
     * 
     * @note This class relies on external dependencies such as LocalStorage, consoleHandlers,
     * and controlHandlers, which are assumed to be defined elsewhere in the project.
     * 
     * @see LocalStorage
     * @see consoleHandlers
     * @see controlHandlers
    */
    runThread run(int argc, char* argv[]) {
        std::vector<consoleHandlers::Command> commands = consoleHandlers::parseConsoleInputs(argc, argv);
        std::vector<consoleHandlers::Command> supportedCommands = controlHandlers::filterForSupportedCommands(commands);
        
        std::string command_name;
        std::string command_argument;

        command_name = supportedCommands.empty() ? "" : supportedCommands[0].name;
        command_argument = supportedCommands.empty() ? "" : supportedCommands[0].argument;

        if (supportedCommands.empty()) {
            std::cout << "No supported commands found." << std::endl;
            return runThread::UNSUPPORTED;
        }

        if (supportedCommands.size() > 1) {
            std::cout << "Multiple supported commands found. Picking first one, ignoring others" << std::endl;
        }

        if (supportedCommands[0].name != commands[0].name) {
            std::cout << "First supported command is different from first unfilted command." << std::endl;
            command_name = commands[0].name;
            command_argument = commands[0].argument;
        }

        if (command_name == "--open") {
            openedFile = localStorage.openLog4DailyFile(workingDirectory, command_argument);
            return runThread::OPEN;
        } else if (command_name == "--new") {
            bool succesed = localStorage.createLog4DailyFile(workingDirectory, command_argument, configName);
            if (succesed) {
                openedFile = localStorage.openLog4DailyFile(workingDirectory, command_argument);
                respondMessage = "Oppeing it now!";
                return runThread::NEW;
            } else {
                respondMessage = "Failed to create log4daily file.";
                return runThread::FAILED;
            }
        } else if (command_name == "--delete") {
            std::cout << "Are you sure you want to delete the log4daily file? (y/n): ";
            std::string confirmation = "";
            std::cin >> confirmation;

            if (confirmation != "y" && confirmation != "Y") {
                respondMessage = "Operation canceled.";
                return runThread::OTHER;
            }

            bool succesed = localStorage.deleteLog4DailyFile(workingDirectory, command_argument, configName);
            if (succesed) {
                return runThread::DELETE;
            } else {
                respondMessage = "Failed to delete log4daily file.";
                return runThread::FAILED;
            }
        } else if (command_name == "--help") {
            show_help();
            return runThread::HELP;
        } else {
            std::cout << "Unsupported command: " << commands[0].name << std::endl;
            std::cout << "Showing help instead." << std::endl;
            show_help();
            return runThread::HELP;
        }

        respondMessage = "Failed to execute command.";
        return runThread::FAILED;
    }

    /**
     * @brief Displays a message based on the result of a command execution.
     * 
     * This function prints a message to the console depending on the result of a command
     * executed by the `runThread` function. It provides feedback to the user about the
     * success or failure of the command, as well as any additional messages passed to it.
     * 
     * @param respond The result of the command execution, represented by the `runThread` enum.
     * @param message An optional additional message to display. Defaults to an empty string.
    */
    void ShowMessage(runThread respond, std::string message = "") {
        std::cout << std::endl;
        switch (respond)
        {
            case runThread::NEW:
                std::cout << "New log4daily file created successfully." << std::endl;
                break;
            case runThread::OPEN:
                std::cout << "Log4daily file opened successfully." << std::endl;
                break;
            case runThread::DELETE:
                std::cout << "Log4daily file deleted successfully." << std::endl;
                break;
            case HELP:
                std::cout << "Help command executed." << std::endl;
                break;
            case runThread::UNSUPPORTED:
                std::cout << "No supported commands found." << std::endl;
                break;
            case runThread::FAILED:
                std::cout << "Failed to execute command." << std::endl;
                break;
            case runThread::OTHER:
                break;
            default:
                std::cout << "Undefined application behavior." << std::endl;
        }
        if (!message.empty()) std::cout << message << std::endl;
    }

    std::string getRespondMessage() {
        return respondMessage;
    }

    FileData getOpenedFile() {
        return openedFile;
    }
private:
    LocalStorage localStorage;
    std::string workingDirectory;
    std::string configName;

    FileData openedFile = FileData();
    std::string respondMessage = "";

    void show_help() {
        std::cout << "Usage: log4daily [command] [log4_file_name]" << std::endl;
        std::cout << "Commands:" << std::endl;
        std::cout << "  --new    [log4_file_name]    Create a new log4daily file" << std::endl;
        std::cout << "  --open   [log4_file_name]    Open an existing log4daily file" << std::endl;
        std::cout << "  --delete [log4_file_name]    Delete an existing log4daily file" << std::endl;
        std::cout << "  --help                       Show this help message" << std::endl;
    }
};
