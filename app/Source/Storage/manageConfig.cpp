#include "../../Headers/Storage/manageConfig.h"

#include <iostream>
#include <fstream>

std::string ManageConfig::getConfigFolderPath() {
    std::string configPath;

#ifdef _WIN32
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path))) {
        configPath = std::string(path) + "\\log4daily";
    } else {
        std::cerr << "Error getting user documents folder path" << std::endl;
        exit(1);
    }
#elif __APPLE__
    configPath = "/Library/Application Support/log4daily";
#else
    const char* homeDir = getenv("HOME");
    if (homeDir != nullptr) {
        configPath = std::string(homeDir) + "/.log4daily";
    } else {
        std::cerr << "Error getting user home directory" << std::endl;
        exit(1);
    }
#endif

    return configPath;
}

ManageConfig::ConfigFolderStatus ManageConfig::createConfigFolder(const std::string& path) {
    struct stat info;

    if (stat(path.c_str(), &info) != 0) {
        if (MKDIR(path.c_str()) == 0) {
            std::cout << "Config folder created successfully: " << path << std::endl;
            return ConfigFolderStatus::CREATED;
        } else {
            std::cout << "Error creating config folder: " << path << std::endl;
            return ConfigFolderStatus::ERROR;
        }
    } else if (info.st_mode & S_IFDIR) {
        return ConfigFolderStatus::EXISTS;
    } else {
        return ConfigFolderStatus::ERROR;
    }
}

std::string ManageConfig::prepareConfigFile() {
    std::string configPath = getConfigFolderPath();
    ConfigFolderStatus status = createConfigFolder(configPath);

    if (status == ConfigFolderStatus::CREATED) {
        std::ofstream configFile(configPath + "/" + configFileName);
        configFile << "{ ";
        configFile << '"';
        configFile << "listOfFilesNames";
        configFile << '"';
        configFile << ": []";
        configFile << " }";
        configFile.close();
    } 

    switch (status) {
        case ConfigFolderStatus::CREATED:
        case ConfigFolderStatus::EXISTS:
            return configPath;
        case ConfigFolderStatus::ERROR:
            return "Problem acured while creating config folder";
            break;
    }

    return "Error!";
}
