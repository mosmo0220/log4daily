#ifndef MANAGECONFIG_H
#define MANAGECONFIG_H

#include <iostream>
#include <fstream>

#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#include <shlobj.h>
#define MKDIR(path) _mkdir(path)
#else
#include <unistd.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

class ManageConfig {

enum class ConfigFolderStatus {
    CREATED,
    EXISTS,
    ERROR
};

public:
    std::string configFileName = "configuration_log4daily.json";
    ManageConfig() {};
    /**
     * @brief Manages the configuration for the log4daily application.
     * 
     * This function manages the configuration for the log4daily application by creating the configuration folder
     * and configuration file if they do not already exist.
     * 
     * @return std::string The path to the configuration folder.
     */
    std::string prepareConfigFile();
    /**
     * @brief Gets the path to the configuration folder.
     * 
     * This function gets the path to the configuration folder for the log4daily application.
     */
    std::string getConfigFolderPath();
    /**
     * @brief Creates the configuration folder.
     * 
     * This function creates the configuration folder for the log4daily application.
     */
    ConfigFolderStatus createConfigFolder(const std::string& path);
};

#endif // MANAGECONFIG_H