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

std::string configFileName = "configuration_log4daily.json";

std::string getConfigFolderPath() {
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
    configPath = "/etc/log4daily";
#endif

    return configPath;
}

enum class ConfigFolderStatus {
    CREATED,
    EXISTS,
    ERROR
};

ConfigFolderStatus createConfigFolder(const std::string& path) {
    struct stat info;

    if (geteuid() != 0 && stat(path.c_str(), &info) != 0) {
        std::cout << "This application must be run as root/sudo at configuration folder creation!" << std::endl;
        return ConfigFolderStatus::ERROR;
    }

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

std::string manageConfig() {
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
