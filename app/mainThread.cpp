#include <iostream>
#include <string>
#include <vector>

// For handling user inputs
#include "handlers.cpp"
// For handling log4daily files
#include "localStorage.cpp"

enum class CommandType {
    New,
    Open,
    Delete,
    Help,
    Unsupported,
    Failed,
    Other
};

/**
 * @class MainThread
 * @brief Provides functionalities to manage the main thread of the log4daily application.
 * 
 * The MainThread class provides functionalities to manage the main thread of the log4daily application.
 * It handles the parsing of console inputs, filtering of supported commands, and execution of the commands.
 * The class also provides methods to show help messages and respond to user inputs.
 */
class MainThread {
public:
    MainThread(std::string workingDirectory, std::string configPath) : localStorage(workingDirectory + configPath) {
            this->workingDirectory = workingDirectory;
            this->configName = configPath;
        }
    
    /**
     * @file MainThread.cpp
     * @brief Implementation of the MainThread class for handling log4daily commands.
     * 
     * This file contains the implementation of the MainThread class, which is responsible
     * for processing and executing commands related to log4daily files. The class provides
     * methods to parse console inputs, filter supported commands, and execute commands such
     * as creating, opening, and deleting log4daily files.
     * 
     * The MainThread class includes the following key methods:
     * - run: Parses and executes commands based on console inputs.
     * - showMessage: Displays a message based on the result of a command execution.
     * - getRespondMessage: Retrieves the response message after command execution.
     * - getOpenedFile: Retrieves the opened log4daily file data.
     * - showHelp: Displays help information for available commands.
     * 
     * The class also maintains internal state such as the working directory, configuration
     * name, opened file data, and response message.
     * 
     * Usage:
     * @code
     * MainThread mt("/path/to/working/directory", "config.cfg");
     * CommandType result = mt.run(argc, argv);
     * mt.showMessage(result);
     * @endcode
     * 
     * @note This class relies on external dependencies such as LocalStorage, consoleHandlers,
     * and controlHandlers, which are assumed to be defined elsewhere in the project.
     * 
     * @see LocalStorage
     * @see consoleHandlers
     * @see controlHandlers
    */
    CommandType run(int argc, char* argv[]) {
        std::vector<consoleHandlers::Command> commands = consoleHandlers::parseConsoleInputs(argc, argv);
        std::vector<consoleHandlers::Command> supportedCommands = controlHandlers::filterForSupportedCommands(commands);
        
        std::string commandName;
        std::string commandArgument;

        commandName = supportedCommands.empty() ? "" : supportedCommands[0].name;
        commandArgument = supportedCommands.empty() ? "" : supportedCommands[0].argument;

        if (supportedCommands.empty()) {
            return CommandType::Unsupported;
        }

        if (supportedCommands.size() > 1) {
            std::cout << "Multiple supported commands found. Picking first one, ignoring others" << std::endl;
        }

        if (supportedCommands[0].name != commands[0].name) {
            std::cout << "First supported command is different from first unfilted command." << std::endl;
            commandName = commands[0].name;
            commandArgument = commands[0].argument;
        }

        if (commandName == "--open") {
            openedFile = localStorage.openLog4DailyFile(workingDirectory, commandArgument);
            if (!(openedFile == FileData())) {
                respondMessage = "Opening it now!";
                return CommandType::Open;
            } else {
                respondMessage = "Failed to open log4daily file. (maybe it does not exist?)";
                return CommandType::Failed;
            }
        } else if (commandName == "--new") {
            bool succeeded = localStorage.createLog4DailyFile(workingDirectory, commandArgument, configName);
            if (succeeded) {
                openedFile = localStorage.openLog4DailyFile(workingDirectory, commandArgument);
                respondMessage = "Opening it now!";
                return CommandType::New;
            } else {
                respondMessage = "Failed to create log4daily file. (maybe it already exists?)";
                return CommandType::Failed;
            }
        } else if (commandName == "--delete") {
            std::cout << "Are you sure you want to delete the log4daily file? (y/n): ";
            std::string confirmation = "";
            std::cin >> confirmation;

            if (confirmation != "y" && confirmation != "Y") {
                respondMessage = "Operation canceled.";
                return CommandType::Other;
            }

            bool succeeded = localStorage.deleteLog4DailyFile(workingDirectory, commandArgument, configName);
            if (succeeded) {
                return CommandType::Delete;
            } else {
                respondMessage = "Failed to delete log4daily file.";
                return CommandType::Failed;
            }
        } else if (commandName == "--help") {
            showHelp();
            return CommandType::Help;
        } else {
            std::cout << "Unsupported command: " << commands[0].name << std::endl;
            std::cout << "Showing help instead." << std::endl;
            showHelp();
            return CommandType::Help;
        }

        respondMessage = "Failed to execute command.";
        return CommandType::Failed;
    }

    void updateFileData(FileData data) {
        localStorage.updateDataToFile(workingDirectory, data.log4FileName, data);
    }

    /**
     * @brief Displays a message based on the result of a command execution.
     * 
     * This function prints a message to the console depending on the result of a command
     * executed by the `CommandType` function. It provides feedback to the user about the
     * success or failure of the command, as well as any additional messages passed to it.
     * 
     * @param respond The result of the command execution, represented by the `CommandType` enum.
     * @param message An optional additional message to display. Defaults to an empty string.
    */
    void showMessage(CommandType respond, std::string message = "") {
        switch (respond)
        {
            case CommandType::New:
                std::cout << "New log4daily file created successfully. ";
                break;
            case CommandType::Open:
                std::cout << "Log4daily file opened successfully. ";
                break;
            case CommandType::Delete:
                std::cout << "Log4daily file deleted successfully. ";
                break;
            case CommandType::Help:
                std::cout << "Help command executed. ";
                break;
            case CommandType::Unsupported:
                std::cout << "No supported commands found. ";
                break;
            case CommandType::Failed:
                std::cout << "Failed to execute command. ";
                break;
            case CommandType::Other:
                break;
            default:
                std::cout << "Undefined application behavior. ";
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

    void showHelp() {
        std::cout << "Usage: log4daily [command] [log4_file_name]" << std::endl;
        std::cout << "Commands:" << std::endl;
        std::cout << "  --new    [log4_file_name]    Create a new log4daily file" << std::endl;
        std::cout << "  --open   [log4_file_name]    Open an existing log4daily file" << std::endl;
        std::cout << "  --delete [log4_file_name]    Delete an existing log4daily file" << std::endl;
        std::cout << "  --help                       Show this help message" << std::endl;
    }
};
