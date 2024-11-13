#include "mainThread.h"

#include <iostream>
#include <string>
#include <vector>

// For handling user inputs
#include "handlers.h"
// For handling log4daily files
#include "./Storage/localStorage.h"

MainThread::MainThread(std::string workingDirectory, std::string configPath) : localStorage(workingDirectory + configPath) {
    this->workingDirectory = workingDirectory;
    this->configName = configPath;
}

CommandType MainThread::run(int argc, char* argv[]) {
    std::vector<Command> commands = Handlers::parseConsoleInputs(argc, argv);
    std::vector<Command> supportedCommands = Handlers::filterForSupportedCommands(commands);
    
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

void MainThread::updateFileData(FileData data) {
    localStorage.updateDataToFile(workingDirectory, data.log4FileName, data);
}

void MainThread::showMessage(CommandType respond, std::string message) {
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

std::string MainThread::getRespondMessage() {
    return respondMessage;
}

FileData MainThread::getOpenedFile() {
    return openedFile;
}

void MainThread::showHelp() {
    std::cout << "Usage: log4daily [command] [log4_file_name]" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  --new    [log4_file_name]    Create a new log4daily file" << std::endl;
    std::cout << "  --open   [log4_file_name]    Open an existing log4daily file" << std::endl;
    std::cout << "  --delete [log4_file_name]    Delete an existing log4daily file" << std::endl;
    std::cout << "  --help                       Show this help message" << std::endl;
}
