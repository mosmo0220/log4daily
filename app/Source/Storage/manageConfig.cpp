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
    const char* configDir = getenv("XDG_CONFIG_HOME");
    const char* homeDir = getenv("HOME");
    
    if (configDir == nullptr && homeDir != nullptr) {
        configPath = std::string(homeDir) + "/.config/log4daily";
    }
    else if (configDir != nullptr) {
        configPath = std::string(configDir) + "/log4daily";
    } else {
        std::cerr << "Error getting user home directory" << std::endl;
        exit(1);
    }
#endif

    return configPath;
}

ConfigFolderStatus ManageConfig::createConfigFolder(const std::string& path) {
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

ConfigFolderStatus ManageConfig::prepareConfigFile() {
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

    return status;
}
