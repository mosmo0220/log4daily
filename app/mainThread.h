/**
 * @file mainThread.h
 * 
 * @brief This file contains the declarations for the MainThread class and related data structures.
 * 
 * The MainThread class provides functionality for managing the main thread of the log4daily application.
 */
#ifndef MAIN_THREAD_H
#define MAIN_THREAD_H

#include <iostream>
#include <string>
#include <vector>

// For handling user inputs
#include "handlers.h"
// For handling log4daily files
#include "./Storage/localStorage.h"

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
    MainThread(std::string workingDirectory, std::string configPath);
    /**
     * @brief Runs the main thread of the log4daily application.
     * 
     * This function runs the main thread of the log4daily application based on the console inputs.
     * It parses the console inputs, filters the supported commands, and executes the commands accordingly.
     * 
     * @param argc The number of command-line arguments.
     * @param argv The array of command-line arguments.
     * @return CommandType The type of command executed by the main thread.
     */
    CommandType run(int argc, char* argv[]);
    /**
     * @brief Updates the data in the opened log4daily file.
     * 
     * This function updates the data in the opened log4daily file with the specified data.
     * 
     * @param data The updated data to be saved to the log4daily file.
     */
    void updateFileData(FileData data);
    /**
     * @brief Shows a message based on the command response.
     * 
     * This function shows a message based on the response to the executed command.
     * The message is displayed in the console to inform the user about the result of the command.
     * 
     * @param respond The type of command response.
     * @param message The additional message to display.
     */
    void showMessage(CommandType respond, std::string message = "");
    /**
     * @brief Gets the response message from the main thread.
     * 
     * This function returns the response message from the main thread.
     * The response message is used to inform the user about the result of the executed command.
     * 
     * @return std::string The response message from the main thread.
     */
    std::string getRespondMessage();
    /**
     * @brief Gets the opened log4daily file data.
     * 
     * This function returns the data of the opened log4daily file.
     * The data includes the file name, calendar events, to-do items, milestones, and diary entries.
     * 
     * @return FileData The data of the opened log4daily file.
     */
    FileData getOpenedFile();
private:
    LocalStorage localStorage;
    std::string workingDirectory;
    std::string configName;
    FileData openedFile;
    std::string respondMessage;
    void showHelp();
};

#endif // MAIN_THREAD_H