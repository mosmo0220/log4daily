/**
 * @file mainThread.h
 * 
 * @brief This file contains the declarations for the MainThread class and related data structures.
 * 
 * The MainThread class provides functionality for managing the main thread of the log4daily application.
 */
#ifndef APPLICATION_MANAGER_H
#define APPLICATION_MANAGER_H

#include <iostream>
#include <string>
#include <vector>

// For handling user inputs
#include "./inputHandlers.h"
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
 * @class ApplicationManager
 * @brief Provides functionalities to manage the log4daily application.
 * 
 * The ApplicationManager class provides functionalities to manage the log4daily application.
 * It includes methods to run the main thread, update the log4daily file data, and show messages to the user.
 * 
 * The class also includes methods to get the response message from the main thread and the opened log4daily file data.
 */
class ApplicationManager {
public:
    ApplicationManager(std::string workingDirectory, std::string configPath);
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
     * @brief Registers the current date in the opened log4daily file.
     * 
     * This function registers the current date in the opened log4daily file.
     * The current date is used to mark the date for diary entry.
     */
    void registerToday();
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

#endif // APPLICATION_MANAGER_H